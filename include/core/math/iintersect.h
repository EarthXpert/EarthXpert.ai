/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/opencv/exopencv.h"

namespace earthxpert::math {
/**
 *  The interface IIntersect defines the behaviours for the intersection of 3D geometries. 
 */
class EXCORE_EXPORTS IIntersect {
public:
  virtual ~IIntersect() = default;

  /**
   *  Intersect 3D line segements.
   *
   *  \param [in] segs the list of end points interlaeved by line segments, each is
   *  defined by two points.
   *  \param [out] p the intersection point.
   *  \param [in] numSegs number of line segments given by segs, -1 indicating dynamic
   *  \return Return true if successful, false if parallel.
   */
  virtual bool Intersect(const Point3d* segs, Point3d& p, int numSegs = -1) const = 0;
};
} // namespace earthxpert::math
