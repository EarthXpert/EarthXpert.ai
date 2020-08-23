/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main_gdal.h"
#include "testconfig.h"

#include "core/math/constants.h"
#include "core/spatialref/gdalct.h"
#include "core/utils/typehelper.h"

using namespace earthxpert;

TEST(coordinatetransform, WGS84_UTM) {
  std::string srcWkt{"WGS84"};    // EPSG:4326
  OGRSpatialReference srcSrs(srcWkt.c_str());
  
  std::string tgtWkt{"+proj=utm +zone=17 +datum=WGS84 +units=m +no_defs"};
  OGRSpatialReference tgtSrs(tgtWkt.c_str());
  //tgtSrs.SetProjCS("UTM 17 / WGS84");
  //tgtSrs.SetWellKnownGeogCS("WGS84");
  //tgtSrs.SetUTM(17);

  GdalCT ctG2M(&srcSrs, &tgtSrs);
  GdalCT ctM2G(&tgtSrs, &srcSrs);

  Point3d ptGeo(Deg2Rad(-79.347015), Deg2Rad(43.651070), 0);  // Toronto
  
  Point3d ptMap{ ptGeo };
  ctG2M.Transform(ptMap);

  EqualD EqM(1.0e-3);  // [mm]
  ASSERT_TRUE(EqM(0.0, ptMap.x)) << "X difference > 1 mm";
  ASSERT_TRUE(EqM(0.0, ptMap.y)) << "Y difference > 1 mm";
  ASSERT_TRUE(EqM(0.0, ptMap.z)) << "Z difference > 1 mm";

  Point3d ptGeoN{ ptMap };
  ctM2G.Transform(ptGeoN);

  EqualD EqA(1.0e-8);  // [mm]
  ASSERT_TRUE(EqA(ptGeoN.x, ptGeo.x)) << "Longitude difference > 1 mm";
  ASSERT_TRUE(EqA(ptGeoN.y, ptGeo.y)) << "Latitude difference > 1 mm";
  ASSERT_TRUE(EqM(ptGeoN.z, ptGeo.z)) << "Height difference > 1 mm";
}
