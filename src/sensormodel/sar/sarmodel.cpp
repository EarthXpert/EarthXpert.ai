/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/sarmodel.h"

using namespace earthxpert;

/**
 *  \brief Construct a multi-layer ANN model.
 *
 *  \param [in] groundSRS ground coordinate system
 */
SarModel::SarModel(std::shared_ptr<OGRSpatialRef> groundSRS) {}

SarModel::SarModel(const SarModel& rhs) {}

SarModel::SarModel(SarModel&& rhs) noexcept {}

SarModel& SarModel::operator=(const SarModel& rhs) { return *this; }

SarModel& SarModel::operator=(SarModel&& rhs) noexcept {
  return *this;
}

bool SarModel::operator==(const SarModel& rhs) const { return true; }

bool SarModel::operator!=(const SarModel& rhs) const {
  return !(operator==(rhs));
}

/**
 *  Image-to-ground forward projection.
 *
 *  \brief Given a 2d position in image coordinate system and a height
 *  in the ground vertical coordinate system, output the ground horizontal
 *  position with respect to the ground coordinate system.
 *
 *  1. Range equation (sphere): (Xpc - X)^2 + (Ypc - Y)^2 + (Zpc - Z)^2 = R^2
 *  2. Doppler equation (squint plane): point (Xpc, Ypc, Zpc), normal (Vx, Vy, Vz)
 *  3. Earth model (inflated ellipsoid): (X^2 + Y^2) / (A + h)^2 + Z^2 / (B + h)^2 = 1
 *  4. Circle: intersection of plane and sphere
 *  5. Ellipse: intersection of plane and ellipsoid
 *  6. point: intersection of circel and ellipse
 *  7. iterative algorithm: calculate height from XYZ, and correct h in Eq. 3
 * 
 *  \param [in] image 2d position (x, y) in imagery raster coordinate system
 *  \param [in/out] ground 3d ground position (X, Y, Z) in ground CS 
  * \return Returns true if the operation succeeds
 */
bool SarModel::ImageToGround(const Point2d& image, Point3d& ground) const {

  return true;
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
bool SarModel::GroundToImage(const Point3d& ground, Point2d& image) const {
  return true;
}

bool SarModel::EqualTo(const ISensorModel& rhs) const {
  if (ISensorModel::ModelType::eMT_SAR != rhs.GetModelType())
    return false;

  auto sarModel = dynamic_cast<const SarModel*>(&rhs);
  assert(sarModel);

  return operator==(*sarModel);
}

void SarModel::BuildCubeG2S() {

}
