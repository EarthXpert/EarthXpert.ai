/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/sensormodel.h"

namespace earthxpert {
/**
 *  The class SarModel implements the synthetic aperture radar geometry.
 *
 *  \details range doppler model
 *
 *  1. range equation:
 *
 *    Rs^2  = (X - Xs)^2 + (Y - Ys)^2 + (Z - Zs)^2 = (R0 + Ms * col)^2
 *
 *  2. doppler equation
 *
 *    Vx * (X - Xs) + Vy * (Y - Ys) + Vz * (Z - Zs) = -lambda * Rs * f / 2
 *
 *  where
 *
 *    R0 - near slant range
 *    Rs - slant range from phase center to ground point
 *    Ms - slant range sampling distance
 *    X, Y, Z - position of ground point
 *    row - row poistion of image pixel
 *    col - column poistion of image pixel
 *    lambda - wavelength
 *    f - doppler central freaquency
 *
 *    Xs, Ys, Zs - position of antena phase center
 *    Xs = Xs0 + Vx0 * t + Ax0 * t^2 + ...
 *    Ys = Ys0 + Vy0 * t + Ay0 * t^2 + ...
 *    Zs = Zs0 + Vz0 * t + Az0 * t^2 + ...

 *    Vx, Vy, Vz - velocity of antena phase center
 *    Vx  = Vx0 + 2 * Ax0 * t + ...
 *    Vy  = Vy0 + 2 * Ay0 * t + ...
 *    Vz  = Vz0 + 2 * Az0 * t + ...
 *
 *  where
 *
 *     t = t0 + dt * row - imaging time of line/row
 *     t0 - start base time of the first line/row
 *     dt - time interval between two contigeous lines
 *     row - row position of image pixel
 */
class EXSENSORMODEL_EXPORTS SarModel final : public SensorModel{
public:
  enum {
    eModelType = ModelType::eMT_SAR
  };

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
