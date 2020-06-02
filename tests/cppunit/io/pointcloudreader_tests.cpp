/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rd_party/gtest/gtest_main.h"
#include "testconfig.h"

#include "io/pointcloud/pointcloudreader.h"
#include <array>
#include <omp.h>

using namespace earthxpert;
using fgt = FieldGroup;

/**
 *  \brief PointcloudReader test cases, zero offsets in .las
 *
 *  TestCase 1: read XYZ fields in .las
 */
TEST(PointcloudReader, ReadPointcloud_LAS_XYZ) {
  constexpr auto tol = 1.0e-6; // [um]
  constexpr size_t filePointCount{ 20680 };

  const std::string lasFile(TestConfig::DataPath("PointClouds/points_small.las"));
  PointcloudReader reader(lasFile, { fgt::kBITXYZ });

  ASSERT_EQ(filePointCount, reader.GetPointCount()) << "number of points";

  auto pc = reader.ReadPointcloud();
  ASSERT_EQ(filePointCount, pc->GetPointCount()) << "number of points";

  const auto fg = pc->GetFieldGroup();
  ASSERT_TRUE(fg.test(fgt::kPOSXYZ));
  ASSERT_FALSE(fg.test(fgt::kPOSCLA));

  const auto origin = pc->GetOrigin();
  ASSERT_NEAR(0, origin.x, tol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.y, tol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.z, tol) << "difference > 1 um";
}

/**
 *  \brief PointcloudReader test cases, non-zero offsets in .las
 *
 *  TestCase 2: read XYZC fields
 */
TEST(PointcloudReader, ReadPointcloud_LAS_XYZC) {
  constexpr auto tol = 1.0e-6; // [um]
  constexpr size_t filePointCount{ 478069 };

  const std::string lasFile(TestConfig::DataPath("PointClouds/guided_tour_c3d/DEM/DSM.las"));
  PointcloudReader reader(lasFile);

  ASSERT_EQ(filePointCount, reader.GetPointCount()) << "number of points";

  auto pc = reader.ReadPointcloud();
  ASSERT_EQ(filePointCount, pc->GetPointCount()) << "number of points";

  const auto fg = pc->GetFieldGroup();
  ASSERT_TRUE(fg.test(fgt::kPOSXYZ));
  ASSERT_TRUE(fg.test(fgt::kPOSCLA));

  const auto origin = pc->GetOrigin();
  ASSERT_NEAR(424996.503, origin.x, tol) << "difference > 1 um";
  ASSERT_NEAR(5132320.247, origin.y, tol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.z, tol) << "difference > 1 um";
}

/**
 *  \brief PointcloudReader test cases, zero offsets in .las
 *
 *  TestCase 3: read XYZ fields in .laz
 */
TEST(PointcloudReader, ReadPointcloud_LAZ_XYZ) {
  constexpr auto tol = 1.0e-6; // [um]
  constexpr size_t filePointCount{ 823560 };

  const std::string lasFile(TestConfig::DataPath("PointClouds/OMA_276_PC3.laz"));
  PointcloudReader reader(lasFile, {fgt::kBITXYZ});

  ASSERT_EQ(filePointCount, reader.GetPointCount()) << "number of points";

  auto pc = reader.ReadPointcloud();
  ASSERT_EQ(filePointCount, pc->GetPointCount()) << "number of points";

  const auto fg = pc->GetFieldGroup();
  ASSERT_TRUE(fg.test(fgt::kPOSXYZ));
  ASSERT_FALSE(fg.test(fgt::kPOSCLA));
  ASSERT_FALSE(fg.test(fgt::kPOSRGB));

  const auto origin = pc->GetOrigin();
  ASSERT_NEAR(0, origin.x, tol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.y, tol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.z, tol) << "difference > 1 um";

  const size_t i = filePointCount / 2;

  const auto xyz = pc->GetXYZ(i);
  ASSERT_NEAR(-11.8599997, xyz.x, tol) << "difference > 1 um";
  ASSERT_NEAR( 99.4300003, xyz.y, tol) << "difference > 1 um";
  ASSERT_NEAR( 11.9899998, xyz.z, tol) << "difference > 1 um";
}

/**
 *  \brief PointcloudReader test cases, zero offsets in .las
 *
 *  TestCase 4: read XYZC & RGB fields in .laz
 */
TEST(PointcloudReader, ReadPointcloud_LAZ_XYZCRGB) {
  constexpr auto tol = 1.0e-6; // [um]
  constexpr size_t filePointCount{ 823560 };

  const std::string lasFile(TestConfig::DataPath("PointClouds/OMA_276_PC3.laz"));
  PointcloudReader reader(lasFile);

  ASSERT_EQ(filePointCount, reader.GetPointCount()) << "number of points";

  auto pc = reader.ReadPointcloud();
  ASSERT_EQ(filePointCount, pc->GetPointCount()) << "number of points";

  const auto fg = pc->GetFieldGroup();
  ASSERT_TRUE(fg.test(fgt::kPOSXYZ));
  ASSERT_TRUE(fg.test(fgt::kPOSCLA));
  ASSERT_TRUE(fg.test(fgt::kPOSRGB));

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

/**
 *  \brief PointcloudReader test cases
 *
 *  TestCase 5: read chunks of points in .las
 */
TEST(PointcloudReader, ReadPointcloud_LAS_XYZC_CHUNKS) {
  constexpr auto tol = 1.0e-6; // [um]
  constexpr size_t filePointCount{ 20680 };
  const size_t chunkNumPts = filePointCount / 3;
  const std::array<size_t, 4> chunkNumPts4{
    chunkNumPts, chunkNumPts, chunkNumPts, filePointCount - chunkNumPts * 3 };
  const std::array<XYZF, 4> xyz4{ XYZF{539839.375f, 1385161.75f, 38.34f},
                                  XYZF{540413.8125f, 1384951.0f, 45.43f},
                                  XYZF{540077.6875f, 1384611.0f, 44.43f},
                                  XYZF{540987.25f, 1384298.125f, 44.05f} };

  // \remark the 4th chunk has only 1 point
  ASSERT_EQ(chunkNumPts4[3], 1);

  const std::string lasFile(TestConfig::DataPath("PointClouds/points_small.las"));
  PointcloudReader reader(lasFile, { fgt::kBITXC });

  ASSERT_EQ(filePointCount, reader.GetPointCount()) << "number of points";

  Pointcloud pc(chunkNumPts, { fgt::kBITXYZ });
  for (size_t i = 0, j = 0; i < filePointCount; i += chunkNumPts, ++j) {
    const auto readNumPts = reader.ReadPointcloud(&pc);
    ASSERT_EQ(chunkNumPts4[j], readNumPts);

    const auto xyz = pc.GetXYZ(0);
    ASSERT_NEAR(xyz4[j].x, xyz.x, tol) << "difference > 1 um";
    ASSERT_NEAR(xyz4[j].y, xyz.y, tol) << "difference > 1 um";
    ASSERT_NEAR(xyz4[j].z, xyz.z, tol) << "difference > 1 um";
  }
}

/**
 *  \brief PointcloudReader test cases OMP
 *
 *  TestCase 6: read chunks of points in .las
 */
TEST(PointcloudReader, ReadPointcloudOMP_LAS_XYZ_CHUNKS) {
  constexpr auto tol = 1.0e-6; // [um]
  constexpr size_t filePointCount{ 20680 };
  const size_t chunkNumPts = filePointCount / 100;

  const std::string lasFile(TestConfig::DataPath("PointClouds/points_small.las"));
  PointcloudReader reader(lasFile, { fgt::kBITXYZ });

  ASSERT_EQ(filePointCount, reader.GetPointCount()) << "number of points";

#pragma omp parallel for
  for (int64 i = 0; i < filePointCount - chunkNumPts; i += chunkNumPts) {
    auto pc = reader.ReadPointcloud(i, chunkNumPts);
  }

  const size_t lastNumPts = filePointCount - chunkNumPts * 100;
  auto pcLast = reader.ReadPointcloud(chunkNumPts*100, lastNumPts);
  ASSERT_EQ(pcLast->GetPointCount(), lastNumPts) << "#points of last chunk";
}
