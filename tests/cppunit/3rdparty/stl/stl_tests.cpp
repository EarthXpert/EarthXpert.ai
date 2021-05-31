/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/stl/bitvector.h"

#include "algo.hpp"
#include <iostream>
#include <string>

using namespace earthxpert;

void CreateFiles(const std::string& path, int numFiles);

TEST(stl, BitVector) {
  BitVector bv;

  bv.add(true).add(true).add(false); // [1, 1, 0]
  bv.add(false); // [1, 1, 0, 0]
  bv.add(true); // [1, 1, 0, 0, 1]

  ASSERT_TRUE(bv.any());
  ASSERT_FALSE(bv.all());
  ASSERT_TRUE(bv.none());
}

// Too Many Open Files
TEST(stl, OpenFiles) {
  const std::string path = "D:\\Workspace\\Results\\temp\\test\\";
  CreateFiles(path, 5000);
}

void CreateFiles(const std::string& path, int numFiles) {

}
