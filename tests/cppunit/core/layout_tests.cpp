/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"

#include "core/exceptions/exexcept.h"
#include "core/utils/layoututils.h"

using namespace earthxpert;

constexpr int kTile = 128;

TEST(layout, EstimateTileSize) {
  auto [tileNumCols, tileNumRows] = EstimateTileSize(1'000'000,
    2, 1000, 1000, kTile, eLK_SQUARE);
  ASSERT_EQ(512, tileNumCols);
  ASSERT_EQ(512, tileNumRows);

  std::tie(tileNumCols, tileNumRows) = EstimateTileSize(1'000'000,
    2, 1000, 1000, kTile, eLK_SQUARE, 100, 100);
  ASSERT_EQ(384, tileNumCols);
  ASSERT_EQ(384, tileNumRows);

  std::tie(tileNumCols, tileNumRows) = EstimateTileSize(1'000'000,
    2, 1000, 1000, kTile, eLK_SQUARE, 200, 200);
  ASSERT_EQ(256, tileNumCols);
  ASSERT_EQ(256, tileNumRows);

  ASSERT_THROW(EstimateTileSize(1'000'000,
    2, 1000, 1000, kTile, eLK_SQUARE, 1000, 1000), std::invalid_argument);
}
