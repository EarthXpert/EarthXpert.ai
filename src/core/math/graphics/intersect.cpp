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
  //mOptions.function_tolerance = ;
  //mOptions.gradient_tolerance = ;
  //mOptions.parameter_tolerance = ;
  mOptions.linear_solver_type = DENSE_QR; // DENSE_NORMAL_CHOLESKY
  //mOptions.preconditioner_type = JACOBI;  // SUBSET
  mOptions.minimizer_progress_to_stdout = false;
  //mOptions.update_state_every_iteration = true;
  //mOptions.callbacks = {};
}

/**
 *  Intersect two 3D line segements.
 *
 *  \param [in] segs the list of four end points interlaeved by line segments,
 *  each is defined by two points.
 *  \param [out] p the intersection point.
 *  \param [out] summary the report of ceres solver.
 *  \return Return true if successful, false if parallel.
 */
bool XTwoLines::Intersect(const Point3d* segs, Point3d& p, 
  Solver::Summary& summary, const int) const {
  std::vector<double> pb(2, 0.5);
  const bool ret = IntersectGeometries<Point3d, LineIntersectResidual>(
    segs, 2, pb, mOptions, summary);
  return ret;
}

/**
 *  Intersect two 3D line segements.
 *
 *  \param [in] s1 the first line segment defined by two end points
 *  \param [in] s2 the second line segment defined by two end points
 *  \return Return true if successful, false if parallel.
 */
bool XTwoLines::Intersect(const Point3d* s1, const Point3d* s2, Point3d& p) const {
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
 *  Intersect three 3D line segements.
 *
 *  \param [in] segs the list of six end points interlaeved by line segments, 
 *  each is defined by two points.
 *  \param [out] p the intersection point.
 *  \param [out] summary the report of ceres solver.
 *  \return Return true if successful, false if parallel.
 */
bool XThreeLines::Intersect(const Point3d* segs, 
  Point3d& p, Solver::Summary& summary, const int) const {
  std::vector<double> pb(3, 0.5);
  const bool ret = IntersectGeometries<Point3d, LineIntersectResidual>(
    segs, 3, pb, mOptions, summary);
  return ret;
}

/**
 *  Intersect three 3D line segements.
 *
 *  \param [in] s1 the first line segment defined by two end points
 *  \param [in] s2 the second line segment defined by two end points
 *  \param [in] s3 the second line segment defined by two end points
 *  \return Return true if successful, false if parallel.
 */
bool XThreeLines::Intersect(const Point3d* s1, const Point3d* s2, 
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
