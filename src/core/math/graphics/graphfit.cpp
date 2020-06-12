/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/math/graphfit.h"
#include "core/exceptions/exexcept.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <type_traits>

using namespace earthxpert;
using namespace earthxpert::math;
using namespace earthxpert::thirdparty;
using namespace::ceres;
using namespace::Eigen;

namespace earthxpert::math {
namespace detail {
/**
 *  \brief cost functors implements IResidual
 *
 *  \param PointTy type of point: Point2f, Point2d, Point3f, Point3d, Vector2f, Vector3d, etc
 *
 *  \param [in] point the observation data
 */
 // 2d equation: y - m * x - c; 3d equation: z - m * x - n * y -c
template <typename PointTy>
struct LinearResidual : IResidual {
  static constexpr int dim = sizeof(PointTy) / sizeof(PointTy::value_type);
  LinearResidual(const PointTy& point) : pt(point) {}

  template <typename Ty> bool operator()(const Ty* const pb, Ty* residual) const {
    if constexpr (2 == dim) {
      residual[0] = pt.y - pb[0] * pt.x - pb[1];
    }
    else if constexpr (3 == dim) {
      residual[0] = pt.z - pb[0] * pt.x - pb[1] * pt.y - pb[2];
    }
    else {
      return false;
    }
    return true;
  }

private:
  const PointTy pt;
};

// 2d equation: y - exp(m * x + c); 3d equation: z - exp(m * x + n * y + c)
template <typename PointTy>
struct ExponentialResidual : IResidual {
  static constexpr int dim = sizeof(PointTy) / sizeof(PointTy::value_type);
  ExponentialResidual(const PointTy& point) : pt(point) {}

  template <typename Ty> bool operator()(const Ty* const pb, Ty* residual) const {
    if constexpr (2 == dim) {
      residual[0] = pt.y - exp(pb[0] * pt.x + pb[1]);
    }
    else if constexpr (3 == dim) {
      residual[0] = pt.z - exp(pb[0] * pt.x + pb[1] * pt.y + pb[2]);
    }
    else {
      return false;
    }
    return true;
  }

private:
  const PointTy pt;
};

/**
 *  \brief fit a two-variable function to a sequence of 2D points, or
 *  a three-variable function to a sequence of 3D points,
 *  assuming equal weighting for point coordinates.
 *
 *  \param PointTy type of point: Point2f, Point2d, Point3f, Point3d, Vector2f, Vector3d
 *  \param ResidualPolicy defines the functor for residual calculation.
 *
 *  The 2d equation is defined by y = func(m, c). for example,
 *
 *    stright line: y = m * x + c
 *    exponential curve: y = exp(m * x + c)
 *
 *  where
 *
 *    y: output variable
 *    x: input variable
 *    m: the slope parameter
 *    c: the intercept parameter
 **
 *  The 3d equation is defined by y = func(m, n, c). for example,
 *
 *    flat plane: z = m * x + n * y + c
 *    exponential surface: z = exp(m * x + n * y + c)
 *
 *  where
 *
 *    z: output variable
 *    x, y: input variables
 *    m, n: the slope parameter
 *    c: the intercept parameter
 *
 *  \param [in] points a sequence, at least two, of 2D oe 3D points
 *  \param [in/out] pb the parameter block holding all unknown parameters
 *                  optionally with initial values.
 *  \param [in] options the options to control ceres solver.
 *  \param [out] summary the report of ceres solver.
 *
 *  \todo do not instantiate LossPolicy before resolving the construtor issue
 *  \todo estimate initial parameter values
 *  \todo weighting of observations
 *  \todo make CostFunction a template parameter
 */
template <typename PointTy, 
  template <typename PtTy = PointTy> class ResidualPolicy, 
  class LossPolicy = void>
void Fit2D_3D(const std::vector<PointTy>& points, std::vector<double>& pb, 
  const Solver::Options& options, Solver::Summary& summary, LossPolicy* lossFunc = nullptr) {
  //static_assert(std::is_same_v<Point2d, PointTy> || std::is_same_v<Point3d, PointTy>, "PointTy");
  static_assert(std::is_base_of_v<IResidual, ResidualPolicy<PointTy>>, "IResidual");
  static_assert(std::is_base_of_v<LossFunction, LossPolicy> || std::is_void_v<LossPolicy>, "LossFunction");
  assert(!points.empty());

  constexpr int dim = sizeof(PointTy) / sizeof(PointTy::value_type);
  if (pb.empty()) {
    pb.resize(dim, 0.0);
  }
  assert(dim == pb.size());

  LossFunction* lossF = (lossFunc) ? static_cast<LossFunction*>(lossFunc) : nullptr;
  
  Problem problem;
  for (auto& point : points) {
    auto costFunc = new AutoDiffCostFunction<ResidualPolicy<PointTy>, 1, dim>(
      new ResidualPolicy<PointTy>(point));

    problem.AddResidualBlock(costFunc, lossF, pb.data());
  }

  ceres::Solve(options, &problem, &summary);
}
} // detail
} // namespace earthxpert::math

GraphFit::GraphFit() {
  mOptions.max_num_iterations = 20;
  mOptions.linear_solver_type = ceres::DENSE_QR;
  mOptions.minimizer_progress_to_stdout = false;
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
void GraphFit::FitStraightLine(const std::vector<Point2d>& points, 
  std::vector<double>& pb, Solver::Summary& summary) {
  if (2 > points.size())
    throw std::invalid_argument("FitStraightLine: must have 2+ points");

  detail::Fit2D_3D<Point2d, detail::LinearResidual>(points, pb, mOptions, summary);
}

void GraphFit::FitStraightLineRobust(const std::vector<Point2d>& points, 
  std::vector<double>& pb, Solver::Summary& summary) {
  if (2 > points.size())
    throw std::invalid_argument("FitStraightLine: must have 2+ points");

  auto* lossFunc = new CauchyLoss(0.5);
  detail::Fit2D_3D<Point2d, detail::LinearResidual, CauchyLoss>(points, 
    pb, mOptions, summary, lossFunc);
}

/**
 *  \brief fit line by Eigen Solver
 */
void GraphFit::FitStraightLine(const std::vector<Point2d>& points, std::vector<double>& pb) {
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

  pb[0] = x[0];
  pb[1] = x[1];
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
void GraphFit::FitExponentialCurve(const std::vector<Point2d>& points, 
  std::vector<double>& pb, Solver::Summary& summary) {
  if (2 > points.size())
    throw std::invalid_argument("FitExponentialCurve: must have 2+ points");

  return detail::Fit2D_3D<Point2d, detail::ExponentialResidual>(points, 
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
void GraphFit::FitPlane(const std::vector<Point3d>& points,
  std::vector<double>& pb, Solver::Summary& summary) {
  if (3 > points.size())
    throw std::invalid_argument("FitPlane: must have 3+ points");

  detail::Fit2D_3D<Point3d, detail::LinearResidual>(points, pb, mOptions, summary);
}
