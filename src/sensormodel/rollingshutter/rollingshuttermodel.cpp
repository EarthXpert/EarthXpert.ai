/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/rollingshuttermodel.h"
#include "3rdparty/gdal/exgdal.h"
#include "sensormodel/scannerep.h"
#include "sensormodel/scannerepcorr.h"
#include "sensormodel/utils/frameip.h"
#include "sensormodel/utils/imagemodel.h"
#include "sensormodel/utils/satellitedp.h"

using namespace earthxpert;

/**
 *  \brief Construct an aerial rolling-shutter geometry model.
 *
 *  \param [in] imgModel image model defining raster vs focal plane relationship
 *  \param [in] scannerIP intrinsic parameters defining scanner interior
 * orientation \param [in] scannerEP extrinsic parameters defining scanner
 * exterior orientation \param [in] groundSRS ground coordinate reference system
 */
RollingShutterModel::RollingShutterModel(
    std::shared_ptr<DigitalImageModelImp> imgModel, std::shared_ptr<FrameIP> scannerIP,
    std::shared_ptr<ScannerEP> scannerEP,
    SPSpatialRefSys groundSRS) {}

/**
 *  \brief Construct a satellite rolling-shutter geometry model.
 *
 *  \param [in] imgModel image model defining raster vs focal plane relationship
 *  \param [in] scannerIP intrinsic parameters defining scanner interior
 * orientation \param [in] scannerEP extrinsic parameters defining scanner
 * exterior orientation \param [in] satelliteDP distortion parameters defining
 * external effects \param [in] groundSRS ground coordinate reference system
 */
RollingShutterModel::RollingShutterModel(
    std::shared_ptr<DigitalImageModelImp> imgModel, std::shared_ptr<FrameIP> scannerIP,
    std::shared_ptr<ScannerEP> scannerEP,
    std::shared_ptr<SatelliteDP> satelliteDP,
    SPSpatialRefSys groundSRS) {}

RollingShutterModel::RollingShutterModel(const RollingShutterModel &rhs) {}

RollingShutterModel::RollingShutterModel(RollingShutterModel &&rhs) noexcept {}

RollingShutterModel &RollingShutterModel::
operator=(const RollingShutterModel &rhs) {
  return *this;
}

RollingShutterModel &RollingShutterModel::
operator=(RollingShutterModel &&rhs) noexcept {
  return *this;
}

bool RollingShutterModel::operator==(const RollingShutterModel &rhs) const {
  return true;
}

bool RollingShutterModel::operator!=(const RollingShutterModel &rhs) const {
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
 *  \details More details
 */
bool RollingShutterModel::ImageToGround(const Point2d &image,
                                        Point3d &ground) const {
  return true;
}

/**
 *  \brief Given a 2d position in image coordinate system and a list of
 *  heights, output ground horizontal positions in ground coordinate system.
 *
 *  \param [in] image 2d position (x, y) in imagery raster coordinate system
 *  \param [in/out] ground 3d ground positions (X, Y, Z) in ground coordinate
 * system
 *
 *  \details More details
 */
size_t RollingShutterModel::ImageToGround(const Point2d &image,
                                          std::vector<Point3d> &ground) const {
  return 0;
}

/**
 *  Ground-to-image backward projection.
 *
 *  \brief Given a 3d ground position with reference to the ground coordinate
 * system, output the 2d image position in the image coordinate system.
 *
 *  \param [in] ground 3d ground position (X, Y, Z)
 *  \param [out] image 2d image position (x, y)
 *  \return Returns true if operation succeeds
 *
 *  \details More details
 */
bool RollingShutterModel::GroundToImage(const Point3d &ground,
                                        Point2d &image) const {
  return true;
}

/**
 *  \brief Set correction model in ground space.
 *
 *  \param [in] scannerEPCorr ephemeris & attitude corrections
 */
void RollingShutterModel::SetCorrection(
    std::shared_ptr<ScannerEPCorr> scannerEPCorr) {}

bool RollingShutterModel::EqualTo(const ISensorModel &rhs) const {
  if (ISensorModel::ModelType::eROLLING != rhs.GetModelType())
    return false;

  auto rsModel = dynamic_cast<const RollingShutterModel *>(&rhs);
  assert(rsModel);

  return operator==(*rsModel);
}
