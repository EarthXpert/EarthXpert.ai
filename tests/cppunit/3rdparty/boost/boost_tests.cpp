/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/boost/exboost.h"

#include "algo.hpp"
#include "geometry.hpp"
#include "hana.hpp"
#include "math.hpp"
#include "multiprecision.hpp"

#include <boost/pool/object_pool.hpp>

TEST(boost, filesystem) {
  bfs::path filePath("D:/Workspace/Results/output.txt");

  ASSERT_FALSE(filePath.filename_is_dot());
  ASSERT_TRUE(filePath.has_extension());
}
