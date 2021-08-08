/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/pushbroommodel.h"
#include "3rdparty/gdal/exgdal.h"
#include "core/exceptions/exexcept.h"

using namespace earthxpert;

/**
 *  \brief Construct an aerial pushbroom geometry model.
 *
 *  \param [in] imgModel image model defining raster vs focal plane relationship
 *  \param [in] scannerIP intrinsic parameters defining scanner interior
 * orientation 
 *  \param [in] scannerEP extrinsic parameters defining scanner exterior orientation 
 *  \param [in] groundSRS ground coordinate reference system
 */
template <typename CraftPolicy> PushbroomModel<CraftPolicy>::PushbroomModel(
    std::shared_ptr<ScannerImageModel> imgModel,
    std::unique_ptr<ScannerIP> &&scannerIP,
    //LineTime lineTime,
    std::shared_ptr<ScannerEP> scannerEP,
    SPSpatialRefSys groundSRS) {}

/**
 *  \brief Construct a satellite pushbroom geometry model.
 *
 *  \param [in] imgModel image model defining raster vs focal plane relationship
 *  \param [in] scannerIP intrinsic parameters defining scanner interior
 * orientation 
 *  \param [in] scannerEP extrinsic parameters defining scanner
 * exterior orientation 
 *  \param [in] satelliteDP distortion parameters defining external effects 
 *  \param [in] groundSRS ground coordinate reference system
 */
template <typename CraftPolicy> PushbroomModel<CraftPolicy>::PushbroomModel(
    std::shared_ptr<ScannerImageModel> imgModel,
    std::unique_ptr<ScannerIP> &&scannerIP,
    //LineTime lineTime,
    std::shared_ptr<ScannerEP> scannerEP,
    std::shared_ptr<SatelliteDP> satelliteDP,
    SPSpatialRefSys groundSRS) {}

template <typename CraftPolicy> 
PushbroomModel<CraftPolicy>::PushbroomModel(const PushbroomModel<CraftPolicy>&rhs) {}

template <typename CraftPolicy> 
PushbroomModel<CraftPolicy>::PushbroomModel(PushbroomModel &&rhs) noexcept {}

template <typename CraftPolicy> PushbroomModel<CraftPolicy>& 
PushbroomModel<CraftPolicy>::operator=(const PushbroomModel<CraftPolicy>&rhs) {
  return *this;
}

template <typename CraftPolicy> PushbroomModel<CraftPolicy>& 
PushbroomModel<CraftPolicy>::operator=(PushbroomModel<CraftPolicy>&&rhs) noexcept {
  return *this;
}

template <typename CraftPolicy> bool 
PushbroomModel<CraftPolicy>::operator==(const PushbroomModel<CraftPolicy>&rhs) const {
  return true;
}

template <typename CraftPolicy> bool 
PushbroomModel<CraftPolicy>::operator!=(const PushbroomModel<CraftPolicy>&rhs) const {
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
 */
template <typename CraftPolicy> bool 
PushbroomModel<CraftPolicy>::ImageToGround(const Point2d &image,
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
 */
template <typename CraftPolicy> size_t
PushbroomModel<CraftPolicy>::ImageToGround(const Point2d &image,
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
 */
template <typename CraftPolicy> bool 
PushbroomModel<CraftPolicy>::GroundToImage(const Point3d &ground,
                                   Point2d &image) const {
  return true;
}

/**
 *  \brief Set correction model in ground space.
 *
 *  \param [in] scannerEPCorr ephemeris & attitude corrections
 */
template <typename CraftPolicy>
void PushbroomModel<CraftPolicy>::SetCorrection(
    std::shared_ptr<ScannerEPCorr> scannerEPCorr) {}

template <typename CraftPolicy>
bool PushbroomModel<CraftPolicy>::EqualTo(const ISensorModel &rhs) const {
  if (ISensorModel::ModelType::eMT_PUSHBROOM != rhs.GetModelType())
    return false;

  auto pbModel = dynamic_cast<const PushbroomModel *>(&rhs);
  assert(pbModel);

  return operator==(*pbModel);
}

template <typename CraftPolicy>
void PushbroomModel<CraftPolicy>::BuildCubeG2S() {

}

template class earthxpert::PushbroomModel<AircraftPolicy>;
template class earthxpert::PushbroomModel<SpacecraftPolicy>;
