/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/sensormodel.h"

#include "opencv2/ml.hpp"
#include <array>

namespace watxtract {
// class cv::ml::ANN_MLP;

/**
 *  The class NetworkModel implements the multi-layer ANN
 *  model for various physical sensors.
 */
class WXSENSORMODEL_EXPORTS NetworkModel final : public SensorModel {
public:
  ~NetworkModel() {}

  /**
   *  \brief Construct a multi-layer ANN model.
   *
   *  \param [in] networkI2G multi-layer ANN for image-to-ground
   *  \param [in] networkG2I multi-layer ANN for ground-to-image
   *  \param [in] normalFators normalization factors including offsets & scales
   *  \param [in] groundSRS ground coordinate system
   */
  NetworkModel(std::unique_ptr<ANN_MLP> &&networkI2G,
               std::unique_ptr<ANN_MLP> &&networkG2I,
               const std::array<double, 10> &normalFators,
               std::shared_ptr<SpatialReferenceSystem> groundSRS);

  NetworkModel(const NetworkModel &rhs);
  NetworkModel(NetworkModel &&rhs) noexcept;

  NetworkModel &operator=(const NetworkModel &rhs);
  NetworkModel &operator=(NetworkModel &&rhs) noexcept;

  bool operator==(const NetworkModel &rhs) const;
  bool operator!=(const NetworkModel &rhs) const;

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
  bool ImageToGround(const Point2d &image, Point3d &ground) const override;

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
  bool GroundToImage(const Ponint3d &ground, Point2d &image) const override;

  // access
  /**
   * @return type of sensor model
   */
  ModelType GetModelType() const noexcept override {
    return ModelType::eNETWORK;
  }

  // ?block adjustment support?

private:
  std::unique_ptr<ANN_MLP>
      mNetworkS2G{}; // multi-layer ANN for sensor-to-ground
  std::unique_ptr<ANN_MLP> mNetworkG2S{}; // multi-layer ANN for Ground-to-image
  std::array<double, 10>
      mNormalFators{}; // normalization factors including offsets & scales

  bool EqualTo(const ISensorModel &rhs) const override;
};
} // namespace watxtract