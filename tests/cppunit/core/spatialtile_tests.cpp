/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"

#include "core/utils/spatialtile.h"

using namespace earthxpert;

TEST(spatialtile, hash) {
  SpatialTile<float> st;
  const size_t hv = std::hash<SpatialTile<float>>{}(st);
  ASSERT_EQ(15630149188884325585, hv);
}
