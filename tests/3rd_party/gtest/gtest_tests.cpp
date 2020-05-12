/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rd_party/gtest/gtest_main.h"

TEST(gtest, dummy) {
  ASSERT_TRUE(true) << "assert(true)";
  ASSERT_FALSE(false);

  EXPECT_TRUE(true) << "assert(true)";
  EXPECT_FALSE(false);
}
