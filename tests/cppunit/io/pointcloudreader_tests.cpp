/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rd_party/gtest/gtest_main.h"
#include "testconfig.h"

#include "io/pointcloud/pointcloudreader.h"

using namespace earthxpert;

/**
 *  \brief PointcloudReader test cases
 *
 *  TestCase 1: read XYZ fields in .las
 */
TEST(PointcloudReader, ReadPointcloud_LAS_XYZ) {
  constexpr auto tol = 1.0e-6; // [um]

  const std::string lasFile(TestConfig::DataPath("PointClouds/points_small.las"));
  PointcloudReader reader(lasFile, FieldGroup{1});

  ASSERT_EQ(20680, reader.GetPointCount()) << "number of points";

  auto pc = reader.ReadPointcloud();
  ASSERT_EQ(20680, pc->GetPointCount()) << "number of points";

  const auto fg = pc->GetFieldGroup();
  ASSERT_TRUE(fg.test(FieldGroup::kPOSXYZ));
  ASSERT_FALSE(fg.test(FieldGroup::kPOSCLA));

  const auto origin = pc->GetOrigin();
  ASSERT_NEAR(0, origin.x, tol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.y, tol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.z, tol) << "difference > 1 um";
}

/**
 *  \brief PointcloudReader test cases
 *
 *  TestCase 2: read XYZC fields, non-zero offsets in .las
 */
TEST(PointcloudReader, ReadPointcloud_LAS_XYZC) {
  constexpr auto tol = 1.0e-6; // [um]

  const std::string lasFile(TestConfig::DataPath("PointClouds/guided_tour_c3d/DEM/DSM.las"));
  PointcloudReader reader(lasFile);

  ASSERT_EQ(478069, reader.GetPointCount()) << "number of points";

  auto pc = reader.ReadPointcloud();
  ASSERT_EQ(478069, pc->GetPointCount()) << "number of points";

  const auto fg = pc->GetFieldGroup();
  ASSERT_TRUE(fg.test(FieldGroup::kPOSXYZ));
  ASSERT_TRUE(fg.test(FieldGroup::kPOSCLA));

  const auto origin = pc->GetOrigin();
  ASSERT_NEAR(424996.503, origin.x, tol) << "difference > 1 um";
  ASSERT_NEAR(5132320.247, origin.y, tol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.z, tol) << "difference > 1 um";
}

/**
 *  \brief PointcloudReader test cases
 *
 *  TestCase 3: read XYZ fields in .laz
 */
TEST(PointcloudReader, ReadPointcloud_LAZ_XYZC) {
  constexpr auto tol = 1.0e-6; // [um]

  const std::string lasFile(TestConfig::DataPath("PointClouds/OMA_276_PC3.laz"));
  PointcloudReader reader(lasFile, FieldGroup{1});

  ASSERT_EQ(823560, reader.GetPointCount()) << "number of points";

  auto pc = reader.ReadPointcloud();
  ASSERT_EQ(823560, pc->GetPointCount()) << "number of points";

  const auto fg = pc->GetFieldGroup();
  ASSERT_TRUE(fg.test(FieldGroup::kPOSXYZ));
  ASSERT_FALSE(fg.test(FieldGroup::kPOSCLA));
  ASSERT_FALSE(fg.test(FieldGroup::kPOSRGB));

  const auto origin = pc->GetOrigin();
  ASSERT_NEAR(0, origin.x, tol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.y, tol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.z, tol) << "difference > 1 um";

  const size_t i = 823560 / 2;

  const auto xyz = pc->GetXYZ(i);
  ASSERT_NEAR(-11.8599997, xyz.x, tol) << "difference > 1 um";
  ASSERT_NEAR( 99.4300003, xyz.y, tol) << "difference > 1 um";
  ASSERT_NEAR( 11.9899998, xyz.z, tol) << "difference > 1 um";
}

/**
 *  \brief PointcloudReader test cases
 *
 *  TestCase 4: read XYZC & RGB fields in .laz
 */
TEST(PointcloudReader, ReadPointcloud_LAZ_XYZCRGB) {
  constexpr auto tol = 1.0e-6; // [um]

  const std::string lasFile(TestConfig::DataPath("PointClouds/OMA_276_PC3.laz"));
  PointcloudReader reader(lasFile);

  ASSERT_EQ(823560, reader.GetPointCount()) << "number of points";

  auto pc = reader.ReadPointcloud();
  ASSERT_EQ(823560, pc->GetPointCount()) << "number of points";

  const auto fg = pc->GetFieldGroup();
  ASSERT_TRUE(fg.test(FieldGroup::kPOSXYZ));
  ASSERT_TRUE(fg.test(FieldGroup::kPOSCLA));
  ASSERT_TRUE(fg.test(FieldGroup::kPOSRGB));

  const auto origin = pc->GetOrigin();
  ASSERT_NEAR(0, origin.x, tol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.y, tol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.z, tol) << "difference > 1 um";

  const size_t i = 823560 / 2;

  const auto xyz = pc->GetXYZ(i);
  ASSERT_NEAR(-11.8599997, xyz.x, tol) << "difference > 1 um";
  ASSERT_NEAR( 99.4300003, xyz.y, tol) << "difference > 1 um";
  ASSERT_NEAR( 11.9899998, xyz.z, tol) << "difference > 1 um";

  const auto c = pc->GetCla(i);
  ASSERT_EQ(0, c.c);

  const auto rgb = pc->GetRGB(i);
  ASSERT_EQ(47616, rgb.r);
  ASSERT_EQ(65280, rgb.g);
  ASSERT_EQ(    0, rgb.b);
}
