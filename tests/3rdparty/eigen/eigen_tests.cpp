#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/eigen/exeigen.h"

#include <Eigen/IterativeLinearSolvers> 	
#include <iostream>

using namespace::Eigen;
using SparseMatrixDR = SparseMatrix<double, RowMajor>;

/**
 *  Test case 1: A * x = b
 */
TEST(eigen, CGSolver) {
  constexpr int n = 10000;
  SparseMatrixDR A(n, n);
  VectorXd b(n);

  for (int i = 0; i < n; ++i) {
    A.insert(i, i) = double(i) * double(i + 1);
    b[i] = i;
  }

  // \todo fill A and b
  ConjugateGradient<SparseMatrixDR, Lower | Upper> cg;
  cg.compute(A);
  VectorXd x = cg.solve(b);

  ASSERT_EQ(0, cg.iterations()) << "converge in 1 step";
  ASSERT_NEAR(6.81325e-17, cg.error(), DBL_EPSILON) << "full rank";
  
  ASSERT_NEAR(0, x[0], DBL_EPSILON) << "x[0]";
  ASSERT_NEAR(0.0099009900990099011, x[100], DBL_EPSILON) << "x[100]";
  ASSERT_NEAR(0.000999000999000999, x[1000], DBL_EPSILON) << "x[1000]";
  ASSERT_NEAR(0.0001, x[9999], DBL_EPSILON) << "x[9999]";
}
