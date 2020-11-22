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
  std::string wgs84Wkt{"WGS84"};    // EPSG:4326
  OGRSpatialReference wgs84Srs(wgs84Wkt.c_str());
  
  std::string utmWkt{"+proj=utm +zone=17 +datum=WGS84 +units=m +no_defs"};
  OGRSpatialReference utmSrs(utmWkt.c_str());
  //utmSrs.SetProjCS("UTM 17 / WGS84");
  //utmSrs.SetWellKnownGeogCS("WGS84");
  //utmSrs.SetUTM(17);

  // WGS84 -> UTM
  OgrCT g2mCT(&wgs84Srs, &utmSrs);

  Point3d geoPt(Deg2Rad(-79.347015), Deg2Rad(43.651070), 0);  // Toronto
  Point3d mapPt;
  g2mCT.Transform(geoPt, mapPt);

  EqualD eqM(1.0e-3);  // [mm]
  ASSERT_TRUE(eqM(0.0, mapPt.x)) << "X difference > 1 mm";
  ASSERT_TRUE(eqM(0.0, mapPt.y)) << "Y difference > 1 mm";
  ASSERT_TRUE(eqM(0.0, mapPt.z)) << "Z difference > 1 mm";

  // UTM -> WGS84
  OgrCT m2gCT(&utmSrs, &wgs84Srs);

  Point3d geoPtN{ mapPt };
  m2gCT.Transform(geoPtN);

  EqualD eqA(1.0e-8);  // [mm]
  ASSERT_TRUE(eqA(geoPtN.x, geoPt.x)) << "Longitude difference > 1 mm";
  ASSERT_TRUE(eqA(geoPtN.y, geoPt.y)) << "Latitude difference > 1 mm";
  ASSERT_TRUE(eqM(geoPtN.z, geoPt.z)) << "Height difference > 1 mm";
}
