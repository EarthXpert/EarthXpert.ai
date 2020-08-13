/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/adsmodel.h"
#include "3rdparty/gdal/exgdal.h"
#include "core/exceptions/exexcept.h"
#include "sensormodel/scannerep.h"
#include "sensormodel/scannerimagemodel.h"
#include "sensormodel/scannerip.h"

using namespace earthxpert;

/**
 *  \brief Construct an aerial pushbroom geometry model.
 *
 *  \param [in] imgModel image model defining raster vs focal plane relationship
 *  \param [in] scannerIP intrinsic parameters defining scanner interior
 * orientation \param [in] scannerEP extrinsic parameters defining scanner
 * exterior orientation \param [in] origin origin of the LSR coordinate system
 *  \param [in] groundSRS ground coordinate reference system
 */
AdsModel::AdsModel(std::shared_ptr<ScannerImageModel> imgModel,
                   std::shared_ptr<ScannerIP> scannerIP,
                   std::shared_ptr<ScannerEP> scannerEP, const Point2d &origin,
                   std::shared_ptr<SpatialReferenceSystem> groundSRS) {}

AdsModel::AdsModel(const AdsModel &rhs) {}

AdsModel::AdsModel(AdsModel &&rhs) noexcept {}

AdsModel &AdsModel::operator=(const AdsModel &rhs) { return *this; }

AdsModel &AdsModel::operator=(AdsModel &&rhs) noexcept { return *this; }

bool AdsModel::operator==(const AdsModel &rhs) const { return true; }

bool AdsModel::operator!=(const AdsModel &rhs) const {
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
bool AdsModel::ImageToGround(const Point2d &image, Point3d &ground) const {
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
size_t AdsModel::ImageToGround(const Point2d &image,
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
bool AdsModel::GroundToImage(const Point3d &ground, Point2d &image) const {
  return true;
}

bool AdsModel::EqualTo(const ISensorModel &rhs) const {
  if (ISensorModel::ModelType::eADS != rhs.GetModelType())
    return false;

  auto adsModel = dynamic_cast<const AdsModel *>(&rhs);
  assert(adsModel);

  return operator==(*adsModel);
}
