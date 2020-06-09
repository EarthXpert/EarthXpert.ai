/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"

#include <boost/filesystem.hpp>
#include <iostream>

namespace bfs = boost::filesystem;

TEST(boost, filesystem) {
  bfs::path filePath("D:/Workspace/Results/output.txt");

  ASSERT_FALSE(filePath.filename_is_dot());
  ASSERT_TRUE(filePath.has_extension());
}
