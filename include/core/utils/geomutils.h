/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "core/utils/rasterwindow.h"
#include "3rdparty/boost/exboost.h"
#include "3rdparty/opencv/exopencv.h"

namespace earthxpert::utils {
std::pair<Point2d, Point2d> EXCORE_EXPORTS CalcBound(
  const Polygon2d& poly, int posTfm = 16);
std::vector<Point2i> EXCORE_EXPORTS ConvertPolygon(const Polygon2d& poly);
Polygon2d EXCORE_EXPORTS ConvertPolygon(const RasterWindow& win);
} // namespace earthxpert::utils
