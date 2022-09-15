/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main_gdal.h"
#include "3rdparty/geotrans/exgeotrans.h"
#include "3rdparty/utils/constants.h"
#include "testconfig.h"

#include "core/spatialref/ctfactory.h"
#include "core/spatialref/earthellipsoid.h"
#include "core/spatialref/srsfactory.h"
#include "core/utils/typehelper.h"
#include <cmath>
#include <iostream>

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

/**
 *  Test case: geotrans
 */
TEST(coordinatetransform, LSR_WGS84_UTM) {
  const Point3d origin(Deg2Rad(39.02214166666666), Deg2Rad(5.453869444444445), 0);  // Africa 5°27?13.93?N, 39°1?19.71?E

  EarthEllipsoid ee;
  ee.SetLsrOrigin(origin);

  const char* datum = "WGE";
  ccs::GeodeticParameters ellParam(ccs_ct::geodetic, ccs_ht::ellipsoidHeight);
  ccs::CoordinateSystemParameters geocParam(ccs_ct::geocentric);
  ccs::GeodeticParameters mslEgm96Param(
    ccs_ct::geodetic, ccs_ht::EGM96FifteenMinBilinear);
  ccs::UTMParameters utmParam(ccs_ct::universalTransverseMercator, 1, 0);

  ccs2 ccsGeodToGeoc(datum, &ellParam, datum, &geocParam);
  ccs2 ccsEllHeightToMslEgm96(datum, &ellParam, datum, &mslEgm96Param);
  ccs2 ccsGeocToUtm(datum, &geocParam, datum, &utmParam);

  std::vector<Point3d> lsrs = {
    Point3d(4780, 4596, 1736.94), Point3d(5088.76, 5110.15, 1839.34),
    Point3d(5959.11, 5504.95, 1793.64)
  };
  std::vector<Point3d> utms = {
    Point3d(502359.5, 602749.7, 0), Point3d(502727.3, 603243.8, 0),
    Point3d(503178.1, 604121.5, 0)
  };

  for (int i = 0; i < lsrs.size(); ++i) {
    const auto lsr = lsrs[i] + Point3d(-4869, -4678, -1800);
    const Point3d lph = ee.LSRToLPH(lsr);

    double x{}, y{}, z{};
    ConvertGeodeticToGeocentric(ccsGeodToGeoc, lph.y, lph.x, lph.z, x, y, z);

    double hgtMsl{};
    ConvertEllHeightToMslEgm96(ccsEllHeightToMslEgm96, lph.y, lph.x, lph.z, hgtMsl);

    long zone;
    char hemi;
    double easting, northing;
    ConvertGeocentricToUtm(ccsGeocToUtm, x, y, z, zone, hemi, easting, northing);

    const auto& utm = utms[i];
    std::cout << lsr.x << ", " << lsr.y << ", " << lsr.z << "  "
      << easting << ", " << northing << ", " << easting-utm.x << ", " << northing-utm.y << "  "
      << lph.x << ", " << lph.y << ", " << lph.z << ", " << hgtMsl << std::endl;
  }
}
