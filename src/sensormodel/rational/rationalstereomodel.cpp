/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/rationalstereomodel.h"
#include "3rdparty/eigen/exeigen.h"
#include "sensormodel/rationalmodel.h"

using namespace earthxpert;
using namespace::Eigen;

/**
 *  Constructor.
 *
 *  \brief Form a stereo model by a pair of sensor models.
 *
 *  \param [in] leftModel left sensor model composing the stereo model
 *  \param [in] rightModel right sensor model composing the stereo model
 */
RationalStereoModel::RationalStereoModel(
  RationalModel const* const leftModel, RationalModel const* const rightModel) 
  : StereoModel(static_cast<const ISensorModel*>(leftModel),
                static_cast<const ISensorModel*>(rightModel)) {
  mModels.resize(2);
  mModels[0] = leftModel;
  mModels[1] = rightModel;
  Initialize();
}

void RationalStereoModel::Initialize() {
  mFwdAlgo = eSM_EXAI;
}

/**
 *  Image-to-ground backward projection (fixed).
 *
 *  \brief Given a pair of sensor models and associated 2d positions
 *  in image coordinate system, do forward intersection of pair-rays, and
 *  output the ground position with respect to the ground coordinate system.
 *
 *  \param [in] leftImage 2d position (x, y) in left image raster coordinate
 * system
 *  \param [in] rightImage the corresponding 2d position (x, y) in right image
 * raster coordinate system
 *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate
 * system
 *  \return Returns true if the operation succeeds
 */
bool RationalStereoModel::ImageToGround(const Point2d& leftImage,
  const Point2d& rightImage, Point3d& ground) const {
  assert(2 == mModels.size());
  //std::vector<Point2d> imgs{ leftImage, rightImage };
  //return ImageToGround(imgs, ground);
  
  Point2d imageApp = mModels[0]->ImageActualToApparant(Point2d{ leftImage.x - 0.5, leftImage.y - 0.5 });
  auto leftImg = mModels[0]->ImageToSensor(imageApp);
  imageApp = mModels[1]->ImageActualToApparant(Point2d{ rightImage.x - 0.5, rightImage.y - 0.5 });
  auto rightImg = mModels[1]->ImageToSensor(imageApp);

  ground.z = (mModels[0]->GetMeanHeight() + mModels[1]->GetMeanHeight()) / 2.0;
  mModels[0]->ImageToGround(leftImage, ground);

  int iter{};
  for (iter = 0; iter < kMaxIters; ++iter) {
    Point2d imgp0;
    auto [dFc_dL0, dFc_dP0, dFc_dH0, dFr_dL0, dFr_dP0, dFr_dH0] =
      CalcJacobian(mModels[0], ground, imgp0);
    auto nfc0 = mModels[0]->GetNormalFators();

    Point2d imgp1;
    auto [dFc_dL1, dFc_dP1, dFc_dH1, dFr_dL1, dFr_dP1, dFr_dH1] =
      CalcJacobian(mModels[1], ground, imgp1);
    auto nfc1 = mModels[1]->GetNormalFators();

    std::array<double, 12> dm = { 
      dFc_dL0* nfc0[eSSAMP], dFc_dP0* nfc0[eSSAMP], dFc_dH0* nfc0[eSSAMP],
      dFr_dL0* nfc0[eSLINE], dFr_dP0* nfc0[eSLINE], dFr_dH0* nfc0[eSLINE],
      dFc_dL1* nfc1[eSSAMP], dFc_dP1* nfc1[eSSAMP], dFc_dH1* nfc1[eSSAMP],
      dFr_dL1* nfc1[eSLINE], dFr_dP1* nfc1[eSLINE], dFr_dH1* nfc1[eSLINE] };
    Map<Eigen::Matrix<double, 4, 3, RowMajor>> A(dm.data());

    std::array<double, 4> cv = { (imgp0.x - leftImg.x) * nfc0[eSSAMP],
                                 (imgp0.y - leftImg.y) * nfc0[eSLINE],
                                 (imgp1.x - rightImg.x)* nfc1[eSSAMP],
                                 (imgp1.y - rightImg.y)* nfc1[eSLINE] };        
    Map<Eigen::Matrix<double, 4, 1>> b(cv.data());

    const auto& x = A.bdcSvd(ComputeFullU | ComputeFullV).solve(b);
    //if () return false;

    ground.x -= x[0];
    ground.y -= x[1];
    ground.z -= x[2];

    if (fabs(x[0]) < kTolD && fabs(x[1]) < kTolD && fabs(x[2]) < kTolH)
      break;
  }

  // return false if reaching max. iterations
  if (kMaxIters == iter) {
    return false;
  }

  return true;
}

/**
 *  \brief Given a triplet of sensor models and associated 2d positions
 *  in image coordinate system, do forward intersection of triple-rays, and
 *  output the ground position with respect to the ground coordinate system.
 *
 *  \param [in] image the corresponding 2d positions (x, y) in imagery raster
 * coordinate systems
 *  \param [in/out] ground 3d ground position (X, Y, Z) in
 * ground coordinate system \return Returns true if the operation succeeds
 *  \return Returns true if the operation succeeds
 */
bool RationalStereoModel::ImageToGround(
  const std::array<Point2d, 3>& images, Point3d& ground) const {
  assert(3 == mModels.size());
  std::vector<Point2d> imgs(images.cbegin(), images.cend());

  return ImageToGround(imgs, ground);
}

/**
 *  \brief Given two or more sensor models and associated 2d positions
 *  in image coordinate system, do forward intersection of rays, and
 *  output the ground position with respect to the ground coordinate system.
 *
 *  \param [in] images the corresponding 2d positions (x, y) in
 *  imagery raster coordinate systems, interleaved by sensor models.
 *  \param [in/out] ground 3d ground position (Lon, Lat, Height) in ground coordinate system
 *  \return Returns true if the operation succeeds
 *  \todo regularization of normal matrix A.t() * A
 *  \todo revist the stability of solve() for Dynamic Ax=b
 */
bool RationalStereoModel::ImageToGround(
  const std::vector<Point2d>& images, Point3d& ground) const {
  const int numRays = int(mModels.size());
  assert(numRays == images.size());
  ground.z = 0.0;

  std::vector<Point2d> imgs(numRays);
  for (int i = 0; i < numRays; ++i) {
    Point2d imageAct{ images[i].x - 0.5, images[i].y - 0.5 };
    const auto imageApp = mModels[i]->ImageActualToApparant(imageAct);
    imgs[i] = mModels[i]->ImageToSensor(imageApp);

    ground.z += mModels[i]->GetMeanHeight();
  }
  ground.z /= double(numRays);
  mModels[numRays/2]->ImageToGround(images[numRays/2], ground);

  MatrixXd A(numRays * 2, 3);
  VectorXd b(numRays * 2);

  int iter{};
  for (iter = 0; iter < kMaxIters; ++iter) {
    for (int i = 0; i < numRays; ++i)
    {
      Point2d imgp;
      auto [dFc_dL, dFc_dP, dFc_dH, dFr_dL, dFr_dP, dFr_dH] =
        CalcJacobian(mModels[i], ground, imgp);

      auto nfc = mModels[i]->GetNormalFators();
      const int i2 = i * 2;
      A(i2, 0) = dFc_dL * nfc[eSSAMP];
      A(i2, 1) = dFc_dP * nfc[eSSAMP];
      A(i2, 2) = dFc_dH * nfc[eSSAMP];
      A(i2 + 1, 0) = dFr_dL * nfc[eSLINE];
      A(i2 + 1, 1) = dFr_dP * nfc[eSLINE];
      A(i2 + 1, 2) = dFr_dH * nfc[eSLINE];

      b[i2]     = (imgp.x - imgs[i].x) * nfc[eSSAMP];
      b[i2 + 1] = (imgp.y - imgs[i].y) * nfc[eSLINE];
    }

    // A.bdcSvd(ComputeThinU | ComputeThinV).solve(b)
    // A.colPivHouseholderQr().solve(b)
    const auto& x = (A.transpose() * A).inverse() * (A.transpose() * b);
    //if () return false;

    ground.x -= x[0];
    ground.y -= x[1];
    ground.z -= x[2];

    if (fabs(x[0]) < kTolD && fabs(x[1]) < kTolD && fabs(x[2]) < kTolH)
      break;
  }

  // return false if reaching max. iterations
  if (kMaxIters == iter) {
    return false;
  }

  return true;
}

/**
 *  \brief Calculate Jacobian of rational functions at given ground poistion.
 *
 *  \return Return {dFc_dL, dFc_dP, dFc_dH, dFr_dL, dFr_dP, dFr_dH}
 */
Tuple6d RationalStereoModel::CalcJacobian(RationalModel const* ratModel,
  const Point3d& ground, Point2d& image) const {
  // use short name for convenience
  auto snc = ratModel->mSampNumCoef.data(); // Sample numerator coefficients
  auto sdc = ratModel->mSampDenCoef.data(); // Sample denominator coefficients
  auto lnc = ratModel->mLineNumCoef.data(); // Line numerator coefficients
  auto ldc = ratModel->mLineDenCoef.data(); // Line denominator coefficients

  auto [u, v, w] = ratModel->GroundNormalize(ground);
  const auto [Ln, Ld, Sn, Sd] = ratModel->CalcPolynomials(u, v, w);

  const auto invSd = 1.0 / Sd;
  const auto invLd = 1.0 / Ld;

  // estimated normalized col/row
  image.x = Sn * invSd;
  image.y = Ln * invLd;

  double duvw_du[RationalModel::NumCoef];
  double duvw_dv[RationalModel::NumCoef];
  double duvw_dw[RationalModel::NumCoef];
  Duvw_du(u, v, w, duvw_du);
  Duvw_dv(u, v, w, duvw_dv);
  Duvw_dv(u, v, w, duvw_dw);

  const double dSn_du = D_du(snc, duvw_du);      // Partial derivative of Sn w.r.t u
  const double dSd_du = D_du(sdc, duvw_du);      // Partial derivative of Sd w.r.t u
  const double dSn_dv = D_dv(snc, duvw_dv);      // Partial derivative of Sn w.r.t v
  const double dSd_dv = D_dv(sdc, duvw_dv);      // Partial derivative of Sd w.r.t v
  const double dSn_dw = D_dw(snc, duvw_dw);      // Partial derivative of Sn w.r.t w
  const double dSd_dw = D_dw(sdc, duvw_dw);      // Partial derivative of Sd w.r.t w

  const double dLn_du = D_du(lnc, duvw_du);      // Partial derivative of Ln w.r.t u
  const double dLd_du = D_du(ldc, duvw_du);      // Partial derivative of Ld w.r.t u
  const double dLn_dv = D_dv(lnc, duvw_dv);      // Partial derivative of Ln w.r.t v
  const double dLd_dv = D_dv(ldc, duvw_dv);      // Partial derivative of Ld w.r.t v
  const double dLn_dw = D_dw(lnc, duvw_dw);      // Partial derivative of Ln w.r.t w
  const double dLd_dw = D_dw(ldc, duvw_dw);      // Partial derivative of Ld w.r.t w

  const double dFc_du = (dSn_du - image.x * dSd_du) * invSd;
  const double dFc_dv = (dSn_dv - image.x * dSd_dv) * invSd;
  const double dFc_dw = (dSn_dw - image.x * dSd_dw) * invSd;
  const double dFr_du = (dLn_du - image.y * dLd_du) * invLd;
  const double dFr_dv = (dLn_dv - image.y * dLd_dv) * invLd;
  const double dFr_dw = (dLn_dw - image.y * dLd_dw) * invLd;

  auto nfc = ratModel->GetNormalFators();
  const double du_dL = 1.0 / nfc[rnf::eSLONG];    // Lon
  const double dv_dP = 1.0 / nfc[rnf::eSLAT];     // Lat
  const double dw_dH = 1.0 / nfc[rnf::eSHEIGHT];  // Hgt

  return { dFc_du * du_dL, dFc_dv * dv_dP, dFc_dw * dw_dH, 
           dFr_du * du_dL, dFr_dv * dv_dP, dFr_dw * dw_dH };
}
