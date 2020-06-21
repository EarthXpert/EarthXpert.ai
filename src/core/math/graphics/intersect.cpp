/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/math/intersect.h"

#include <cassert>
#include <cmath>
#include <iostream>

using namespace earthxpert::math;
using namespace::ceres;
using namespace::Eigen;

// construcctor
Intersect::Intersect() {
  mOptions.max_num_iterations = 20;
  mOptions.num_threads = 1; // kNumPhysicalCores
  //mOptions.function_tolerance = 1e-4;  // [0.1mm]
  //mOptions.gradient_tolerance = 1e-8;
  //mOptions.parameter_tolerance = 1e-4;  // [0.1mm]
  mOptions.linear_solver_type = DENSE_QR; // DENSE_NORMAL_CHOLESKY
  //mOptions.preconditioner_type = JACOBI;  // SUBSET
  mOptions.minimizer_progress_to_stdout = false;
  //mOptions.update_state_every_iteration = true;
  //mOptions.callbacks = {};
}

/**
 *  Intersect two 3D line segements (ceres).
 *
 *  \param [in] segs the list of four end points interlaeved by line segments,
 *  each is defined by two points.
 *  \param [out] p the intersection point.
 *  \param [out] summary the report of ceres solver.
 *  \return Return true if successful, false if parallel.
 */
bool XTwoLines::IntersectCeres(const Point3d* segs, Point3d& p, 
  Solver::Summary& summary, const int) const {
  std::vector<double> pb(2, 0.5);
  const bool ret = IntersectGeometries<Point3d, AutoLineIntersectResidual>(
    segs, 2, pb, mOptions, summary);

  const Point3d v1 = segs[1] - segs[0];
  const Point3d v2 = segs[3] - segs[2];
  const Point3d p1 = segs[0] + v1 * pb[0];
  const Point3d p2 = segs[2] + v2 * pb[1];
  p = (p1 + p2) * 0.5;
  return ret;
}

/**
 *  Intersect two 3D line segements (Eigen).
 *
 *  \param [in] s1 the first line segment defined by two end points
 *  \param [in] s2 the second line segment defined by two end points
 *  \return Return true if successful, false if parallel.
 */
bool XTwoLines::IntersectEigen(const Point3d* s1, const Point3d* s2, Point3d& p) const {
  const Point3d v1 = s1[1] - s1[0];
  const Point3d v2 = s2[1] - s2[0];

  std::array<double, 30> dm{ 1, 0, 0, -v1.x, 0,
                             0, 1, 0, -v1.y, 0,
                             0, 0, 1, -v1.z, 0,
                             1, 0, 0, 0, -v2.x,
                             0, 1, 0, 0, -v2.y,
                             0, 0, 1, 0, -v2.z };
  Map<Eigen::Matrix<double, 6, 5, RowMajor>> A(dm.data());

  std::array<double, 6> cv{ s1[0].x, s1[0].y, s1[0].z, s2[0].x, s2[0].y, s2[0].z };
  Map<Eigen::Matrix<double, 6, 1>> b(cv.data());

  const auto& x = A.bdcSvd(ComputeFullU | ComputeFullV).solve(b);
  //if () return false;

  p = { x[0], x[1], x[2] };
  return true;
}

/**
 *  Intersect three 3D line segements (ceres).
 *
 *  \param [in] segs the list of six end points interlaeved by line segments, 
 *  each is defined by two points.
 *  \param [out] p the intersection point.
 *  \param [out] summary the report of ceres solver.
 *  \return Return true if successful, false if parallel.
 */
bool XThreeLines::IntersectCeres(const Point3d* segs, 
  Point3d& p, Solver::Summary& summary, const int) const {
  std::vector<double> pb(3, 0.5);
  const bool ret = IntersectGeometries<Point3d, NumericLineIntersectResidual>(
    segs, 3, pb, mOptions, summary);

  const auto v1 = segs[1] - segs[0];
  const auto v2 = segs[3] - segs[2];
  const auto v3 = segs[5] - segs[4];
  const auto p1 = segs[0] + v1 * pb[0]; // t1
  const auto p2 = segs[2] + v2 * pb[1]; // t2
  const auto p3 = segs[4] + v3 * pb[2]; // t3
  p = (p1 + p2 + p3) / 3;
  return ret;
}

/**
 *  Intersect three 3D line segements (Eigen).
 *
 *  \param [in] s1 the first line segment defined by two end points
 *  \param [in] s2 the second line segment defined by two end points
 *  \param [in] s3 the second line segment defined by two end points
 *  \return Return true if successful, false if parallel.
 */
bool XThreeLines::IntersectEigen(const Point3d* s1, const Point3d* s2, 
  const Point3d* s3, Point3d& p) const {
  const Point3d v1 = s1[1] - s1[0];
  const Point3d v2 = s2[1] - s2[0];
  const Point3d v3 = s2[1] - s2[0];

  std::array<double, 54> dm{ 1, 0, 0, -v1.x, 0, 0,
                             0, 1, 0, -v1.y, 0, 0,
                             0, 0, 1, -v1.z, 0, 0,
                             1, 0, 0, 0, -v2.x, 0,
                             0, 1, 0, 0, -v2.y, 0,
                             0, 0, 1, 0, -v2.z, 0,
                             1, 0, 0, 0, 0, -v3.x,
                             0, 1, 0, 0, 0, -v3.y,
                             0, 0, 1, 0, 0, -v3.z };
  Map<Eigen::Matrix<double, 9, 6, RowMajor>> A(dm.data());

  std::array<double, 9> cv{ s1[0].x, s1[0].y, s1[0].z, s2[0].x, s2[0].y, s2[0].z, s3[0].x, s3[0].y, s3[0].z };
  Map<Eigen::Matrix<double, 9, 1>> b(cv.data());

  const auto& x = A.bdcSvd(ComputeFullU | ComputeFullV).solve(b);
  //if () return false;

  p = { x[0], x[1], x[2] };
  return true;
}

/**
 *  Intersect multiple 3D line segements (ceres).
 *
 *  \param [in] segs the list of six end points interlaeved by line segments, each is
 *  defined by two points.
 *  \param [out] p the intersection point.
 *  \param [in] numSegs number of line segments given by segs, must be 2+
 *  \param [out] summary the report of ceres solver.
 *  \param [in] numSegs number of line segments given by segs, must be 2+
 *  \return Return true if successful, false if parallel.
 */
bool XMultipleLines::IntersectCeres(const Point3d* segs, Point3d& p,
  Solver::Summary& summary, const int numSegs) const {
  assert(2 <= numSegs);
  std::vector<double> pb(numSegs, 0.5);
  const bool ret = IntersectGeometries<Point3d, NumericLineIntersectResidual>(
    segs, numSegs, pb, mOptions, summary);

  p = Point3d();
  for (int i = 0; i < numSegs; ++i) {
    const auto v = segs[i*2 + 1] - segs[i*2];
    p += segs[i * 2] + v * pb[i];
  }
  p /= double(numSegs);
  return true;
}

/**
 *  Intersect multiple 3D line segements (Eigen).
 *
 *  \param [in] segs the list of six end points interlaeved by line segments, each is
 *  defined by two points.
 *  \param [out] p the intersection point.
 *  \param [in] numSegs number of line segments given by segs, must be 2+
 *  \return Return true if successful, false if parallel.
 */
bool XMultipleLines::IntersectEigen(const Point3d* segs, 
  Point3d& p, const int numSegs) const {
  assert(2 <= numSegs);
  return true;
}
