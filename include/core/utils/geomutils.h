/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/boost/exboost.h"
#include "3rdparty/opencv/exopencv.h"

namespace earthxpert::utils {
std::pair<Point2d, Point2d> EXCORE_EXPORTS CalcBound(
  const Polygon2d& fp, int posTfm = 16);
} // namespace earthxpert::utils
