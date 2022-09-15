/* -----------------------------------------------------------------------------
 *  Copyright (c) 2022.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "utils/lsr2utm.h"
#include <iostream>

using namespace earthxpert;
using namespace ceres;
using namespace ::Eigen;

/**
 *  \brief Construct rasterizer object
 */
LsrToUtm::LsrToUtm() {
}

void LsrToUtm::Initialize() {
  mOptions.max_num_iterations = 20;
  mOptions.num_threads = 1; // kNumPhysicalCores
  //mOptions.function_tolerance = 1e-3;   // [pixel]
  //mOptions.gradient_tolerance = 1e-7;
  //mOptions.parameter_tolerance = 1e-8;  // [degree, degree, meter]
  mOptions.linear_solver_type = DENSE_QR; // DENSE_NORMAL_CHOLESKY
  //mOptions.preconditioner_type = JACOBI;  // SUBSET
  mOptions.minimizer_progress_to_stdout = false;
  //mOptions.update_state_every_iteration = true;
  //mOptions.callbacks = {};
}

/**
 *  \brief
 */
void LsrToUtm::Solve() {
}

/**
 *  \brief
 */
void LsrToUtm::Convert() {
}
