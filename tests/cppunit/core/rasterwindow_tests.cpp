/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"

#include "core/utils/rasterwindow.h"

using namespace earthxpert;

TEST(rasterwindow, hash) {
  RasterWindow rw;
  const size_t hv = std::hash<RasterWindow>{}(rw);
  ASSERT_EQ(15630149188884325585, hv);
}
