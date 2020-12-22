/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/xtensor/exxtensor.h"

#include <xtensor/xarray.hpp>

using namespace xt;

 /**
  *  TestCase 0: 
  */
TEST(xtensor, DUMMY) {
  xarray<int, layout_type::row_major> a = { {1, 2, 3, 4},
                                           {5, 6, 7, 8} };
  xarray<int, layout_type::column_major> b = { {1, 2, 3, 4},
                                              {5, 6, 7, 8} };

  xarray<int, layout_type::column_major> ra = a;
  ASSERT_EQ(b, ra);

  xarray<int, layout_type::row_major> rb = b;
  ASSERT_EQ(a, rb);
}
