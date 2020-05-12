/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rd_party/gtest/gtest_main.h"
#include "testconfig.h"

#include "io/pointcloud/pointcloudreader.h"

using namespace watxtract;

/**
 *  \brief PointcloudReader test cases
 *
 *  TestCase 1: XYZ fields
 */
TEST(PointcloudReader, XYZ) {
  const std::string lasFile(TestConfig::DataPath("PointClouds/points_small.las"));
  PointcloudReader reader(lasFile);

  ASSERT_EQ(20680, reader.GetPointCount());
}
