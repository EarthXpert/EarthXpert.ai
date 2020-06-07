/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rd_party/opencv/exopencv.h"

#include <utility>

namespace earthxpert::math {
/**
 *  \breif fit a parameterized stright line to a sequence of 2D points.
 *  The line equation is defined by
 *
 *    y = m * x + c
 *
 *  where
 *
 *    y: output variable
 *    x: input variable
 *    m: the slope parameter
 *    c: the intercept parameter
 *
 *  \param [in] points a sequence, at least two, of 2D points
 *  \return Return line parameters m and c
 */
std::pair<double, double> EXCORE_EXPORTS FitStraightLine(const std::vector<Point2d>& points);
std::pair<double, double> EXCORE_EXPORTS FitStraightLineRobust(const std::vector<Point2d>& points);
} // namespace earthxpert::math
