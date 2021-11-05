/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/isensormodel.h"

namespace earthxpert {
/**
 *  The class NullSensorModel provides mock functios doing nothing.
 */
class EXSENSORMODEL_EXPORTS NullSensorModel final : public ISensorModel {
public:
  ~NullSensorModel() = default;

  bool operator==(const NullSensorModel& rhs) const override { return true; }
  bool operator!=(const NullSensorModel& rhs) const override { return false; }

  std::unique_ptr<NullSensorModel> Clone() const override { return nullptr; }

  bool ImageToGround(const Point2d& image, 
    Point3d& ground) const override { return false; }

  bool operator()(const Point2d& image, 
    Point3d& ground) const override { return false; }

  size_t ImageToGround(const Point2d &image,
    std::vector<Point3d>& ground) const override { return 0; }

  size_t ImageToGround(const std::vector<Point2d> &image,
    std::vector<Point3d>& ground) const override { return 0; }

  bool GroundToImage(const Point3d &ground, 
    Point2d &image) const override { return false; }

  bool operator()(const Point3d& ground, 
    Point2d& image) const override { return false; }

  size_t GroundToImage(const std::vector<Point3d> &ground,
    std::vector<Point2d> &image) const override { return 0; }

  ModelType GetModelType() const noexcept override { return SensorType::eUNDEFINED; }

  std::shared_ptr<SpatialReferenceSystem>
    GetCoordinateSystem() const noexcept override { return nullptr; }

  /**
   *  \return Returns type of SRS
   */
  SrsType GetSrsType() const noexcept override { return SrsType::eUNKNOWN; }

  std::pair<int, int> GetImageSize() const noexcept override { return {0, 0}; }

  double GetMeanHeight() const noexcept override { return 0; }
};

using UPNullSensorModel = std::unique_ptr<NullSensorModel>;
using SPNullSensorModel = std::shared_ptr<NullSensorModel>;
} // namespace earthxpert
