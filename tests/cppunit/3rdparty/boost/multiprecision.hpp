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

using ComplexQuad = bmp::cpp_complex_quad;

/**
 *  Test case 1: solve Ax = b for x
 */
TEST(boost, mp_eigen) {
  // fill A and b
  Matrix<ComplexQuad, 2, 2> A;
  A << ComplexQuad(2, 3), ComplexQuad(-1, -2), ComplexQuad(-1, -4), ComplexQuad(3, 6);
  //std::cout << "A:\n" << A << std::endl;
  
  Matrix<ComplexQuad, 2, 2> b;
  b << 1, 2, 3, 1;
  //std::cout << "rhs b:\n" << b << std::endl;

  // Solve for x:
  Matrix<ComplexQuad, 2, 2> x = A.fullPivHouseholderQr().solve(b);
  //std::cout << "solution:\n" << x << std::endl;

  // Compute the error in the solution by using the norms of Ax - b and b:
  ComplexQuad::value_type relativeError = (A * x - b).norm() / b.norm();
  std::cout << "relative error: " << relativeError << std::endl;

  //ASSERT_NEAR(2.63132e-34, relativeError, std::numeric_limits<ComplexQuad::value_type>::epsilon()) << "relative error";
}
