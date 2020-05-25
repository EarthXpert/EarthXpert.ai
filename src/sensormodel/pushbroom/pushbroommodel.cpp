/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/pushbroommodel.h"
#include "3rd_party/gdal/exgdal.h"

using earthxpert::ISensorModel;
using earthxpert::Point2d;
using earthxpert::Point3d;
using earthxpert::PushBroomModel;
using earthxpert::SatelliteDP;
using earthxpert::ScannerEP;
using earthxpert::ScannerEPCorr;
using earthxpert::ScannerImageModel;
using earthxpert::ScannerIP;
using earthxpert::SpatialReferenceSystem;

/**
 *  \brief Construct an aerial pushbroom geometry model.
 *
 *  \param [in] imgModel image model defining raster vs focal plane relationship
 *  \param [in] scannerIP intrinsic parameters defining scanner interior
 * orientation \param [in] scannerEP extrinsic parameters defining scanner
 * exterior orientation \param [in] groundSRS ground coordinate reference system
 */
PushBroomModel::PushBroomModel(
    std::shared_ptr<ScannerImageModel> imgModel,
    std::unique_ptr<ScannerIP> &&scannerIP,
    std::shared_ptr<ScannerEP> scannerEP,
    std::shared_ptr<SpatialReferenceSystem> groundSRS) {}

/**
 *  \brief Construct a satellite pushbroom geometry model.
 *
 *  \param [in] imgModel image model defining raster vs focal plane relationship
 *  \param [in] scannerIP intrinsic parameters defining scanner interior
 * orientation \param [in] scannerEP extrinsic parameters defining scanner
 * exterior orientation \param [in] satelliteDP distortion parameters defining
 * external effects \param [in] groundSRS ground coordinate reference system
 */
PushBroomModel::PushBroomModel(
    std::shared_ptr<ScannerImageModel> imgModel,
    std::unique_ptr<ScannerIP> &&scannerIP,
    std::shared_ptr<ScannerEP> scannerEP,
    std::shared_ptr<SatelliteDP> satelliteDP,
    std::shared_ptr<SpatialReferenceSystem> groundSRS) {}

PushBroomModel::PushBroomModel(const PushBroomModel &rhs) {}

PushBroomModel::PushBroomModel(PushBroomModel &&rhs) noexcept {}

PushBroomModel &PushBroomModel::operator=(const PushBroomModel &rhs) {
  return *this;
}

PushBroomModel &PushBroomModel::operator=(PushBroomModel &&rhs) noexcept {
  return *this;
}

bool PushBroomModel::operator==(const PushBroomModel &rhs) const {
  return true;
}

bool PushBroomModel::operator!=(const PushBroomModel &rhs) const {
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
bool PushBroomModel::ImageToGround(const Point2d &image,
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
size_t PushBroomModel::ImageToGround(const Point2d &image,
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
bool PushBroomModel::GroundToImage(const Point3d &ground,
                                   Point2d &image) const {
  return true;
}

/**
 *  \brief Set correction model in ground space.
 *
 *  \param [in] scannerEPCorr ephemeris & attitude corrections
 */
void PushBroomModel::SetCorrection(
    std::shared_ptr<ScannerEPCorr> scannerEPCorr) {}

bool PushBroomModel::EqualTo(const ISensorModel &rhs) const {
  if (ISensorModel::ModelType::ePUSHBROOM != rhs.GetModelType())
    return false;

  auto pbModel = dynamic_cast<const PushBroomModel *>(&rhs);
  assert(pbModel);

  return operator==(*pbModel);
}
