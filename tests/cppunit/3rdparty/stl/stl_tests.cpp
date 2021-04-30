/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/stl/bitvector.h"

#include "algo.hpp"

TEST(stl, BitVector) {
  BitVector bv;

  bv.add(true).add(true).add(false); // [1, 1, 0]
  bv.add(false); // [1, 1, 0, 0]
  bv.add(true); // [1, 1, 0, 0, 1]

  ASSERT_TRUE(bv.any());
  ASSERT_FALSE(bv.all());
  ASSERT_TRUE(bv.none());
}
