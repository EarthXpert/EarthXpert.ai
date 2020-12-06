/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include <Eigen/IterativeLinearSolvers>

#include <random>

using namespace::Eigen;
using SpMatDR = SparseMatrix<double, RowMajor>;

static constexpr auto kScale = 1.0 / static_cast<double>(std::mt19937::max());
static constexpr int64_t kN20M = 20'000'000;

void LSCG_NonDiagonal(const int& count) {
  std::mt19937 rng;
  SpMatDR A(kN20M, kN20M);
  VectorXd b(kN20M);

  std::vector<int> reserveSizes(kN20M, count);
  A.reserve(reserveSizes);

  // fill A and b
  for (int i = 0; i < kN20M; ++i) {
    A.insert(i, i) = double(i) + rng() * kScale;  // [0, 1]
    b[i] = double(i) + (rng() * kScale - 0.5) * 0.1;  // [-0.05, 0.05]

    // add count-1 elements [-0.5, 0.5] to each row
    for (int j =1; j < count; ++j)
      A.insert(i, rng() % kN20M) += rng() * kScale - 0.5;
  }

  LeastSquaresConjugateGradient<SpMatDR> lscg;
  lscg.compute(A);
  VectorXd x = lscg.solve(b);

  std::cout << "absolute error: " << lscg.error() << std::endl;

  double err = (A * x - b).norm() / b.norm();
  std::cout << "relative error: " << err << std::endl;
}

static const std::vector<int> args_cg{ 3, 15, 30 };

SLTBENCH_FUNCTION_WITH_ARGS(LSCG_NonDiagonal, args_cg);
