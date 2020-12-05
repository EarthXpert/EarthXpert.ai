/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */

struct FitCostFunction {
  typedef double Scalar;

  enum {
    NUM_RESIDUALS = 2,
    NUM_PARAMETERS = 3,
  };

  bool operator()(const double* parameters,
                  double* residuals, double* jacobian) const {
    //residuals[0] = x + 2*y + 4*z;
    //residuals[1] = y * z;

    if (jacobian) {
      jacobian[0 * 2 + 0] = 1;   // First column (x).
      jacobian[0 * 2 + 1] = 0;

      jacobian[1 * 2 + 0] = 2;   // Second column (y).
      //jacobian[1 * 2 + 1] = z;

      jacobian[2 * 2 + 0] = 4;   // Third column (z).
      //jacobian[2 * 2 + 1] = y;
    }
    return true;
  }

  int NumResiduals() const { return NUM_RESIDUALS; }
  int NumParameters() const { return NUM_PARAMETERS; }
};

using FitTinySolver = ceres::TinySolver<FitCostFunction>;

/**
 *  TestCase 1: 
 */
void TinySolver_(const int64_t& count) {
  FitCostFunction fitCostFunc;
  typename FitTinySolver::Parameters x;

  FitTinySolver tinySolver;
  tinySolver.Solve(fitCostFunc, &x);
}
