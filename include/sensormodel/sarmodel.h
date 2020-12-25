/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/sensormodel.h"

namespace earthxpert {
/**
 *  The class SarModel implements the synthetic aperture radar geometry.
 */
class EXSENSORMODEL_EXPORTS SarModel final : public SensorModel{
public:
  ~SarModel() {}

  /**
   *  \brief Construct a SAR model.
   *
   *  \param [in] groundSRS ground coordinate system
   */
  SarModel(std::shared_ptr<OGRSpatialRef> groundSRS);

  SarModel(const SarModel& rhs);
  SarModel(SarModel&& rhs) noexcept;

  SarModel& operator=(const SarModel& rhs);
  SarModel& operator=(SarModel&& rhs) noexcept;

  bool operator==(const SarModel& rhs) const;
  bool operator!=(const SarModel& rhs) const;

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
  bool ImageToGround(const Point2d& image, Point3d& ground) const override;

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
  bool GroundToImage(const Point3d& ground, Point2d& image) const override;

  // access
  /**
   * @return type of sensor model
   */
  ModelType GetModelType() const noexcept override {
    return ModelType::eMT_SAR;
  }

  // ?block adjustment support?

private:
  bool EqualTo(const ISensorModel& rhs) const override;
};

using UPSarModel = std::unique_ptr<SarModel>;
using SPSarModel = std::shared_ptr<SarModel>;
} // namespace earthxpert
