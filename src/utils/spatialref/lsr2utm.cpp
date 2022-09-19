/* -----------------------------------------------------------------------------
 *  Copyright (c) 2022.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "utils/lsr2utm.h"
#include <cmath>
#include <iostream>
#include <memory>

using namespace earthxpert;
using namespace ceres;
using namespace ::Eigen;

/**
 *  \brief Construct rasterizer object
 */
LsrToUtm::LsrToUtm(const Point3d& origin) {
  mCT = std::make_unique<LsrToUtmCT>(origin);
  Initialize();
}

void LsrToUtm::Initialize() {
  mOptions.max_num_iterations = 100;
  mOptions.num_threads = 1; // kNumPhysicalCores
  //mOptions.function_tolerance = 1e-3;   // [meter]
  //mOptions.gradient_tolerance = 1e-7;
  //mOptions.parameter_tolerance = 1e-8;  // [radian, meter]
  mOptions.linear_solver_type = DENSE_QR; // DENSE_NORMAL_CHOLESKY
  //mOptions.preconditioner_type = JACOBI;  // SUBSET
  mOptions.minimizer_progress_to_stdout = false;
  //mOptions.update_state_every_iteration = true;
  //mOptions.callbacks = {};
}

/**
 *  \brief
 */
bool LsrToUtm::Solve(const std::vector<Point3d>& lsrs, 
  const std::vector<Point2d>& utms, double* pb) {
  assert(pb);
  Problem::Options popt;
  popt.loss_function_ownership = DO_NOT_TAKE_OWNERSHIP;
  Problem problem(popt);

  auto lossF = std::make_unique<CauchyLoss>(0.5);

  for (int i = 0; i < lsrs.size(); ++i) {
    auto costFunc = new NumericDiffCostFunction<NumericLsr2UtmResidual, ceres::CENTRAL, 2, 7>(
      new NumericLsr2UtmResidual(lsrs.data() + i, utms.data() + i, mCT.get()));

    problem.AddResidualBlock(costFunc, lossF.get(), pb);
  }


  Solver::Summary summary;
  ceres::Solve(mOptions, &problem, &summary);
  std::cout << summary.BriefReport() << std::endl; 

  return summary.IsSolutionUsable();
}

/**
 *  \brief
 */
std::vector<Point2d> LsrToUtm::Convert(const std::vector<Point3d>& lsrs, const double* pb) const {
  std::vector<Point2d> utms(lsrs.size());

  for (int i = 0; i < lsrs.size(); ++i) {
    mCT->Convert(lsrs.data() + i, pb, utms[i]);
  }
  return utms;
}

void LsrToUtm::SetParameterBounds(Problem& problem, double* pb) {
  // bounds of parameters
  problem.SetParameterLowerBound(pb, 0, 0.95);
  problem.SetParameterUpperBound(pb, 0, 1.05);
  problem.SetParameterLowerBound(pb, 1, -0.1);
  problem.SetParameterUpperBound(pb, 1, 0.1);
  problem.SetParameterLowerBound(pb, 2, -0.1);
  problem.SetParameterUpperBound(pb, 2, 0.1);
  problem.SetParameterLowerBound(pb, 3, -0.1);
  problem.SetParameterUpperBound(pb, 3, 0.1);
  problem.SetParameterLowerBound(pb, 4, -3000);
  problem.SetParameterUpperBound(pb, 4, 3000);
  problem.SetParameterLowerBound(pb, 5, -3000);
  problem.SetParameterUpperBound(pb, 5, 3000);
  problem.SetParameterLowerBound(pb, 6, -3000);
  problem.SetParameterUpperBound(pb, 6, 3000);
}
