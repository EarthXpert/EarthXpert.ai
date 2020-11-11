/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/utils/istereomodel.h"

namespace earthxpert {
/**
 *  The class NullStereoModel provides mock functions doing nothing.
 */
class EXSENSORMODEL_EXPORTS NullStereoModel final : public IStereoModel {
public:
  ~NullStereoModel() = default;

  bool ImagesToGround(const Point2d &leftImage,
    const Point2d &rightImage, Point3d &ground) const override { return false; }

  size_t ImagesToGround(const std::vector<Point2d> &leftImages,
    const std::vector<Point2d> &rightImages, 
    std::vector<Point3d> &grounds) const override { return 0; }

  bool ImagesToGround(const std::array<Point2d, 3> &images, 
    Point3d &ground) const override { return false; }

  bool ImagesToGround(const std::vector<Point2d> &images, 
    Point3d &ground) const override { return false; }

  bool ImagesToGround(const std::vector<const ISensorModel*>& sensorModels,
    const std::vector<Point2d>& images, Point3d& ground) const override {
    return false;
  }

  size_t ImagesToGround(const std::vector<Point2d> &images, 
    std::vector<Point3d> &grounds) const override { return 0; }

  bool GroundToImages(const Point3d& ground, 
    std::vector<Point2d>& images) const override { return false; }

  bool GroundToImages(const std::vector<const ISensorModel*>& sensorModels, 
    const Point3d& ground, std::vector<Point2d>& images) const override {
    return false;
  }

  size_t GroundToImages(const std::vector<Point3d> &grounds, 
    std::vector<Point2d> &images) const override { return 0; }
};

using UPNullStereoModel = std::unique_ptr<NullStereoModel>;
using SPNullStereoModel = std::shared_ptr<NullStereoModel>;
} // namespace earthxpert
