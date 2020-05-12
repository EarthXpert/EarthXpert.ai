#include "3rd_party/gtest/gtest_main_gdal.h"

#include "gdal.h"
#include "ogr_spatialref.h"

#include <iostream>
#include <memory>

TEST(gdal, CreateWgs84SRS) {
  auto wgs84Geog = std::make_unique<OGRSpatialReference>("WGS84");

  ASSERT_TRUE(nullptr != wgs84Geog) << "returned nullptr";
}
