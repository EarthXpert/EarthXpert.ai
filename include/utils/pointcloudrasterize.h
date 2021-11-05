/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "utils/exutils.h"
#include "core/datasource/pointcloud.h"

namespace earthxpert {
/**
 *  The class PointcloudRasterize converts point clouds into an image.
 */
class EXUTILS_EXPORTS PointcloudRasterize {
public:
  PointcloudRasterize();
  ~PointcloudRasterize() = default;

  void Process();

private:
};
} // namespace earthxpert
