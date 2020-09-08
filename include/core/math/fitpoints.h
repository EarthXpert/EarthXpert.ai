/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ceres/exceres.h"
#include "3rdparty/eigen/exeigen.h"
#include "3rdparty/opencv/exopencv.h"

namespace earthxpert {
using thirdparty::IResidual;

namespace math {
// residual base for fitting points
template <typename PointTy> struct FitResidual : IResidual {
  FitResidual(const PointTy* const point) : pt(point) {}
  virtual ~FitResidual() = default;

protected:
  const PointTy* const pt; // observation
};

/**
 *  \brief cost functors implements IResidual
 *
 *  \param PointTy type of point: Point2f, Point2d, Point3f, Point3d, Vector2f, Vector3d, etc
 */
 // 2d line equation: y - m * x - c; 3d plane equation: z - m * x - n * y -c
template <typename PointTy> struct LinearFitResidual final : FitResidual<PointTy> {
  static constexpr int kDim = sizeof(PointTy) / sizeof(PointTy::value_type);
  static constexpr int kNumUnknowns = kDim;
  /**
   *  \param [in] point the observation data.
   */
  LinearFitResidual(const PointTy* const point) : FitResidual<PointTy>(point) {}
  ~LinearFitResidual() = default;

  /**
   *  \param [in] pb the two parameters m & c.
   *  \param [out] residual discrepency between the measured dimension and
   *  its estimated value calculated using the equation.
   */
  template <typename Ty = double> bool operator()(const Ty* const pb, Ty* residual) const {
    //static_assert(std::is_same_v<double, Ty>, "LinearFitResidual: operator()");
    static_assert(sizeof(Ty) >= sizeof(PointTy::value_type), "Parameter precision must be not lowert than observation's");

    if constexpr(2 == kDim) {
      residual[0] = pt->y - pb[0] * pt->x - pb[1];
    }
    else if constexpr(3 == kDim) {
      residual[0] = pt->z - pb[0] * pt->x - pb[1] * pt->y - pb[2];
    }
    else {
      return false;
    }
    return true;
  }
};

// 3d line equation: p(x, y, z) = p0(x0, y0, z0) + t * v(a, b, c), where v = p1 - p0

// 2d curve equation: y - exp(m * x + c); 3d surface equation: z - exp(m * x + n * y + c)
template <typename PointTy> struct ExponentialFitResidual final : FitResidual<PointTy> {
  static constexpr int kDim = sizeof(PointTy) / sizeof(PointTy::value_type);
  static constexpr int kNumUnknowns = kDim;
  /**
   *  \param [in] point the observation data.
   */
  ExponentialFitResidual(const PointTy* const point) : FitResidual<PointTy>(point) {}
  ~ExponentialFitResidual() = default;

  /**
   *  \param [in] pb the two parameters m & c.
   *  \param [out] residual discrepency between the measured dimension and
   *  its estimated value calculated using the equation.
   */
  template <typename Ty = double> bool operator()(const Ty* const pb, Ty* residual) const {
    if constexpr(2 == kDim) {
      residual[0] = pt->y - exp(pb[0] * pt->x + pb[1]);
    }
    else if constexpr(3 == kDim) {
      residual[0] = pt->z - exp(pb[0] * pt->x + pb[1] * pt->y + pb[2]);
    }
    else {
      return false;
    }
    return true;
  }
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
 *  \param [in] points a sequence, at least two, of N-Dimensional points
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
  template <typename PtTy = PointTy> class ResidualPolicy, class LossPolicy = void>
bool FitPoints(const std::vector<PointTy>& points, std::vector<double>& pb,
  const Solver::Options& options, Solver::Summary& summary, LossPolicy* lossFunc = nullptr) {
  using CostFunctor = ResidualPolicy<PointTy>;
  //static_assert(std::is_same_v<Point2d, PointTy> || std::is_same_v<Point3d, PointTy>, "PointTy");
  static_assert(std::is_base_of_v<IResidual, CostFunctor>, "IResidual");
  static_assert(std::is_base_of_v<LossFunction, LossPolicy> || std::is_void_v<LossPolicy>, "LossFunction");
  assert(!points.empty());

  if (pb.empty()) {
    pb.resize(CostFunctor::kDim, 0.0);
  }
  assert(CostFunctor::kDim == pb.size());

  Problem::Options popt;
  //popt.cost_function_ownership = DO_NOT_TAKE_OWNERSHIP;
  popt.loss_function_ownership = DO_NOT_TAKE_OWNERSHIP;
  Problem problem(popt);

  LossFunction* lossF = (lossFunc) ? static_cast<LossFunction*>(lossFunc) : nullptr;

  for (auto& point : points) {
    auto costFunc = new AutoDiffCostFunction<CostFunctor, 1, CostFunctor::kDim>(
      new CostFunctor(&point));

    problem.AddResidualBlock(costFunc, lossF, pb.data());
  }

  ceres::Solve(options, &problem, &summary);
  return summary.IsSolutionUsable();
}
}
} // namespace earthxpert::math
