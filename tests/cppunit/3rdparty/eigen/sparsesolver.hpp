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

  std::cout << "Condition number: " << ConditionNumber(A) << std::endl;

  // CG
  ConjugateGradient<SpMatDR, Lower | Upper> cg;
  cg.compute(A);
  VectorXd x = cg.solve(b);

  ASSERT_EQ(1, cg.iterations()) << "converge in 1 step";
  ASSERT_NEAR(0, cg.error(), DBL_EPSILON) << "full rank";

  //ASSERT_NEAR(0,      x[0],    DBL_EPSILON) << "x[0]";
  //ASSERT_NEAR(0.001,  x[999],  DBL_EPSILON) << "x[999]";
  //ASSERT_NEAR(0.0001, x[9999], DBL_EPSILON) << "x[9999]";

  // LSCG
  LeastSquaresConjugateGradient<SpMatDR> lscg;
  lscg.compute(A);
  x = lscg.solve(b);

  ASSERT_EQ(0, lscg.iterations()) << "converge in 1 step";
  ASSERT_NEAR(6.81325e-17, lscg.error(), DBL_EPSILON) << "full rank";

  //ASSERT_NEAR(0,      x[0],    DBL_EPSILON) << "x[0]";
  //ASSERT_NEAR(0.001,  x[999],  DBL_EPSILON) << "x[999]";
  //ASSERT_NEAR(0.0001, x[9999], DBL_EPSILON) << "x[9999]";
}

/**
 *  Test case 2: A * x = b
 *
 *  \brief add 3x3 subblocks to form block diagonal with band width of 3
 */
TEST(eigen, SparseSolver_Diagonal) {
  std::mt19937 rng;
  SpMatDR A(kN30M, kN30M);
  VectorXd b(kN30M);

  // fill A and b
  for (int i = 0; i < kN30M; ++i) {
    A.insert(i, i) = double(i) + rng() * kScale;
    b[i] = i + (rng() * kScale - 0.5) * 0.1;  // [-0.05, 0.05]

    // add two elements [-0.5, 0.5] to each row
    if (0 < i && i < kN30M - 1) {
      A.insert(i, i - 1) = rng() * kScale - 0.5;
      A.insert(i, i + 1) = rng() * kScale - 0.5;
    }
  }

  // first row
  A.insert(0, 1) = rng() * kScale - 0.5;
  A.insert(0, 2) = rng() * kScale - 0.5;

  // last row
  A.insert(kN30M - 1, kN30M - 3) = rng() * kScale - 0.5;
  A.insert(kN30M - 1, kN30M - 2) = rng() * kScale - 0.5;

  //std::cout << "Condition number: " << ConditionNumber(A) << std::endl;

  LeastSquaresConjugateGradient<SpMatDR> lscg;
  lscg.compute(A);
  VectorXd x = lscg.solve(b);

  ASSERT_EQ(4, lscg.iterations()) << "converge";
  ASSERT_NEAR(0, lscg.error(), DBL_EPSILON) << "full rank";
  //ASSERT_NEAR(0.0016456911197363748,  x[0], DBL_EPSILON) << "x[0]";
  //ASSERT_NEAR(6.6666662302292853e-08, x[kN30M/2], DBL_EPSILON) << "x[kN30M/2]";
  //ASSERT_NEAR(3.3333333346269354e-08, x[kN30M-1], DBL_EPSILON) << "x[kN30M-1]";
}

/**
 *  Test case 3: A * x = b
 *
 *  \brief add 3 fill-ins randomly on each row
 */
TEST(eigen, SparseSolver_NonDiagonal) {
  std::mt19937 rng;
  SpMatDR A(kN30M, kN30M);
  VectorXd b(kN30M);

  // fill A and b
  for (int i = 0; i < kN30M; ++i) {
    A.insert(i, i) = double(i) + rng() * kScale;  // [0, 1]
    b[i] = i + (rng() * kScale - 0.5) * 0.1;  // [-0.05, 0.05]

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
  //ASSERT_NEAR(0.0031678088890426804, x[0], DBL_EPSILON) << "x[0]";
  //ASSERT_NEAR(6.6666662302292853e-08, x[kN30M/2], DBL_EPSILON) << "x[kN30M/2]";
  //ASSERT_NEAR(3.3333333346269354e-08, x[kN30M-1], DBL_EPSILON) << "x[kN30M-1]";
}
