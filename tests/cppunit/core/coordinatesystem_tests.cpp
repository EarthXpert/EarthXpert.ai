/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main_gdal.h"
#include "3rdparty/utils/constants.h"
#include "testconfig.h"

#include "core/spatialref/srsfactory.h"
#include "core/utils/typehelper.h"

using namespace earthxpert;

/**
 *  Test case: CCS
 */

/**
 *  Test case: OGR
 */
TEST(coordinatesystem, OGR_WGS84) {
  std::string wkt{"WGS84"};    // EPSG:4326
  OGRSpatialRef wgs84(wkt.c_str());
}

TEST(coordinatesystem, OGR_UTM) {
  std::string wkt{ R"wkt(+proj=utm +zone=17 +datum=WGS84 +units=m +no_defs)wkt" };
  OGRSpatialRef utm;
  utm.importFromProj4(wkt.c_str());
}
