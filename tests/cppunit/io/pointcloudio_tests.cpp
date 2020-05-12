#include "3rd_party/gtest/gtest_main.h"
#include "testconfig.h"

#include "io/pointcloud/pointcloudreader.h"
#include "io/pointcloud/pointcloudwriter.h"

using namespace watxtract;

TEST(pointswindow, CreatePoint) {
  double x = 100;
  double y = 200;
  double z = 300;
  uint16_t intensity = 1;
  uint8_t classification = 1;
  uint16_t r = 0;
  uint16_t g = 255;
  uint16_t b = 166;

  Point point(x, y, z, intensity, classification, r, g, b);
  
  ASSERT_EQ(point.x, x);
  ASSERT_EQ(point.y, y);
  ASSERT_EQ(point.z, z);
  ASSERT_EQ(point.intensity, intensity);
  ASSERT_EQ(point.classification, classification);
  ASSERT_EQ(point.r, r);
  ASSERT_EQ(point.g, g);
  ASSERT_EQ(point.b, b);
}
/*
TEST(pointcloudreader, ReadPoints) {
  const std::string lasFile(TestConfig::DataPath("PointClouds/guided_tour_c3d/DEM/DSM.las"));
  PointCloudReader pcReader(lasFile);

  auto pw = pcReader.ReadPoints();
  auto& points = pw->GetPointsData();
  EXPECT_EQ(pcReader.GetNumberPoints(), points.size());

  double maxX, minX, maxY, minY;
  pcReader.GetBoundary(maxX, minX, maxY, minY);
  ASSERT_TRUE(true);
  //ASSERT_TRUE(1.77531e+06 <= maxX && maxX <= 1.77533e+06);
  //ASSERT_TRUE(1.7732e+06 <= minX && minX <= 1.7734e+06);
  //ASSERT_TRUE(5.43491e+06 <= maxY && maxY <= 5.43493e+06);
  //ASSERT_TRUE(5.4328e+06 <= minY && minY <= 5.4330e+06);
}

TEST(pointcloudwriter, WritePoints) {
  const std::string lasFile(TestConfig::DataPath("PointClouds/guided_tour_c3d/DEM/DSM.las"));
  PointCloudReader pcReader(lasFile);
  auto pw = pcReader.ReadPoints();

  const std::string lasFileT(TestConfig::OutputPath("DSM_t.las"));

  //PointCloudWriter pcWriter(lasFileT, pcReader.GetHeader().get());
  //pcWriter.WritePoints(pw.get());
}
*/