/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main_gdal.h"
#include "testconfig.h"

#include "core/math/constants.h"
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
  OGRSpatialReference wgs84(wkt.c_str());
}

TEST(coordinatesystem, OGR_UTM) {
  std::string wkt{ "+proj=utm +zone=17 +datum=WGS84 +units=m +no_defs" };
  OGRSpatialReference utm(wkt.c_str());
  //utm.SetProjCS("UTM 17 / WGS84");
  //utm.SetWellKnownGeogCS("WGS84");
  //utm.SetUTM(17);
}
