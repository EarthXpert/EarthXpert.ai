/* ----------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/math/intersect.h"
#include "3rdparty/ceres/exceres.h"
#include "3rdparty/eigen/exeigen.h"

#include <cassert>
#include <cmath>
#include <iostream>

using namespace::ceres;
using namespace::Eigen;

namespace earthxpert::math {
/**
 * Intersect two 3D line segements.
 *
 *  \return Return true if successful, false if parallel. 
 */
bool IntersectTwoLines(const Point3d* p1, const Point3d* p2, Point3d& p) {
  const Point3d v1 = p1[1] - p1[0];
  const Point3d v2 = p2[1] - p2[0];

  Eigen::Matrix<double, 6, 5> A;
  A << 1, 0, 0, -v1.x, 0,         // 1, 0, 0, -v1.x,
       0, 1, 0, -v1.y, 0,         // 0, 1, 0, -v1.y,
       0, 0, 1, -v1.z, 0,         // 0, 0, 1, -v1.z,
       1, 0, 0, 0, -v2.x,         // 1, 0, 0, -v2.x,
       0, 1, 0, 0, -v2.y,         // 0, 1, 0, -v2.y,
       0, 0, 1, 0, -v2.z;         // 0, 0, 1, -v2.z;

  Eigen::Matrix<double, 6, 1> b;
  b << p1[0].x, p1[0].y, p1[0].z, p2[0].x, p2[0].y, p2[0].z;

  const auto& x = A.bdcSvd(ComputeFullU | ComputeFullV).solve(b);
  //if () return false;

  p = {x[0], x[1], x[2]};
  return true;
}
} // namespace earthxpert::math
