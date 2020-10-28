/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/rationalstereomodel.h"
#include "3rdparty/eigen/exeigen.h"
#include "core/exceptions/exexcept.h"
#include "sensormodel/rationalmodel.h"
#include "sensormodel/rmutils.h"

using namespace earthxpert;
using namespace ::Eigen;

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
  mRatModels.resize(2);
  mRatModels[0] = leftModel;
  mRatModels[1] = rightModel;
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
 *
 *  \remark array & Map<> is 12% faster than direct sized Matrix<>;
 *          sized Matrix<> is 2x faster than dynamic Matrix<>
 */
bool RationalStereoModel::ImageToGround(const Point2d& leftImage,
  const Point2d& rightImage, Point3d& ground) const {
  assert(2 == mRatModels.size());
  //std::vector<Point2d> imgs{ leftImage, rightImage };
  //return ImageToGround(imgs, ground);
  
  const auto leftImg = mRatModels[0]->ImageToSensor(leftImage);
  const auto rightImg = mRatModels[1]->ImageToSensor(rightImage);

  ground.z = (mRatModels[0]->GetMeanHeight() + mRatModels[1]->GetMeanHeight()) / 2.0;
  mRatModels[0]->ImageToGround(leftImage, ground);

  std::array<double, 12> dm;
  std::array<double, 4> cv;

  Map<Eigen::Matrix<double, 4, 3, RowMajor>> A(dm.data());
  Map<Eigen::Matrix<double, 4, 1>> b(cv.data());

  const auto nfc0 = mRatModels[0]->GetNormalFators();
  const auto nfc1 = mRatModels[1]->GetNormalFators();

  int iter{};
  for (iter = 0; iter < kMaxIters; ++iter) {
    //std::atomic_fetch_add_explicit(&mNumIters, 1, std::memory_order_relaxed);

    Point2d imgp0, imgp1;
    const auto [dFc_dL0, dFc_dP0, dFc_dH0, dFr_dL0, dFr_dP0, dFr_dH0] =
      CalcJacobianNc(mRatModels[0], ground, imgp0);
    const auto [dFc_dL1, dFc_dP1, dFc_dH1, dFr_dL1, dFr_dP1, dFr_dH1] =
      CalcJacobianNc(mRatModels[1], ground, imgp1);

    dm[0] = dFc_dL0 * nfc0[eSSAMP];
    dm[1] = dFc_dP0 * nfc0[eSSAMP];
    dm[2] = dFc_dH0 * nfc0[eSSAMP];
    dm[3] = dFr_dL0 * nfc0[eSLINE];
    dm[4] = dFr_dP0 * nfc0[eSLINE];
    dm[5] = dFr_dH0 * nfc0[eSLINE];
    dm[6] = dFc_dL1 * nfc1[eSSAMP];
    dm[7] = dFc_dP1 * nfc1[eSSAMP];
    dm[8] = dFc_dH1 * nfc1[eSSAMP];
    dm[9] = dFr_dL1 * nfc1[eSLINE];
    dm[10] = dFr_dP1 * nfc1[eSLINE];
    dm[11] = dFr_dH1 * nfc1[eSLINE];

    cv[0] = (imgp0.x - leftImg.x) * nfc0[eSSAMP];
    cv[1] = (imgp0.y - leftImg.y) * nfc0[eSLINE];
    cv[2] = (imgp1.x - rightImg.x) * nfc1[eSSAMP];
    cv[3] = (imgp1.y - rightImg.y) * nfc1[eSLINE];

    const auto& x = A.bdcSvd(ComputeFullU | ComputeFullV).solve(b);
    //if () return false;

    ground -= Point3d(x[0], x[1], x[2]);

    if (fabs(x[0]) < kTolXYD && fabs(x[1]) < kTolXYD && fabs(x[2]) < kTolZ)
      break;
  }

  // return false if reaching max. iterations
  if (kMaxIters == iter) 
    return false;

  return true;
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
  const int numRays = int(mRatModels.size());
  assert(numRays == images.size());
  ground.z = 0.0;

  std::vector<Point2d> imgs(numRays);
  for (int i = 0; i < numRays; ++i) {
    imgs[i] = mRatModels[i]->ImageToSensor(images[i]);
    ground.z += mRatModels[i]->GetMeanHeight();
  }
  ground.z /= double(numRays);
  mRatModels[numRays/2]->ImageToGround(images[numRays/2], ground);

  Point2d imgp;
  MatrixXd A(numRays * 2, 3);
  VectorXd b(numRays * 2);

  int iter{};
  for (iter = 0; iter < kMaxIters; ++iter) {
    //std::atomic_fetch_add_explicit(&mNumIters, 1, std::memory_order_relaxed);

    for (int i = 0; i < numRays; ++i) {
      const auto [dFc_dL, dFc_dP, dFc_dH, dFr_dL, dFr_dP, dFr_dH] =
        CalcJacobianNc(mRatModels[i], ground, imgp);

      const auto nfc = mRatModels[i]->GetNormalFators();
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

    ground -= Point3d(x[0], x[1], x[2]);

    if (fabs(x[0]) < kTolXYD && fabs(x[1]) < kTolXYD && fabs(x[2]) < kTolZ)
      break;
  }

  // return false if reaching max. iterations
  if (kMaxIters == iter) 
    return false;

  return true;
}

/**
 *  \brief Calculate Jacobian of rational functions at given ground position (analytic).
 *
 *  \param [out] image estimated normalized col/row
 *  \return Return {dFc_dL, dFc_dP, dFc_dH, dFr_dL, dFr_dP, dFr_dH}
 */
Tuple6d RationalStereoModel::CalcJacobianA(RationalModel const* ratModel,
  const Point3d& ground, Point2d& image) const {
  auto [u, v, w] = ratModel->GroundNormalize(ground);
  const auto [Ln, Ld, Sn, Sd] = ratModel->CalcPolynomials(u, v, w);

  const auto invSd = 1.0 / Sd;
  const auto invLd = 1.0 / Ld;
  image.x = Sn * invSd;
  image.y = Ln * invLd;

  double duvw_du[RationalModel::NumCoef];
  double duvw_dv[RationalModel::NumCoef];
  double duvw_dw[RationalModel::NumCoef];
  Duvw_du(u, v, w, duvw_du);
  Duvw_dv(u, v, w, duvw_dv);
  Duvw_dv(u, v, w, duvw_dw);

  // use short name for convenience
  const auto snc = ratModel->mSampNumCoef.data(); // Sample numerator coefficients
  const auto sdc = ratModel->mSampDenCoef.data(); // Sample denominator coefficients
  const double dSn_du = D_du(snc, duvw_du);      // Partial derivative of Sn w.r.t u
  const double dSd_du = D_du(sdc, duvw_du);      // Partial derivative of Sd w.r.t u
  const double dSn_dv = D_dv(snc, duvw_dv);      // Partial derivative of Sn w.r.t v
  const double dSd_dv = D_dv(sdc, duvw_dv);      // Partial derivative of Sd w.r.t v
  const double dSn_dw = D_dw(snc, duvw_dw);      // Partial derivative of Sn w.r.t w
  const double dSd_dw = D_dw(sdc, duvw_dw);      // Partial derivative of Sd w.r.t w

  const auto lnc = ratModel->mLineNumCoef.data(); // Line numerator coefficients
  const auto ldc = ratModel->mLineDenCoef.data(); // Line denominator coefficients
  const double dLn_du = D_du(lnc, duvw_du);      // Partial derivative of Ln w.r.t u
  const double dLd_du = D_du(ldc, duvw_du);      // Partial derivative of Ld w.r.t u
  const double dLn_dv = D_dv(lnc, duvw_dv);      // Partial derivative of Ln w.r.t v
  const double dLd_dv = D_dv(ldc, duvw_dv);      // Partial derivative of Ld w.r.t v
  const double dLn_dw = D_dw(lnc, duvw_dw);      // Partial derivative of Ln w.r.t w
  const double dLd_dw = D_dw(ldc, duvw_dw);      // Partial derivative of Ld w.r.t w

  const double dFc_du = (dSn_du - image.x * dSd_du) * invSd;    // x = Sn / Sd
  const double dFc_dv = (dSn_dv - image.x * dSd_dv) * invSd;
  const double dFc_dw = (dSn_dw - image.x * dSd_dw) * invSd;
  const double dFr_du = (dLn_du - image.y * dLd_du) * invLd;    // y = Ln / Ld
  const double dFr_dv = (dLn_dv - image.y * dLd_dv) * invLd;
  const double dFr_dw = (dLn_dw - image.y * dLd_dw) * invLd;

  const double du_dL = ratModel->mInvScale[risf::eISLONG];    // 1.0 / nfc[rnf::eSLONG]; u = (Lon - nfc[rnf::eOLONG]) / nfc[rnf::eSLONG]
  const double dv_dP = ratModel->mInvScale[risf::eISLAT];     // 1.0 / nfc[rnf::eSLAT]; v = (Lat - nfc[rnf::eOLAT])/ nfc[rnf::eSLAT]
  const double dw_dH = ratModel->mInvScale[risf::eISHEIGHT];  // 1.0 / nfc[rnf::eSHEIGHT]; w = (Hgt - nfc[rnf::eOHEIGHT]) / nfc[rnf::eSHEIGHT]

  return { dFc_du * du_dL, dFc_dv * dv_dP, dFc_dw * dw_dH, 
           dFr_du * du_dL, dFr_dv * dv_dP, dFr_dw * dw_dH };
}

/**
 *  \brief Calculate Jacobian of rational functions at given ground position
 *  (forward finite difference).
 *
 *  \param [out] image estimated normalized col/row
 *  \return Return {dFc_dL, dFc_dP, dFc_dH, dFr_dL, dFr_dP, dFr_dH}
 *
 *  \remark 2x slower than CalcJacobianA
 */
Tuple6d RationalStereoModel::CalcJacobianNf(RationalModel const* ratModel,
  const Point3d& ground, Point2d& image) const {
  ratModel->GroundToSensor(ground, image);

  Point3d grds[3] = { {ground.x + kTolXYD, ground.y, ground.z},
                      {ground.x, ground.y + kTolXYD, ground.z},
                      {ground.x, ground.y, ground.z + kTolZ} };
  Point2d imgs[3];

  for (int i = 0; i < 3; ++i) {
    ratModel->GroundToSensor(grds[i], imgs[i]);
  }

  constexpr double facLP = 0.5 / kTolXYD;
  constexpr double facH = 0.5 / kTolZ;

  const double dFc_dL = (imgs[0].x - image.x) * facLP;
  const double dFc_dP = (imgs[1].x - image.x) * facLP;
  const double dFc_dH = (imgs[2].x - image.x) * facH;
  const double dFr_dL = (imgs[0].y - image.y) * facLP;
  const double dFr_dP = (imgs[1].y - image.y) * facLP;
  const double dFr_dH = (imgs[2].y - image.y) * facH;

  return { dFc_dL, dFc_dP, dFc_dH, dFr_dL, dFr_dP, dFr_dH };
}

/**
 *  \brief Calculate Jacobian of rational functions at given ground position 
 *  (central finite difference).
 *
 *  \param [out] image estimated normalized col/row
 *  \return Return {dFc_dL, dFc_dP, dFc_dH, dFr_dL, dFr_dP, dFr_dH}
 *
 *  \remark 25% faster than CalcJacobianA
 */
Tuple6d RationalStereoModel::CalcJacobianNc(RationalModel const* ratModel,
  const Point3d& ground, Point2d& image) const {
  ratModel->GroundToSensor(ground, image);

  Point3d grds[6] = { 
    {ground.x - kTolXYD, ground.y, ground.z}, {ground.x + kTolXYD, ground.y, ground.z},
    {ground.x, ground.y - kTolXYD, ground.z}, {ground.x, ground.y + kTolXYD, ground.z},
    {ground.x, ground.y, ground.z - kTolZ}, {ground.x, ground.y, ground.z + kTolZ} };
  Point2d imgs[6];

  for (int i = 0; i < 6; ++i) {
    ratModel->GroundToSensor(grds[i], imgs[i]);
  }

  constexpr double facLP = 0.5 / kTolXYD;
  constexpr double facH = 0.5 / kTolZ;

  const double dFc_dL = (imgs[1].x - imgs[0].x) * facLP;
  const double dFc_dP = (imgs[3].x - imgs[2].x) * facLP;
  const double dFc_dH = (imgs[5].x - imgs[4].x) * facH;
  const double dFr_dL = (imgs[1].y - imgs[0].y) * facLP;
  const double dFr_dP = (imgs[3].y - imgs[2].y) * facLP;
  const double dFr_dH = (imgs[5].y - imgs[4].y) * facH;

  return { dFc_dL, dFc_dP, dFc_dH, dFr_dL, dFr_dP, dFr_dH };
}
