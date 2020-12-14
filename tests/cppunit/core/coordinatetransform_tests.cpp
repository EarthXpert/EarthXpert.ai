/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main_gdal.h"
#include "3rdparty/utils/constants.h"
#include "testconfig.h"

#include "core/spatialref/ctfactory.h"
#include "core/spatialref/srsfactory.h"
#include "core/utils/typehelper.h"

using namespace earthxpert;

/**
 *  Test case: CCS
 */

/**
 *  Test case: OGR
 */
TEST(coordinatetransform, OGR_WGS84_UTM) {
  OGRSpatialRef wgs84Srs;
  wgs84Srs.importFromEPSG(4326);  // WGS84
  
  std::string utmWkt{ R"wkt(+proj=utm +zone=17 +datum=WGS84 +units=m +no_defs)wkt"};
  OGRSpatialRef utmSrs;
  utmSrs.importFromProj4(utmWkt.c_str());
  //utmSrs.importFromEPSG(32617);

  // WGS84 -> UTM
  OgrCt g2mCt(&wgs84Srs, &utmSrs);

  Point3d geoPt(-79.347015, 43.651070, 0);  // Toronto
  Point3d mapPt;
  g2mCt.Transform(geoPt, mapPt);

  EqualD eqM(1.0e-3);  // [mm]
  ASSERT_TRUE(eqM(633301.7104163748, mapPt.x)) << "X difference > 1 mm";
  ASSERT_TRUE(eqM(4834446.627614007, mapPt.y)) << "Y difference > 1 mm";
  ASSERT_TRUE(eqM(0.0, mapPt.z)) << "Z difference > 1 mm";

  // UTM -> WGS84
  OgrCt m2gCT(&utmSrs, &wgs84Srs);

  Point3d geoPtN{ mapPt };
  m2gCT.Transform(geoPtN);

  EqualD eqA(1.0e-8);  // [mm]
  ASSERT_TRUE(eqA(geoPtN.x, geoPt.x)) << "Longitude difference > 1 mm";
  ASSERT_TRUE(eqA(geoPtN.y, geoPt.y)) << "Latitude difference > 1 mm";
  ASSERT_TRUE(eqM(geoPtN.z, geoPt.z)) << "Height difference > 1 mm";
}
