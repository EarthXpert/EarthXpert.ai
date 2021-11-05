/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/xtensor/exxtensor.h"

#include <xtensor/xarray.hpp>
#include <iostream>

using namespace xt;

/**
 *  TestCase 0: xarray
 */
TEST(xtensor, xarray) {
  xarray<int, layout_type::row_major> a = { {1, 2, 3, 4},
                                           {5, 6, 7, 8} };
  xarray<int, layout_type::column_major> b = { {1, 2, 3, 4},
                                              {5, 6, 7, 8} };
  ASSERT_EQ(a, b);

  ASSERT_EQ(2, a.shape(0));
  ASSERT_EQ(4, a.shape(1));

  ASSERT_EQ(2, b.shape(0));
  ASSERT_EQ(4, b.shape(1));

  xarray<int, layout_type::column_major> ra = a;
  ASSERT_EQ(b, ra);

  xarray<int, layout_type::row_major> rb = b;
  ASSERT_EQ(a, rb);
}

/**
 *  TestCase 0: xtensor
 */
TEST(xtensor, xtensor) {
}
