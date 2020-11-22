/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/framemodel.h"
#include "3rdparty/opencv/exopencv.h"
#include "3rdparty/utils/constants.h"
#include "core/exceptions/exexcept.h"
#include "core/math/rotation.h"

#include <algorithm>
#include <cassert>
#include <numeric>

using namespace earthxpert;
using namespace earthxpert::math;

/**
 *  \brief Construct a geometry model considering distortions from
 *  various effects, such as atmospheric refraction, earth curvature, etc.
 *
 *  \param [in] imageModel transformation between focal plane and imagery
 *  \param [in] frameIP intrinsic parameters defining interior orientation
 *  \param [in] frameEP extrinsic parameters defining exterior orientation
 *  \param [in] frameCP correction parameters defining external effects
 */
template <class ImageModelPolicy>
FrameModel<ImageModelPolicy>::FrameModel(ImageModelPolicy &&imageModel,
                                         std::shared_ptr<FrameIP> frameIP,
                                         std::unique_ptr<FrameEP> &&frameEP,
                                         std::shared_ptr<FrameCP> frameCP)
    : mImageModel(std::move(imageModel)), mFrameIP(frameIP),
      mFrameEP(std::move(frameEP)), mFrameCP(frameCP) {
  const auto f = mFrameIP->GetFocalLength();
  mInvF2 = 1.0 / (f * f);
  if (mFrameCP->ApplyCurvature()) {
    mCurvFactor = 0.5 * mInvF2 / mFrameCP->GetEarthRadius();
  }

  CheckModel();
  CalcFusedEO();
}

/**
 *  Constructor.
 *
 *  \brief Construct a simple pin-hole geometry model.
 *
 *  \param [in] imageModel transformation between focal plane and imagery
 *  \param [in] frameIP intrinsic parameters defining interior orientation
 *  \param [in] frameEP extrinsic parameters defining exterior orientation
 */
template <class ImageModelPolicy>
FrameModel<ImageModelPolicy>::FrameModel(ImageModelPolicy &&imageModel,
                                         std::shared_ptr<FrameIP> frameIP,
                                         std::unique_ptr<FrameEP> &&frameEP)
    : FrameModel(std::move(imageModel), frameIP, std::move(frameEP),
                  std::make_shared<FrameCP>()) {
}

template <class ImageModelPolicy>
FrameModel<ImageModelPolicy>::FrameModel(
  const FrameModel<ImageModelPolicy>& rhs) {
  CopyFrom(rhs);
}

template <class ImageModelPolicy>
FrameModel<ImageModelPolicy>::FrameModel(
    FrameModel<ImageModelPolicy> &&rhs) noexcept {
  MoveFrom(std::move(rhs));
}

template <class ImageModelPolicy>
FrameModel<ImageModelPolicy> &FrameModel<ImageModelPolicy>::
operator=(const FrameModel<ImageModelPolicy> &rhs) {
  if (this != &rhs) {
    CopyFrom(rhs);
  }
  return *this;
}

template <class ImageModelPolicy>
FrameModel<ImageModelPolicy> &FrameModel<ImageModelPolicy>::
operator=(FrameModel<ImageModelPolicy> &&rhs) noexcept {
  if (this != &rhs) {
    MoveFrom(std::move(rhs));
  }
  return *this;
}

template <class ImageModelPolicy>
bool FrameModel<ImageModelPolicy>::
operator==(const FrameModel<ImageModelPolicy> &rhs) const {
  if (this != &rhs) {
    if (mImageModel != rhs.mImageModel) return false;
    if (*mFrameIP != *rhs.mFrameIP) return false;
    if (*mFrameEP != *rhs.mFrameEP) return false;
    if (*mFrameCP != *rhs.mFrameCP) return false;
    //if (*mFrameIPCorr != *rhs.mFrameIPCorr) return false;
    //if (*mFrameEPCorr != *rhs.mFrameEPCorr) return false;
  }
  return true;
}

template <class ImageModelPolicy>
bool FrameModel<ImageModelPolicy>::
operator!=(const FrameModel<ImageModelPolicy> &rhs) const {
  return !(operator==(rhs));
}

/**
 *  Image-to-ground forward projection.
 *
 *  \brief Given a 2d position in image coordinate system and a height
 *  in the ground vertical coordinate system, output the ground horizontal
 *  position with respect to the ground coordinate system.
 *
 *  \param [in] image 2d position (x, y) in imagery raster coordinate system
 *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate
 * system \return Returns true if the operation succeeds
 *
 *  \details
 */
template <class ImageModelPolicy>
bool FrameModel<ImageModelPolicy>::ImageToGround(const Point2d &image,
                                                 Point3d &ground) const {
  Point2d sensorAct = mImageModel.ImageToSensor(image);

  Point2d sensorApp; // apparant
  if (!SensorActualToApparant(sensorAct, ground.z, sensorApp))
    return false;

  return SensorToGround(sensorApp, ground);
}

/**
 *  Ground-to-image backward projection.
 *
 *  \brief Given a 3d ground position with reference to the ground coordinate
 * system, output the 2d image position in the image coordinate system.
 *
 *  \param [in] ground 3d position (X, Y, Z) in ground coordinate system
 *  \param [out] image 2d position (x, y) in imagery raster coordinate system
 *  \return Returns true if operation succeeds
 *
 *  \details
 */
template <class ImageModelPolicy>
bool FrameModel<ImageModelPolicy>::GroundToImage(const Point3d &ground,
                                                 Point2d &image) const {
  Point2d sensorApp;
  GroundToSensor(ground, sensorApp);

  Point2d sensorAct;
  if (!SensorApparantToActual(sensorApp, ground.z, sensorAct))
    return false;

  image = mImageModel.SensorToImage(sensorAct);
  return true;
}

/**
 *  \brief Fetch the ground coordinate system where the sensor model operates.
 *  Use this SRS to construct the coordinate transformations
 *  from results of ImageToGround() or inputs to GroundToImage() methods.
 *
 *  \return pointer to a coordinate system
 */
template <class ImageModelPolicy>
SPSpatialRefSys
FrameModel<ImageModelPolicy>::GetCoordinateSystem() const noexcept {
  return mFrameEP->GetCoordinateSystem();
}

/**
 *  \return Return raster size of image (numCols, numRows) [unit: pixel]
 */
template <class ImageModelPolicy>
std::pair<int, int> FrameModel<ImageModelPolicy>::GetImageSize() const noexcept {
  return mImageModel.GetImageSize();
}

/**
 *  \brief Check parameters if they represent a valid model.
 *  \except Throw invalid_arguments if it is a bad model.
 */
template <class ImageModelPolicy>
void FrameModel<ImageModelPolicy>::CheckModel() const {
  constexpr auto tol = std::numeric_limits<double>::epsilon();

  const auto f = mFrameIP->GetFocalLength();
  if (f < tol)
    throw std::invalid_argument(_("Focal length is too small."));
}

/**
 *  \brief Compare two models.
 *
 *  \param [in] rhs The other model to compare
 *  \return Return true if two models represent exactly same geometry
 */
template <class ImageModelPolicy>
bool FrameModel<ImageModelPolicy>::EqualTo(const ISensorModel &rhs) const {
  if (ISensorModel::ModelType::eFRAME != rhs.GetModelType())
    return false;

  auto frameModel = dynamic_cast<const FrameModel<ImageModelPolicy> *>(&rhs);
  assert(frameModel);

  return operator==(*frameModel);
}

/**
 *  Sensor-to-ground forward projection.
 *
 *  \brief Given a 2d position in sensor coordinate system and a height
 *  in the ground vertical coordinate system, output the ground horizontal
 *  position with respect to the ground coordinate system.
 *
 *  \param [in] sensor 2d position (x, y) in sensor focal plane coordinate
 * system
 *  \param [in/out] ground 3d ground position (X, Y, Z) in ground
 * coordinate system \return Returns true if the operation succeeds
 *  \return Return true if successful, otherwisr false.
 */
template <class ImageModelPolicy>
bool FrameModel<ImageModelPolicy>::SensorToGround(const Point2d &sensor,
                                                  Point3d &ground) const {
  const double f = mFrameIP->GetFocalLength();

  const Point3d cam{ sensor.x, sensor.y, -f };
  const cv::Mat grd = mG2S.t() * cv::Mat(cam);

  const double denom = grd.at<double>(2);
  if (denom < std::numeric_limits<double>::epsilon())
    throw std::underflow_error(_("Denominator is 0: SensorToGround"));

  const double scale = (ground.z - mSensorCenter.z) / denom;
  ground.x = mSensorCenter.x + grd.at<double>(0) * scale;
  ground.y = mSensorCenter.y + grd.at<double>(1) * scale;
  return true;
}

/**
 *  Ground-to-sensor backward projection.
 *
 *  \brief Given a 3d ground position with reference to the ground coordinate
 * system, output the 2d position in the sensor focal plane coordinate system.
 *
 *  \param [in] ground 3d ground position (X, Y, Z)
 *  \param [out] sensor 2d sensor position (x, y)
 *  \return Returns true if operation succeeds
 */
template <class ImageModelPolicy>
bool FrameModel<ImageModelPolicy>::GroundToSensor(const Point3d &ground,
                                                  Point2d &sensor) const {
  const double f = mFrameIP->GetFocalLength();

  const Point3d delta = ground - mSensorCenter;
  const cv::Mat cam = mG2S * cv::Mat(delta);

  const double denom = cam.at<double>(2);
  if (denom < std::numeric_limits<double>::epsilon())
      throw std::underflow_error(_("Denominator is 0: GroundToSensor"));

  const double scale = -f / denom;
  sensor.x = cam.at<double>(0) * scale;
  sensor.y = cam.at<double>(1) * scale;
  return true;
}

/**
 *  Actual-to-apparant convertion.
 *
 *  \brief Convert the measured actual position to the idea apparant position by
 * canceling corrects for various distortions in focal plane, including
 * radial/decentering optical distortions, grid distortion, atmospheric
 * refraction, and/or earth curvature.
 *
 *  \param [in] sensorAct the measured position where the ray actually hits the
 * focal plane
 *  \param [in] height the ground height above ellipsoid [m]
 *  \param [in] sensorApp the ideal position where the ray wpould apparantly
 * hit the focal plane if existing no distortions 
 *  \return Returns true if operation succeeds
 */
template <class ImageModelPolicy> bool 
FrameModel<ImageModelPolicy>::SensorActualToApparant(
  const Point2d& sensorAct, const double height, Point2d& sensorApp) const {
  // optical distortion in focal plane w.r.t. PPS
  sensorApp = SensorActualToApparantPPS(sensorAct);

  // camera coordinate system w.r.t. PPA
  sensorApp = mFrameIP->PPSToPPA(sensorApp);

  // atmospheric refraction & earth curvature
  sensorApp = SensorActualToApparantPPA(sensorApp, height);
  return true;
}

// optical distortion in focal plane w.r.t. PPS
template <class ImageModelPolicy> Point2d 
FrameModel<ImageModelPolicy>::SensorActualToApparantPPS(
  const Point2d& sensor) const {
  const auto dRad = mFrameIP->CalcRadialCorr(sensor);
  const auto dDec = mFrameIP->CalcDecentCorr(sensor);
  const auto dLin = mFrameIP->CalcLinearCorr(sensor);
  Point2d sensorApp = sensor + dRad + dDec + dLin;

  const auto dGrd = mFrameIP->CalcGridCorr(sensorApp);
  return (sensorApp + dGrd);
}

// vertical distortion in camera CS w.r.t. PPA
template <class ImageModelPolicy>
Point2d FrameModel<ImageModelPolicy>::SensorActualToApparantPPA(
  const Point2d& sensor, const double height) const {
  auto sensorApp = sensor;

  // atmospheric refraction
  if (mFrameCP->ApplyRefraction()) {
    const auto rcp2 = cv::normL2Sqr<double>(sensorApp);
    const auto H = mSensorCenter.z * 0.001; // [km]
    const auto h = height * 0.001;
    const auto K = mFrameCP->GetRefractionK(H, h);
    const auto factor = K * (1.0 + rcp2 * mInvF2);

    sensorApp -= sensorApp * factor;
  }

  // earth curvature
  if (mFrameCP->ApplyCurvature()) {
    const auto rcp2 = cv::normL2Sqr<double>(sensorApp);
    const auto Hh = mSensorCenter.z - height;
    const auto factor = rcp2 * Hh * mCurvFactor;

    sensorApp += sensorApp * factor;
  }

  return sensorApp;
}

// oblique distortion in camera CS w.r.t. Nadir Point
// \todo oblique equations
template <class ImageModelPolicy>
Point2d FrameModel<ImageModelPolicy>::SensorActualToApparantNP(
  const Point2d& sensor, const double height) const {
  return SensorActualToApparantPPA(sensor, height);
}

/**
 *  Apparant-to-actual convertion.
 *
 *  \brief Convert the idea apparant position to the measured actual position by
 * inserting corrects for various distortions in focal plane, including
 * radial/decentering optical distortions, grid distortion, atmospheric
 * refraction, and/or earth curvature.
 *
 *  \param [in] sensorApp the ideal position where the ray wpould apparantly hit
 * the focal plane if existing no distortions \param [in] height the ground
 * height above ellipsoid [m]
 *  \param [in] sensorAct the measured position where the ray actually hits
 * the focal plane \return Return true if the operation succeeds
 *  \return Returns true if operation succeeds
 */
template <class ImageModelPolicy>
bool FrameModel<ImageModelPolicy>::SensorApparantToActual(
  const Point2d& sensorApp, const double height, Point2d& sensorAct) const {

  // atmospheric refraction and earth curvature
  sensorAct = SensorApparantToActualPPA(sensorApp, height);

  // focal plane CS w.r.t. PPS
  sensorAct = mFrameIP->PPAToPPS(sensorAct);

  // optical distortion in focal plane CS w.r.t. PPS
  sensorAct = SensorApparantToActualPPS(sensorAct);
  return true;
}

// oblique distortion in camera CS w.r.t. Nadir Point
// \todo oblique equations
template <class ImageModelPolicy>
Point2d FrameModel<ImageModelPolicy>::SensorApparantToActualNP(
  const Point2d& sensor, const double height) const {
  return SensorApparantToActualPPA(sensor, height);
}

/**
 *  \brief vertical distortion in camera CS w.r.t. PPA at given height
 *
 *  \param [in] sensor apparant position on focal plane
 *  \param [in] height elevation of ground point
 */
template <class ImageModelPolicy>
Point2d FrameModel<ImageModelPolicy>::SensorApparantToActualPPA(
  const Point2d& sensor, const double height) const {
  auto sensorAct = sensor;

  int iter;

  // earth curvature
  if (mFrameCP->ApplyCurvature()) {
    const auto rcp = cv::norm(sensorAct);
    const double Hh = mSensorCenter.z - height;

    double rcP{ rcp }; // radius from previous iteration
    double rc{};     // new radius after correction

    for (iter = 0; iter < kMaxIterG2I; ++iter) {
      const auto rcP2 = rcP * rcP;
      const auto f_rcP = rcP + Hh * mCurvFactor * rcP2 * rcP - rcp;
      const auto df_drcP = 1.0 + 3.0 * Hh * mCurvFactor * rcP2;
      const auto corr = f_rcP / df_drcP;

      rc = rcP - corr;
      if (fabs(corr) < kTolMM)
        break;

      rcP = rc;
    }

    if (kMaxIterG2I == iter)
      return { 0.0, 0.0 };

    sensorAct *= rc / rcp;
  }

  // atmospheric refraction
  if (mFrameCP->ApplyRefraction()) {
    const auto rcp = cv::norm(sensorAct);
    const auto H = mSensorCenter.z * 0.001;
    const auto h = height * 0.001;
    const auto K = mFrameCP->GetRefractionK(H, h);

    double rcP{ rcp }, rc{};

    for (iter = 0; iter < kMaxIterG2I; ++iter) {
      const auto rcP2 = rcP * rcP;
      const auto f_rcP = rcP - K * rcP * (1.0 + rcP2 * mInvF2) - rcp;
      const auto df_drcP = 1.0 - K * (1.0 + 3.0 * rcP2 * mInvF2);
      const auto corr = f_rcP / df_drcP;

      rc = rcP - corr;
      if (fabs(corr) < kTolMM)
        break;

      rcP = rc;
    }

    if (kMaxIterG2I == iter)
      return { 0.0, 0.0 };

    sensorAct *= rc / rcp;
  }

  return sensorAct;
}

/**
 *  \brief optical distortion in focal plane CS w.r.t. PPS
 *
 *  \param [in] sensor apparant position on focal plane
 */
template <class ImageModelPolicy>
Point2d FrameModel<ImageModelPolicy>::SensorApparantToActualPPS(
  const Point2d& sensor) const {
  auto sensorAct = sensor;

  int iter;
  auto [hasRadD, hasDecD, hasLinD, hasGridD] = mFrameIP->GetDistFlags();

  // distortion grid from added parameters
  if (hasGridD) {
    const Point2d cRD{ sensorAct }; // fixed position

    for (iter = 0; iter < kMaxIterG2I; ++iter) {
      const auto dGrd = mFrameIP->CalcGridCorr(sensorAct);
      const Point2d res{ cRD - (sensorAct + dGrd) };
      sensorAct += res;

      if (fabs(res.x) < kTolMM && fabs(res.y) < kTolMM)
        break;
    }

    if (kMaxIterG2I == iter)
      return { 0.0, 0.0 };
  }

  if (hasRadD || hasDecD || hasLinD) {
    const Point2d cEarth{ sensorAct };

    for (iter = 0; iter < kMaxIterG2I; ++iter) {
      const auto dRad = mFrameIP->CalcRadialCorr(sensorAct);
      const auto dDec = mFrameIP->CalcDecentCorr(sensorAct);
      const auto dLin = mFrameIP->CalcLinearCorr(sensorAct);
      const auto corr = cEarth - (sensorAct + dRad + dDec + dLin);
      sensorAct += corr;

      if (fabs(corr.x) < kTolMM && fabs(corr.y) < kTolMM)
        break;

      // if corrX or corrY is large, it is not converging. use the original value
      if ((kMaxIterG2I - 1) == iter || 10 < fabs(corr.x) || 10 < fabs(corr.y)) {
        sensorAct = cEarth;
        break;
      }
    }

    if (kMaxIterG2I == iter)
      return { 0.0, 0.0 };
  }

  return sensorAct;
}

/**
 *  \brief helper of copy constructor & assignment.
 */
template <class ImageModelPolicy> void 
FrameModel<ImageModelPolicy>::CopyFrom(const FrameModel<ImageModelPolicy> &rhs) {
  mImageModel = rhs.mImageModel;
  mFrameIP = rhs.mFrameIP;
  mFrameEP = rhs.mFrameEP->Clone();
  mFrameCP = rhs.mFrameCP;
  mFrameIPCorr = rhs.mFrameIPCorr;
  mFrameEPCorr = rhs.mFrameEPCorr;
}

/**
 *  \brief helper of move constructor & assignment.
 */
template <class ImageModelPolicy> void
FrameModel<ImageModelPolicy>::MoveFrom(FrameModel<ImageModelPolicy> &&rhs) noexcept {
  mImageModel = std::move(rhs.mImageModel);
  mFrameIP = std::move(rhs.mFrameIP);
  mFrameEP = std::move(rhs.mFrameEP);
  mFrameCP = std::move(rhs.mFrameCP);
  mFrameIPCorr = std::move(rhs.mFrameIPCorr);
  mFrameEPCorr = std::move(rhs.mFrameEPCorr);
  rhs.Clear();
}

template <class ImageModelPolicy>
void FrameModel<ImageModelPolicy>::Clear() noexcept {
}

/**
 *  \brief fuse EO.
 */
template <class ImageModelPolicy>
void FrameModel<ImageModelPolicy>::CalcFusedEO() {
  // ground-to-sensor rotation, TODO mis-alignment
  auto ang = mFrameEP->GetRotation(); // [degree]
  const double angRad[3]{ang[0] * kDEG2RAD,
                         ang[1] * kDEG2RAD,
                         ang[2] * kDEG2RAD};
  EulerXYZToMatrix(angRad, mG2S);

  // sensor center position, TODO eccentricity
  auto pc = mFrameEP->GetPosition();
  mSensorCenter.x = pc[0];
  mSensorCenter.y = pc[1];
  mSensorCenter.z = pc[2];
}

/**
 *  \brief Frame camera has only two acquisition approaches: digital, film.
 */
template class FrameModel<DigitalImageModel>;
template class FrameModel<FilmImageModel>;
