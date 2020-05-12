/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/networkmodel.h"

using watxtract::ISensorModel;
using watxtract::NetworkModel;
using watxtract::SpatialReferenceSystem;

/**
 *  \brief Construct a multi-layer ANN model.
 *
 *  \param [in] networkI2G multi-layer ANN for image-to-ground
 *  \param [in] networkG2I multi-layer ANN for ground-to-image
 *  \param [in] normalFators normalization factors including offsets & scales
 *  \param [in] groundSRS ground coordinate system
 */
NetworkModel::NetworkModel(std::unique_ptr<cv::ANN_MLP> &&networkI2G,
                           std::unique_ptr<cv::ANN_MLP> &&networkG2I,
                           const std::array<double, 10> &normalFators,
                           std::shared_ptr<SpatialReferenceSystem> groundSRS) {}

NetworkModel::NetworkModel(const NetworkModel &rhs) {}

NetworkModel::NetworkModel(NetworkModel &&rhs) noexcept {}

NetworkModel &NetworkModel::operator=(const NetworkModel &rhs) { return *this; }

NetworkModel &NetworkModel::operator=(NetworkModel &&rhs) noexcept {
  return *this;
}

bool NetworkModel::operator==(const NetworkModel &rhs) const { return true; }

bool NetworkModel::operator!=(const NetworkModel &rhs) const {
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
bool NetworkModel::ImageToGround(const Point2d &image, Point3d &ground) const {
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
bool NetworkModel::GroundToImage(const Ponint3d &ground, Point2d &image) const {
  return true;
}

bool NetworkModel::EqualTo(const ISensorModel &rhs) const {
  if (ISensorModel::ModelType::eNETWORK != rhs.GetModelType())
    return false;

  auto netModel = dynamic_cast<const NetworkModel *>(&rhs);
  assert(netModel);

  return operator==(*netModel);
}
