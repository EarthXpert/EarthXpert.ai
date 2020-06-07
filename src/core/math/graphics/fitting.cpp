/* ----------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/math/fitting.h"
#include "3rd_party/ceres/exceres.h"
#include "core/exceptions/exexcept.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <type_traits>

using namespace earthxpert::thirdparty;
using namespace::ceres;

namespace earthxpert::math {
namespace detail {
  /**
   *  \brief cost functors implements IResidual
   */
  // equation: y - m * x - c
  struct LinearResidual : IResidual {
    LinearResidual(double x, double y)
      : x(x), y(y) {}

    template <typename Ty> bool operator()(const Ty* const m,
      const Ty* const c, Ty* residual) const {
      residual[0] = y - m[0] * x - c[0];
      return true;
    }

  private:
    const double x;
    const double y;
  };

  // equation: y - exp(m * x + c)
  struct ExponentialResidual : IResidual {
    ExponentialResidual(double x, double y)
      : x(x), y(y) {}

    template <typename Ty> bool operator()(const Ty* const m,
      const Ty* const c, Ty* residual) const {
      residual[0] = y - exp(m[0] * x + c[0]);
      return true;
    }

  private:
    const double x;
    const double y;
  };

  /**
   *  \breif fit a two-variable function to a sequence of 2D points,
   *  assuming equal weighting for point coordinates.
   *
   *  \param ResidualPolicy defines the functor for residual calculation.
   *
   *  The equation is defined by y = func(m, c). for example,
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
   *
   *  \param [in] points a sequence, at least two, of 2D points
   *  \return Return parameters m and c
   *
   *  \todo do not instantiate LossPolicy before resolving the construtor issue
   */
  template <typename ResidualPolicy, typename LossPolicy = void>
  std::pair<double, double> FitCurve2D(const std::vector<Point2d>& points, 
    LossPolicy* lossFunc = nullptr) {
    static_assert(std::is_base_of_v<IResidual, ResidualPolicy>, "IResidual");
    static_assert(std::is_base_of_v<LossFunction, LossPolicy> || std::is_void_v<LossPolicy>, "LossFunction");
    assert(!points.empty());
    double m{};
    double c{};
    
    LossFunction* lossF = (lossFunc) ? static_cast<LossFunction*>(lossFunc) : nullptr;

    Problem problem;
    for (size_t i = 0; i < points.size(); ++i) {
      auto costFunc = new AutoDiffCostFunction<ResidualPolicy, 1, 1, 1>(
        new ResidualPolicy(points[i].x, points[i].y));
      problem.AddResidualBlock(costFunc, lossF, &m, &c);
    }

    Solver::Options options;
    options.max_num_iterations = 5;
    options.linear_solver_type = ceres::DENSE_QR;
    //options.minimizer_progress_to_stdout = true;

    Solver::Summary summary;
    Solve(options, &problem, &summary);
    //std::cout << summary.BriefReport() << "\n";
    //std::cout << "Initial m: " << 0.0 << " c: " << 0.0 << "\n";
    //std::cout << "Final   m: " << m << " c: " << c << "\n";
    return { m, c };
  }
}

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
std::pair<double, double> FitStraightLine(const std::vector<Point2d>& points) {
  if (2 > points.size())
    throw std::invalid_argument("FitStraightLine: must have 2+ points");

  return detail::FitCurve2D<detail::LinearResidual>(points);
}
} // namespace earthxpert::math
