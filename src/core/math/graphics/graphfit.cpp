/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/math/graphfit.h"
#include "core/math/fitpoints.h"
#include "core/exceptions/exexcept.h"
#include "core/parallel/exomp.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <type_traits>

using namespace earthxpert;
using namespace earthxpert::math;
using namespace earthxpert::thirdparty;
using namespace::ceres;
using namespace::Eigen;

GraphFit::GraphFit() {
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

GraphFit::GraphFit(const Solver::Options& options) : mOptions(options) {
}

/**
 *  \brief fit a parameterized stright line to a sequence of 2D points.
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
 *  \param [in/out] pb the parameter block holding all unknown parameters
 *                  optionally with initial values.
 */
bool GraphFit::FitStraightLine(const std::vector<Point2d>& points, 
  std::vector<double>& pb, Solver::Summary& summary) {
  if (2 > points.size())
    throw std::invalid_argument("FitStraightLine: must have 2+ points");

  return FitPoints<Point2d, LinearFitResidual>(points, pb, mOptions, summary);
}

bool GraphFit::FitStraightLineRobust(const std::vector<Point2d>& points, 
  std::vector<double>& pb, Solver::Summary& summary) {
  if (2 > points.size())
    throw std::invalid_argument("FitStraightLine: must have 2+ points");

  auto* lossFunc = new CauchyLoss(0.5);
  return FitPoints<Point2d, LinearFitResidual, CauchyLoss>(points,
    pb, mOptions, summary, lossFunc);
}

/**
 *  \breif fit a parameterized stright line to a sequence of 3D points.
 *  The line equation is defined by
 *
 *    p = p0 + v * t
 *
 *  where
 *
 *    p: the observed 3D point
 *    p0: the anchor point, can be initialized by the average position or a selected observation
 *    v: the direction vector
 *    t: the parametric ratio
 *
 *  \param [in] points a sequence, at least two, of 3D points
 *  \param [in/out] pb the parameter block holding all unknown parameters
 *                  optionally with initial values.
 *  \param [out] summary the report of ceres solver.
 */
bool FitStraightLine(const std::vector<Point3d>& points,
  std::vector<double>& pb, Solver::Summary& summary) {
  if (2 > points.size())
    throw std::invalid_argument("FitStraightLine: must have 2+ points");

  return false;
}

/**
 *  \brief fit line by Eigen Solver
 */
bool GraphFit::FitStraightLine(const std::vector<Point2d>& points, std::vector<double>& pb) {
  const int n = int(points.size());
  if (2 > n)
    throw std::invalid_argument("FitStraightLine: must have 2+ points");

  if (pb.empty()) {
    pb.resize(2, 0.0);
  }

  MatrixXd A(n, 2);
  VectorXd b(n);
  for (int i = 0; i < n; ++i) {
    A(i, 0) = points[i].x;
    A(i, 1) = 1.0;
    b[i] = points[i].y;
  }

  //const auto& x = A.colPivHouseholderQr().solve(b);
  const auto& x = A.bdcSvd(ComputeThinU | ComputeThinV).solve(b);
  // if () return false;

  pb[0] = x[0];
  pb[1] = x[1];
  return true;
}

/**
 *  \breif fit a parameterized exponential curve to a sequence of 2D points.
 *  The line equation is defined by
 *
 *    y = exp(m * x + c)
 *
 *  where
 *
 *    y: output variable
 *    x: input variable
 *    m: the slope parameter
 *    c: the intercept parameter
 *
 *  \param [in] points a sequence, at least two, of 2D points
 *  \param [in/out] pb the parameter block holding all unknown parameters
 *                  optionally with initial values.
 */
bool GraphFit::FitExponentialCurve(const std::vector<Point2d>& points, 
  std::vector<double>& pb, Solver::Summary& summary) {
  if (2 > points.size())
    throw std::invalid_argument("FitExponentialCurve: must have 2+ points");

  return FitPoints<Point2d, ExponentialFitResidual>(points,
    pb, mOptions, summary);
}

/**
 *  \breif fit a parameterized plane to a sequence of 3D points.
 *  The plane equation is defined by
 *
 *    z = m * x + n * y + c
 *
 *  where
 *
 *    z: output variable
 *    x,y: input variables
 *    m,n: the slope parameters
 *    c: the intercept parameter
 *
 *  \param [in] points a sequence, at least three non-colinear, of 3D points
 *  \param [in/out] pb the parameter block holding all unknown parameters
 *                  optionally with initial values.
 *  \param [out] summary the report of ceres solver.
 */
bool GraphFit::FitPlane(const std::vector<Point3d>& points,
  std::vector<double>& pb, Solver::Summary& summary) {
  if (3 > points.size())
    throw std::invalid_argument("FitPlane: must have 3+ points");

  return FitPoints<Point3d, LinearFitResidual>(points, pb, mOptions, summary);
}
