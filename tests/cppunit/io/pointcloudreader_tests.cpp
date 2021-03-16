/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"

#include "core/utils/spatiallayout.h"
#include "io/pointcloud/pointcloudreader.h"
#include <boost/format.hpp>
#include <array>
#include <iostream>
#include <omp.h>

using namespace earthxpert;
using fgt = FieldGroup;

constexpr double kTol = 1.0e-6; // [um]

/**
 *  \brief PointcloudReader test cases, zero offsets in .las
 *
 *  TestCase 1: read XYZ fields in .las
 */
TEST(pointcloudreader, ReadPointcloud_LAS_XYZ) {
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
  ASSERT_NEAR(0, origin.x, kTol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.y, kTol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.z, kTol) << "difference > 1 um";

  const auto bbox = pc->GetBufTile();
  ASSERT_NEAR(539836, bbox.minX, kTol) << "difference > 1 um";
  ASSERT_NEAR(1384288.75, bbox.minY, kTol) << "difference > 1 um";
  ASSERT_NEAR(1205, bbox.width, kTol) << "difference > 1 um";
  ASSERT_NEAR(873, bbox.height, kTol) << "difference > 1 um";
}

/**
 *  \brief PointcloudReader test cases, non-zero offsets in .las
 *
 *  TestCase 2: read XYZC fields
 */
TEST(pointcloudreader, ReadPointcloud_LAS_XYZC) {
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
  ASSERT_NEAR(424996.503, origin.x, kTol) << "difference > 1 um";
  ASSERT_NEAR(5132320.247, origin.y, kTol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.z, kTol) << "difference > 1 um";

  const auto bbox = pc->GetBufTile();
  ASSERT_NEAR(424854.93700573733, bbox.minX, kTol) << "difference > 1 um";
  ASSERT_NEAR(5132303.1900007939, bbox.minY, kTol) << "difference > 1 um";
  ASSERT_NEAR(292.94400024414062, bbox.width, kTol) << "difference > 1 um";
  ASSERT_NEAR(17.056999206542969, bbox.height, kTol) << "difference > 1 um";
}

/**
 *  \brief PointcloudReader test cases, zero offsets in .las
 *
 *  TestCase 3: read XYZ fields in .laz
 */
TEST(pointcloudreader, ReadPointcloud_LAZ_XYZ) {
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
  ASSERT_NEAR(0, origin.x, kTol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.y, kTol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.z, kTol) << "difference > 1 um";

  const size_t i = filePointCount / 2;

  const auto xyz = pc->GetField<XYZF>(i);
  ASSERT_NEAR(-11.8599997, xyz.x, kTol) << "difference > 1 um";
  ASSERT_NEAR( 99.4300003, xyz.y, kTol) << "difference > 1 um";
  ASSERT_NEAR( 11.9899998, xyz.z, kTol) << "difference > 1 um";

  const auto bbox = pc->GetBufTile();
  ASSERT_NEAR(-244.16000366210938, bbox.minX, kTol) << "difference > 1 um";
  ASSERT_NEAR(-320.20001220703125, bbox.minY, kTol) << "difference > 1 um";
  ASSERT_NEAR(512, bbox.width, kTol) << "difference > 1 um";
  ASSERT_NEAR(0.850006103515625, bbox.height, kTol) << "difference > 1 um";
}

/**
 *  \brief PointcloudReader test cases, zero offsets in .las
 *
 *  TestCase 4: read XYZC & RGB fields in .laz
 */
TEST(pointcloudreader, ReadPointcloud_LAZ_XYZCRGB) {
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
  ASSERT_NEAR(0, origin.x, kTol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.y, kTol) << "difference > 1 um";
  ASSERT_NEAR(0, origin.z, kTol) << "difference > 1 um";

  const size_t i = 823560 / 2;

  const auto xyz = pc->GetField<XYZF>(i);
  ASSERT_NEAR(-11.8599997, xyz.x, kTol) << "difference > 1 um";
  ASSERT_NEAR( 99.4300003, xyz.y, kTol) << "difference > 1 um";
  ASSERT_NEAR( 11.9899998, xyz.z, kTol) << "difference > 1 um";

  const auto c = pc->GetField<Classification>(i);
  ASSERT_EQ(0, c.c);

  const auto rgb = pc->GetField<RGB16>(i);
  ASSERT_EQ(47616, rgb.r);
  ASSERT_EQ(65280, rgb.g);
  ASSERT_EQ(    0, rgb.b);
}

/**
 *  \brief PointcloudReader test cases
 *
 *  TestCase 5: read chunks of points in .las
 */
TEST(pointcloudreader, ReadPointcloud_LAS_XYZC_CHUNKS) {
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
    const auto readNumPts = reader.ReadPointcloud(i, &pc);
    ASSERT_EQ(chunkNumPts4[j], readNumPts);

    const auto xyz = pc.GetField<XYZF>(0);
    ASSERT_NEAR(xyz4[j].x, xyz.x, kTol) << "difference > 1 um";
    ASSERT_NEAR(xyz4[j].y, xyz.y, kTol) << "difference > 1 um";
    ASSERT_NEAR(xyz4[j].z, xyz.z, kTol) << "difference > 1 um";
  }
}

/**
 *  \brief PointcloudReader test cases OMP
 *
 *  TestCase 6: read chunks of points in .las
 */
TEST(pointcloudreader, ReadPointcloudOMP_LAS_XYZ_CHUNKS) {
  constexpr size_t filePointCount{ 20680 };
  const size_t chunkNumPts{ 21 };
  const size_t numChunks{ filePointCount / chunkNumPts };

  const std::string lasFile(TestConfig::DataPath("PointClouds/points_small.las"));
  PointcloudReader reader(lasFile, { fgt::kBITXYZ });

  ASSERT_EQ(filePointCount, reader.GetPointCount()) << "number of points";

#pragma omp parallel
  {
    Pointcloud pc(chunkNumPts, { fgt::kBITXYZ });

    #pragma omp for
      for (int64_t i = 0; i < filePointCount - chunkNumPts; i += chunkNumPts) {
        reader.ReadPointcloud(i, &pc);
        //ASSERT_EQ(pc.GetPointCount(), chunkNumPts) << "#points of last chunk";  // fatal error C1001: Internal compiler error
      }
  }

  auto pcLast = reader.ReadPointcloud(chunkNumPts * numChunks, 16);
  ASSERT_EQ(pcLast->GetPointCount(), 16) << "#points of last chunk";
}

/**
 *  \brief PointcloudReader test cases
 *
 *  TestCase 7: read tiles of points in .las
 *  \todo examine extent of reader
 */
TEST(pointcloudreader, ReadPointcloud_LAS_XYZC_TILES) {
  constexpr size_t filePointCount{ 20680 };
  const std::array<size_t, 4> tileNumPts4{
    6680, 4257, 6390, 3249 };
  const std::array<XYZF, 4> xyz4{ XYZF{539852.6875f, 1384727.875f, 41.35f},
                                  XYZF{540465.5f, 1384716.5f, 45.89f},
                                  XYZF{539838.8125f, 1385144.0f, 38.21f},
                                  XYZF{540464.625f, 1384732.75f, 45.82f} };

  const std::string lasFile(TestConfig::DataPath("PointClouds/points_small.las"));
  PointcloudReader reader(lasFile, { fgt::kBITXC });

  ASSERT_EQ(filePointCount, reader.GetPointCount()) << "number of points";

  auto header = reader.GetLasHeader();
  const auto width = header->max_x - header->min_x;
  const auto height = header->max_y - header->min_y;
  const auto tileWidth = width * 0.5 + 1.0e-6; // increase a little bit to ensure only two horizontal tiles
  const auto tileHeight = height * 0.5 + 1.0e-6;

  SpatialTile<double> range{header->min_x, header->min_y, width, height};
  RegionLayoutD layout(range, tileWidth, tileHeight);
  auto& tiles = layout.GetSpatialTiles();

  ASSERT_EQ(4, tiles.size()) << "number of tiles";

  int j{};
  for (int64_t i = 0; i < tiles.size(); ++i) {
    auto& tile = tiles[i];
    auto pc = reader.ReadPointcloud(tile);

    ASSERT_EQ(tileNumPts4[i], pc->GetPointCount()) << "#points";

    const auto xyz = pc->GetField<XYZF>(0);
    ASSERT_NEAR(xyz4[i].x, xyz.x, kTol) << "difference > 1 um";
    ASSERT_NEAR(xyz4[i].y, xyz.y, kTol) << "difference > 1 um";
    ASSERT_NEAR(xyz4[i].z, xyz.z, kTol) << "difference > 1 um";

    const auto tile2 = pc->GetBufTile();
    if (!(tile >= tile2)) {
      ++j;
    }
  }
  ASSERT_EQ(4, j) << "#tiles";
}

/**
 *  \brief PointcloudReader test cases OMP
 *
 *  TestCase 8: read tile of points in .las
 */
TEST(pointcloudreader, ReadPointcloudOMP_LAS_XYZ_TILES) {
  constexpr size_t filePointCount{ 20680 };

  const std::string lasFile(TestConfig::DataPath("PointClouds/points_small.las"));
  PointcloudReader reader(lasFile, { fgt::kBITXYZ });

  ASSERT_EQ(filePointCount, reader.GetPointCount()) << "number of points";

  auto header = reader.GetLasHeader();
  const auto width = header->max_x - header->min_x;
  const auto height = header->max_y - header->min_y;
  const auto tileWidth = width / 25.0 + 1.0e-6; // increase a little bit to ensure only two horizontal tiles
  const auto tileHeight = height / 25.0 + 1.0e-6;

  SpatialTile<double> range{ header->min_x, header->min_y, width, height };
  RegionLayoutD layout(range, tileWidth, tileHeight);
  auto& tiles = layout.GetSpatialTiles();

  ASSERT_EQ(625, tiles.size()) << "number of tiles";

  int j{};
#pragma omp parallel for reduction(+ : j)   // \todo fatal error C1001: Internal compiler error
  for (int64_t i = 0; i < tiles.size(); ++i) {
    auto& tile = tiles[i];
    auto pc = reader.ReadPointcloud(tile);

    const auto tile2 = pc->GetBufTile();
    if (!(tile >= tile2)) {
      //std::cerr << boost::format("tile extent: %1%\n") % i;
      ++j;
    }
  }
  ASSERT_EQ(277, j) << "#tiles";
}
