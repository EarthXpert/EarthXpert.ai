/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <boost/multiprecision/cpp_complex.hpp>
#include <boost/multiprecision/eigen.hpp>
#include <Eigen/Dense>

#include <iostream>

namespace bmp = boost::multiprecision;
using namespace Eigen;

using ComplexD = bmp::cpp_complex_double;
using ComplexQ = bmp::cpp_complex_quad;

/**
 *  \tparam Ty number type
 */
template<typename Ty> void DenseSolver() {
  // fill A and b
  Matrix<Ty, 2, 2> A;
  A << Ty(2, 3), Ty(-1, -2), Ty(-1, -4), Ty(3, 6);
  //std::cout << "A:\n" << A << std::endl;

  Matrix<Ty, 2, 2> b;
  b << 1, 2, 3, 1;
  //std::cout << "rhs b:\n" << b << std::endl;

  // Solve for x:
  Matrix<Ty, 2, 2> x = A.fullPivHouseholderQr().solve(b);
  //std::cout << "solution:\n" << x << std::endl;

  // Compute the error using the norms of Ax - b and b:
  Ty::value_type relativeError = (A * x - b).norm() / b.norm();
  std::cout << "relative error: " << relativeError << std::endl;
}

/**
 *  Test case 1: solve Ax = b for x
 */
TEST(eigen, DenseSolverD) {
  DenseSolver<ComplexD>();
}

TEST(eigen, DenseSolverQ) {
  DenseSolver<ComplexQ>();
}
