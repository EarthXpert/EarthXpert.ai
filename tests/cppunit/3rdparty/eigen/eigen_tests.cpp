/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/eigen/exeigen.h"

//#include "densesolver_bmp.hpp"
#include "densesolver_dyn.hpp"
#include "sparsesolver.hpp"

#include <iostream>

TEST(eigen, alias) {
  Eigen::MatrixXf A(3, 2), B(2, 2), C(2, 2);
  A << 2, 0, 0, 3, 1, 1;
  B << 2, 0, 0, -2;
  C << 2, 0, 0, -2;

  B = (A * B).cwiseAbs();
  C = (A * C).eval().cwiseAbs();
  ASSERT_EQ(B, C);
}
