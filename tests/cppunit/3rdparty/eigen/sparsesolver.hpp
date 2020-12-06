/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <Eigen/IterativeLinearSolvers> 	
#include <iostream>
#include <random>

using namespace earthxpert;
using namespace::Eigen;
using SpMatDR = SparseMatrix<double, RowMajor>;

static constexpr auto kScale = 1.0 / static_cast<double>(std::mt19937::max());
static constexpr int64_t kN10k = 10000;
static constexpr int64_t kN30M = 30'000'000;

/**
 *  Test case 1: A * x = b
 */
TEST(eigen, SparseSolver) {
  SpMatDR A(kN10k, kN10k);
  VectorXd b(kN10k);

  // fill A and b
  for (int i = 0; i < kN10k; ++i) {
    A.insert(i, i) = double(i + 1);
    b[i] = i;
  }

  //std::cout << "Condition number: " << ConditionNumber(A) << std::endl;

  // CG
  ConjugateGradient<SpMatDR, Lower | Upper> cg;
  cg.compute(A);
  VectorXd x = cg.solve(b);

  ASSERT_EQ(1, cg.iterations()) << "converge in 1 step";
  ASSERT_NEAR(0, cg.error(), DBL_EPSILON) << "full rank";
  ASSERT_NEAR(0,      x[0],    DBL_EPSILON) << "x[0]";
  ASSERT_NEAR(0.999,  x[999],  DBL_EPSILON) << "x[999]";
  ASSERT_NEAR(0.9999, x[9999], DBL_EPSILON) << "x[9999]";

  // LSCG
  LeastSquaresConjugateGradient<SpMatDR> lscg;
  lscg.compute(A);
  x = lscg.solve(b);

  ASSERT_EQ(0, lscg.iterations()) << "converge in 1 step";
  ASSERT_NEAR(6.81325e-17, lscg.error(), DBL_EPSILON) << "full rank";
  ASSERT_NEAR(0,      x[0],    DBL_EPSILON) << "x[0]";
  ASSERT_NEAR(0.999,  x[999],  DBL_EPSILON) << "x[999]";
  ASSERT_NEAR(0.9999, x[9999], DBL_EPSILON) << "x[9999]";
}

/**
 *  Test case 2: A * x = b
 *
 *  \brief add 3x3 subblocks to form block diagonal with band width of 3
 */
TEST(eigen, LSCG_Diagonal) {
  std::mt19937 rng;
  SpMatDR A(kN30M, kN30M);
  VectorXd b(kN30M);

  std::vector<int> reserveSizes(kN30M, 3);
  A.reserve(reserveSizes);

  // fill A and b
  // first row
  A.insert(0, 0) = rng() * kScale;
  A.insert(0, 1) = rng() * kScale - 0.5;
  A.insert(0, 2) = rng() * kScale - 0.5;
  b[0] = (rng() * kScale - 0.5) * 0.1;  // [-0.05, 0.05]

  for (int i = 1; i < kN30M - 1; ++i) {
    A.insert(i, i) = double(i) + rng() * kScale;
    b[i] = double(i) + (rng() * kScale - 0.5) * 0.1;  // [-0.05, 0.05]

    // add two elements [-0.5, 0.5] to each row
    A.insert(i, i - 1) = rng() * kScale - 0.5;
    A.insert(i, i + 1) = rng() * kScale - 0.5;
  }
  // last row
  A.insert(kN30M - 1, kN30M - 3) = rng() * kScale - 0.5;
  A.insert(kN30M - 1, kN30M - 2) = rng() * kScale - 0.5;
  A.insert(kN30M - 1, kN30M - 1) = double(kN30M - 1) + rng() * kScale;
  b[kN30M - 1] = double(kN30M - 1) + (rng() * kScale - 0.5) * 0.1;  // [-0.05, 0.05]

  //std::cout << "Condition number: " << ConditionNumber(A) << std::endl;

  LeastSquaresConjugateGradient<SpMatDR> lscg;
  lscg.compute(A);
  VectorXd x = lscg.solve(b);

  ASSERT_EQ(4, lscg.iterations()) << "converge";
  ASSERT_NEAR(0, lscg.error(), DBL_EPSILON) << "full rank";
  ASSERT_NEAR(0.090088967211201298,  x[0], DBL_EPSILON) << "x[0]";
  ASSERT_NEAR(0.99999995778632444, x[kN30M/2], DBL_EPSILON) << "x[kN30M/2]";
  ASSERT_NEAR(1.0000000072469144, x[kN30M-1], DBL_EPSILON) << "x[kN30M-1]";

  double err = (A * x - b).norm() / b.norm();
  std::cout << "relative error: " << err << std::endl;
}

/**
 *  Test case 3: A * x = b
 *
 *  \brief add 3 fill-ins randomly on each row
 */
TEST(eigen, LSCG_NonDiagonal) {
  std::mt19937 rng;
  SpMatDR A(kN30M, kN30M);
  VectorXd b(kN30M);

  std::vector<int> reserveSizes(kN30M, 3);
  A.reserve(reserveSizes);

  // fill A and b
  for (int i = 0; i < kN30M; ++i) {
    A.insert(i, i) = double(i) + rng() * kScale;  // [0, 1]
    b[i] = double(i) + (rng() * kScale - 0.5) * 0.1;  // [-0.05, 0.05]

    // add two elements [-0.5, 0.5] to each row
    A.insert(i, rng() % kN30M) += rng() * kScale - 0.5;
    A.insert(i, rng() % kN30M) += rng() * kScale - 0.5;
  }

  //std::cout << "Condition number: " << ConditionNumber(A) << std::endl;

  LeastSquaresConjugateGradient<SpMatDR> lscg;
  lscg.compute(A);
  VectorXd x = lscg.solve(b);

  //ASSERT_EQ(6, lscg.iterations()) << "converge";
  //ASSERT_NEAR(0, lscg.error(), DBL_EPSILON) << "full rank";
  ASSERT_NEAR(-0.084974989099441012, x[0], DBL_EPSILON) << "x[0]";
  ASSERT_NEAR(0.99999991487069606, x[kN30M/2], DBL_EPSILON) << "x[kN30M/2]";
  ASSERT_NEAR(0.99999999529972705, x[kN30M-1], DBL_EPSILON) << "x[kN30M-1]";

  double err = (A * x - b).norm() / b.norm();
  std::cout << "relative error: " << err << std::endl;
}
