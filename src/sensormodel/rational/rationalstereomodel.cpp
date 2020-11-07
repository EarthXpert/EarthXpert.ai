/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/rationalstereomodel.h"
#include "3rdparty/eigen/exeigen.h"
#include "core/exceptions/exexcept.h"
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
 *  \param [in] leftImage 2d position (x, y) in left image raster CS
 *  \param [in] rightImage the corresponding 2d position in right image raster CS
 *  \param [in/out] ground 3d position (X, Y, Z) in ground coordinate system
 *  \return Returns true if the operation succeeds
 *
 *  \remark array & Map<> is 12% faster than direct sized Matrix<>;
 *          sized Matrix<> is 2x faster than dynamic Matrix<>
 */
bool RationalStereoModel::ImagesToGround(const Point2d& leftImage,
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

  const auto nfc0 = mRatModels[0]->GetNormalFactors();
  const auto nfc1 = mRatModels[1]->GetNormalFactors();

  int iter{};
  for (iter = 0; iter < kMaxIters; ++iter) {
    //std::atomic_fetch_add_explicit(&mNumIters, 1, std::memory_order_relaxed);

    Point2d imgp0, imgp1;
    std::tie(dm[0], dm[1], dm[2], dm[3], dm[4], dm[5]) =
      CalcJacobianNc(mRatModels[0], ground, imgp0);
    std::tie(dm[6], dm[7], dm[8], dm[9], dm[10], dm[11]) =
      CalcJacobianNc(mRatModels[1], ground, imgp1);

    dm[0] *= nfc0[eSSAMP]; dm[1] *= nfc0[eSSAMP]; dm[2] *= nfc0[eSSAMP];
    dm[3] *= nfc0[eSLINE]; dm[4] *= nfc0[eSLINE]; dm[5] *= nfc0[eSLINE];
    dm[6] *= nfc1[eSSAMP]; dm[7] *= nfc1[eSSAMP]; dm[8] *= nfc1[eSSAMP];
    dm[9] *= nfc1[eSLINE]; dm[10] *= nfc1[eSLINE]; dm[11] *= nfc1[eSLINE];

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
  return (kMaxIters > iter);
}

/**
 *  Image-to-ground forward projection (variable).
 *
 *  \brief Given a variable number of sensor models and associated 2d positions
 *  in image coordinate system, do forward intersection of rays, and
 *  output the ground position with respect to the ground coordinate system.
 *
 *  \param [in] sensorModels an array of sensor models.
 *  \param [in] images the corresponding 2d positions (x, y) in
 * imagery raster coordinate systems, interleaved by sensor models. \param
 * [in/out] ground 3d ground position (X, Y, Z) in ground coordinate system
 *  \return Returns true if the operation succeeds
 */
bool RationalStereoModel::ImagesToGround(
  const std::vector<const RationalModel*>& ratModels,
  const std::vector<Point2d>& images, Point3d& ground) const {
  const int numRays = int(ratModels.size());
  assert(numRays == images.size());
  ground.z = 0.0;

  std::vector<Point2d> imgs(numRays);
  for (int i = 0; i < numRays; ++i) {
    imgs[i] = ratModels[i]->ImageToSensor(images[i]);
    ground.z += ratModels[i]->GetMeanHeight();
  }
  ground.z /= double(numRays);
  ratModels[numRays / 2]->ImageToGround(images[numRays / 2], ground);

  Point2d imgp;
  MatrixXd A(numRays * 2, 3);
  VectorXd b(numRays * 2);

  int iter{};
  for (iter = 0; iter < kMaxIters; ++iter) {
    //std::atomic_fetch_add_explicit(&mNumIters, 1, std::memory_order_relaxed);

    for (int i = 0; i < numRays; ++i) {
      const auto [dFc_dL, dFc_dP, dFc_dH, dFr_dL, dFr_dP, dFr_dH] =
        CalcJacobianNc(ratModels[i], ground, imgp);

      const int i2 = i * 2;
      const auto nfc = ratModels[i]->GetNormalFactors();
      A(i2, 0) = dFc_dL * nfc[eSSAMP];  // sensor-to-pixel scaling
      A(i2, 1) = dFc_dP * nfc[eSSAMP];
      A(i2, 2) = dFc_dH * nfc[eSSAMP];
      A(i2 + 1, 0) = dFr_dL * nfc[eSLINE];
      A(i2 + 1, 1) = dFr_dP * nfc[eSLINE];
      A(i2 + 1, 2) = dFr_dH * nfc[eSLINE];

      b[i2] = (imgp.x - imgs[i].x) * nfc[eSSAMP];
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
  return (kMaxIters > iter);
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
  Duvw_duvw(u, v, w, duvw_du, duvw_dv, duvw_dw);

  const auto [dSn_du, dSn_dv, dSn_dw] = D_duvw(ratModel->snc, duvw_du, duvw_dv, duvw_dw); // Partial derivative of Sn w.r.t u, v,w
  const auto [dSd_du, dSd_dv, dSd_dw] = D_duvw(ratModel->sdc, duvw_du, duvw_dv, duvw_dw); // Partial derivative of Sd w.r.t u, v, w
  const auto [dLn_du, dLn_dv, dLn_dw] = D_duvw(ratModel->lnc, duvw_du, duvw_dv, duvw_dw); // Partial derivative of Ln w.r.t u, v,w
  const auto [dLd_du, dLd_dv, dLd_dw] = D_duvw(ratModel->ldc, duvw_du, duvw_dv, duvw_dw); // Partial derivative of Ld w.r.t u, v, w

  const auto dFc_duvw = (Point3d(dSn_du, dSn_dv, dSn_dw) - \
    image.x * Point3d(dSd_du, dSd_dv, dSd_dw)) * invSd;  // dFc_du, dFc_dv, dFc_dw
  const auto dFr_duvw = (Point3d(dLn_du, dLn_dv, dLn_dw) - \
    image.y * Point3d(dLn_du, dLn_dv, dLn_dw)) * invLd;  // dFr_du, dFr_dv, dFr_dw

  const double du_dL = ratModel->mInvScale[risf::eISLON];    // 1.0 / nfc[rnf::eSLON]; u = (Lon - nfc[rnf::eOLON]) / nfc[rnf::eSLON]
  const double dv_dP = ratModel->mInvScale[risf::eISLAT];     // 1.0 / nfc[rnf::eSLAT]; v = (Lat - nfc[rnf::eOLAT])/ nfc[rnf::eSLAT]
  const double dw_dH = ratModel->mInvScale[risf::eISHGT];   // 1.0 / nfc[rnf::eSHGT]; w = (Hgt - nfc[rnf::eOHGT]) / nfc[rnf::eSHGT]

  return { dFc_duvw.x * du_dL, dFc_duvw.y * dv_dP, dFc_duvw.z * dw_dH, 
           dFr_duvw.x * du_dL, dFr_duvw.y * dv_dP, dFr_duvw.z * dw_dH };
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

  const auto dF_dL = (imgs[0] - image) * kFacLP;  // dFc_dL, dFr_dL
  const auto dF_dP = (imgs[1] - image) * kFacLP;  // dFc_dP, dFr_dP
  const auto dF_dH = (imgs[2] - image) * kFacH;   // dFc_dH, dFr_dH

  return { dF_dL.x, dF_dP.x, dF_dH.x, dF_dL.y, dF_dP.y, dF_dH.y };
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

  const auto dF_dL = (imgs[1] - imgs[0]) * kFacLP;  // dFc_dL, dFr_dL
  const auto dF_dP = (imgs[3] - imgs[2]) * kFacLP;  // dFc_dP, dFr_dP
  const auto dF_dH = (imgs[5] - imgs[4]) * kFacH;   // dFc_dH, dFr_dH

  return { dF_dL.x, dF_dP.x, dF_dH.x, dF_dL.y, dF_dP.y, dF_dH.y };
}
