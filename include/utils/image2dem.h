/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "utils/exutils.h"
#include "dem/idem.h"
#include "sensormodel/isensormodel.h"

namespace earthxpert {
/**
 *  The class Image2Dem project an image point to DEM.
 */
class EXUTILS_EXPORTS Image2Dem {
public:
  Image2Dem(std::shared_ptr<ISensorModel> sensorModel, std::shared_ptr<IDem> dem);
  ~Image2Dem() = default;

  void Project(const Point2d& image, Point3d& ground);

private:
  static constexpr int kMaxIterI2D = 10; // max. number of iterations allowed for Project

  void Initialize();

  std::shared_ptr<ISensorModel> mSensorModel;
  std::shared_ptr<IDem> mDem;
};
} // namespace earthxpert
