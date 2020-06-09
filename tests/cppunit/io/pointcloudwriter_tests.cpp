/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"

#include "io/pointcloud/pointcloudreader.h"
#include "io/pointcloud/pointcloudwriter.h"
#include <filesystem>

using namespace earthxpert;
using fgt = FieldGroup;
namespace fs = std::filesystem;

static std::unique_ptr<Pointcloud> ReadPointcloud(
  const std::string& pcFile, const fgt fieldGroup) {
  PointcloudReader reader(pcFile, fieldGroup);

  return reader.ReadPointcloud();
}

static std::unique_ptr<Pointcloud> ReadPointcloud(const std::string& pcFile) {
  return ReadPointcloud(pcFile, {});
}

/**
 *  \brief compare point cloud contents including # of points, XYZ, classification, RGB, etc.
 *
 *  \param [in] lasFile the original image
 *  \param [in] lasFileT the test image
 *  \return Returns true if the original and the test point clouds are exactly same.
 */
static bool IsPointcloudEqual(const std::string& lasFile, const std::string& lasFileT) {
  PointcloudReader reader(lasFile);
  auto las = reader.ReadPointcloud();

  PointcloudReader readerT(lasFileT);
  auto lasT = readerT.ReadPointcloud();

  // \todo

  return true;
}

/**
 *  \brief PointcloudWriter test cases, zero offsets in .las
 *
 *  TestCase 1: XYZ fields, use origin of Pointcloud object as offset
 */
TEST(pointcloudwriter, WritePointcloud_LAS_XYZ) {
  constexpr size_t filePointCount{ 20680 };

  const std::string lasFile(TestConfig::DataPath("PointClouds/points_small.las"));
  auto pc = ReadPointcloud(lasFile, { fgt::kBITXYZ });

  const auto fg = pc->GetFieldGroup();
  const auto origin = pc->GetOrigin();

  LASquantizer quantizer;
  quantizer.x_offset = origin.x;
  quantizer.y_offset = origin.y;
  quantizer.z_offset = origin.z;

  const std::string lasFileT(TestConfig::OutputPath("points_small_t.las"));
  PointcloudWriter writer(lasFileT, fg, quantizer);

  const auto pointCount = writer.WritePointcloud(pc.get());
  ASSERT_EQ(filePointCount, pointCount);
  ASSERT_TRUE(fs::exists(fs::path(lasFileT))) << lasFileT;
}

/**
 *  \brief PointcloudWriter test cases, zero offsets in .las
 *
 *  TestCase 2: XYZ fields, use any point of Pointcloud as offset, 0.001 as scale factor
 */
TEST(pointcloudwriter, WritePointcloud_LAS_XYZ2) {
  constexpr size_t filePointCount{ 20680 };

  const std::string lasFile(TestConfig::DataPath("PointClouds/points_small.las"));
  auto pc = ReadPointcloud(lasFile, { fgt::kBITXC });

  const auto fg = pc->GetFieldGroup();
  const auto point = pc->GetXYZD(0);  // first point

  LASquantizer quantizer;
  quantizer.x_scale_factor = 0.001;
  quantizer.y_scale_factor = 0.001;
  quantizer.z_scale_factor = 0.001;
  quantizer.x_offset = point.x;
  quantizer.y_offset = point.y;
  quantizer.z_offset = 0.0;

  const std::string lazFileT(TestConfig::OutputPath("points_small_t2.laz"));
  PointcloudWriter writer(lazFileT, fg, quantizer, "laz");

  const auto pointCount = writer.WritePointcloud(pc.get());
  ASSERT_EQ(filePointCount, pointCount);
  ASSERT_TRUE(fs::exists(fs::path(lazFileT))) << lazFileT;
}

/**
 *  \brief PointcloudWriter test cases
 *
 *  TestCase 6: write chunks of points in .las
 */
TEST(pointcloudwriter, WritePointcloud_LAS_XYZC_CHUNKS) {
  constexpr size_t filePointCount{ 20680 };
  const size_t chunkNumPts = filePointCount / 3;

  const std::string lasFile(TestConfig::DataPath("PointClouds/points_small.las"));
  PointcloudReader reader(lasFile, { fgt::kBITXC });

  const auto fg = reader.GetFieldGroup();
  auto lasReader = reader.GetLasReader();
  auto quantizer = static_cast<LASquantizer>(lasReader->header);

  const std::string lasFileT(TestConfig::OutputPath("points_small_t3.las"));
  PointcloudWriter writer(lasFileT, fg, quantizer, "las");

  Pointcloud pc(chunkNumPts, fg);
  for (size_t i = 0; i < filePointCount; i += chunkNumPts) {
    const auto readNumPts = reader.ReadPointcloud(i, &pc);
    const auto writeNumPts = writer.WritePointcloud(&pc, 0, readNumPts);
    ASSERT_EQ(readNumPts, writeNumPts);
  }

  ASSERT_TRUE(fs::exists(fs::path(lasFileT))) << lasFileT;
}

/**
 *  \brief PointcloudWriter test cases
 *
 *  TestCase 4: write XYZC fields, non-zero offsets in .las
 */
TEST(pointcloudwriter, WritePointcloud_LAS_XYZC) {
  constexpr size_t filePointCount{ 478069 };

  const std::string lasFile(TestConfig::DataPath("PointClouds/guided_tour_c3d/DEM/DSM.las"));
  auto pc = ReadPointcloud(lasFile);

  const auto fg = pc->GetFieldGroup();
  const auto origin = pc->GetOrigin();
  const auto point = pc->GetXYZD(filePointCount-1);  // last point

  LASquantizer quantizer;
  quantizer.x_scale_factor = 0.001;
  quantizer.y_scale_factor = 0.001;
  quantizer.z_scale_factor = 0.01;
  quantizer.x_offset = point.x;
  quantizer.y_offset = point.y;
  quantizer.z_offset = origin.z;

  const std::string lasFileT(TestConfig::OutputPath("DSM_t.las"));
  PointcloudWriter writer(lasFileT, fg, quantizer);

  const auto pointCount = writer.WritePointcloud(pc.get());
  ASSERT_EQ(filePointCount, pointCount);
  ASSERT_TRUE(fs::exists(fs::path(lasFileT))) << lasFileT;
}

/**
 *  \brief PointcloudWriter test cases
 *
 *  TestCase 5: write XYZ fields in .las
 */
TEST(pointcloudwriter, WritePointcloud_LAZ_XYZ) {
  constexpr size_t filePointCount{ 823560 };

  const std::string lasFile(TestConfig::DataPath("PointClouds/OMA_276_PC3.laz"));
  auto pc = ReadPointcloud(lasFile, { fgt::kBITXYZ });

  const auto fg = pc->GetFieldGroup();
  const auto origin = pc->GetOrigin();
  const auto point = pc->GetXYZD(filePointCount / 2);  // middle point

  LASquantizer quantizer;
  quantizer.x_scale_factor = 0.01;
  quantizer.y_scale_factor = 0.01;
  quantizer.z_scale_factor = 0.001;
  quantizer.x_offset = origin.x;
  quantizer.y_offset = origin.y;
  quantizer.z_offset = point.z;

  const std::string lasFileT(TestConfig::OutputPath("OMA_276_PC3_t.laz"));
  PointcloudWriter writer(lasFileT, fg, quantizer, "laz");

  const auto pointCount = writer.WritePointcloud(pc.get());
  ASSERT_EQ(filePointCount, pointCount);
  ASSERT_TRUE(fs::exists(fs::path(lasFileT))) << lasFileT;
}

/**
 *  \brief PointcloudWriter test cases
 *
 *  TestCase 6: write XYZC & RGB fields in .laz
 */
TEST(pointcloudwriter, WritePointcloud_LAZ_XYZCRGB) {
  constexpr size_t filePointCount{ 823560 };

  const std::string lasFile(TestConfig::DataPath("PointClouds/OMA_276_PC3.laz"));
  auto pc = ReadPointcloud(lasFile);

  const auto fg = pc->GetFieldGroup();
  const auto origin = pc->GetOrigin();
  const auto point = pc->GetXYZD(filePointCount / 2);  // middle point

  LASquantizer quantizer;
  quantizer.x_scale_factor = 0.001;
  quantizer.y_scale_factor = 0.001;
  quantizer.z_scale_factor = 0.01;
  quantizer.x_offset = point.x;
  quantizer.y_offset = point.y;
  quantizer.z_offset = origin.z;

  const std::string lasFileT(TestConfig::OutputPath("OMA_276_PC3_t2.las"));
  PointcloudWriter writer(lasFileT, fg, quantizer);

  const auto pointCount = writer.WritePointcloud(pc.get());
  ASSERT_EQ(filePointCount, pointCount);
  ASSERT_TRUE(fs::exists(fs::path(lasFileT))) << lasFileT;
}

/**
 *  \brief GDAL test cases
 *
 *  TestCase: compare output image files with original images
 */
TEST(pointcloudwriter, Equality) {
  // TestCase 1
  ASSERT_TRUE(IsPointcloudEqual(TestConfig::DataPath("PointClouds/points_small.las"),
    TestConfig::OutputPath("points_small_t.las"))) << "points_small_t.las";
  // TestCase 2
  ASSERT_TRUE(IsPointcloudEqual(TestConfig::DataPath("PointClouds/points_small.las"),
    TestConfig::OutputPath("points_small_t2.laz"))) << "points_small_t2.laz";
  // TestCase 3
  ASSERT_TRUE(IsPointcloudEqual(TestConfig::DataPath("PointClouds/points_small.las"),
    TestConfig::OutputPath("points_small_t3.las"))) << "points_small_t3.las";
  // TestCase 3
  ASSERT_TRUE(IsPointcloudEqual(TestConfig::DataPath("PointClouds/guided_tour_c3d/DEM/DSM.las"),
    TestConfig::OutputPath("DSM_t.las"))) << "DSM_t.las";
  // TestCase 4
  ASSERT_TRUE(IsPointcloudEqual(TestConfig::DataPath("PointClouds/OMA_276_PC3.laz"),
    TestConfig::OutputPath("OMA_276_PC3_t.laz"))) << "OMA_276_PC3_t.laz";
  // TestCase 5
  ASSERT_TRUE(IsPointcloudEqual(TestConfig::DataPath("PointClouds/OMA_276_PC3.laz"),
    TestConfig::OutputPath("OMA_276_PC3_t2.las"))) << "OMA_276_PC3_t2.las";
}
