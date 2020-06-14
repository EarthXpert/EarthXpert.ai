/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/opencv/exopencv.h"

namespace earthxpert::math {
bool EXCORE_EXPORTS IntersectTwoLines(const Point3d* p1, const Point3d* p2, Point3d& p);
} // namespace earthxpert::math
