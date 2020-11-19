#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/geotrans/exgeotrans.h"
#include "core/math/constants.h"
#include "core/spatialref/ccsutils.h"

#include <iostream>

using namespace earthxpert;

static constexpr double kTolR = Deg2Rad(1.0e-8);   // [mm]
static constexpr double kTol = 1.0e-3;   // [mm]

/**
 * \brief test the MSP Coordinate Conversion Service.
 *
 * Includes the following conversions:
 *
 * |=============================|=============================|
 * | Source                      | Target                      |
 * |=============================+=============================|
 * | Geodetic (Ellipsoid Height) | Geocentric                  |
 * | Geocentric                  | Geodetic (Ellipsoid Height) |
 * |-----------------------------+-----------------------------|
 * | Geocentric                  | Geodetic (MSL EGM 96 15M)   |
 * |-----------------------------+-----------------------------|
 * | Geodetic (Ellipsoid Height) | Geodetic (MSL EGM 96 15M)   |
 * | Geodetic (MSL EGM 96 15M)   | Geodetic (Ellipsoid Height) |
 * |-----------------------------+-----------------------------|
 * | Geocentric                  | UTM                         |
 * |-----------------------------+-----------------------------|
 * | Geocentric                  | MGRS                        |
 * |-----------------------------+-----------------------------|
 */
TEST(geotrans, CCS_WGS84) {
  const char* datum = "WGE";

  // Coordinate System Parameters 
  ccs::GeodeticParameters ellParam(ccs_ct::geodetic, ccs_ht::ellipsoidHeight);
  ccs::CoordinateSystemParameters geocParam(ccs_ct::geocentric);
  ccs::GeodeticParameters mslEgm96Param(
    ccs_ct::geodetic, ccs_ht::EGM96FifteenMinBilinear);
  ccs::UTMParameters utmParam(ccs_ct::universalTransverseMercator, 1, 0);
  ccs::CoordinateSystemParameters mgrsParam(ccs_ct::militaryGridReferenceSystem);

  // Coordinate Conversion Services 
  ccs2 ccsGeodToGeoc(datum, &ellParam, datum, &geocParam);
  ccs2 ccsGeocToGeodMslEgm96(datum, &geocParam, datum, &mslEgm96Param);
  ccs2 ccsMslEgm96ToEllHeight(datum, &mslEgm96Param, datum, &ellParam);
  ccs2 ccsEllHeightToMslEgm96(datum, &ellParam, datum, &mslEgm96Param);
  ccs2 ccsGeocToUtm(datum, &geocParam, datum, &utmParam);
  ccs2 ccsGeocToMgrs(datum, &geocParam, datum, &mgrsParam);

  // Geodetic (Ellipsoid Height) to Geocentric
  double lat = Deg2Rad(43.651070); // Toronto
  double lon = Deg2Rad(-79.347015);
  double hgt = 0.0;  // [m]

  double x{}, y{}, z{};
  ConvertGeodeticToGeocentric(ccsGeodToGeoc, lat, lon, hgt, x, y, z);
  ASSERT_NEAR(854483.7334228333, x, kTol) << "ECEF X difference > 1 mm";
  ASSERT_NEAR(-4542656.050692913, y, kTol) << "ECEF Y difference > 1 mm";
  ASSERT_NEAR(4380121.547158894, z, kTol) << "ECEF Z difference > 1 mm";

  // Geocentric to Geodetic (Ellipsoid Height)

  // function ConvertGeocentricToGeodetic() reuses the
  // ccsGeodToGeoc instance to perform the reverse conversion
  double latN{}, lonN{}, hgtN{};
  ConvertGeocentricToGeodetic(ccsGeodToGeoc, x, y, z, latN, lonN, hgtN);
  ASSERT_NEAR(latN, lat, kTolR) << "Latitude difference > 1 mm";
  ASSERT_NEAR(lonN, lon, kTolR) << "Longitude difference > 1 mm";
  ASSERT_NEAR(hgtN, hgt, kTol) << "Height difference > 1 mm";

  // reuse ccsGeodToGeoc instance to perform another
  // Geodetic (Ellipsoid Height) to Geocentric conversions
  double xN{}, yN{}, zN{};
  ConvertGeodeticToGeocentric(ccsGeodToGeoc, lat, lon, 1000.0, xN, yN, zN);
  ASSERT_NEAR(854617.4903328838, xN, kTol) << "ECEF X difference > 1 mm";
  ASSERT_NEAR(-4543367.136946514, yN, kTol) << "ECEF Y difference > 1 mm";
  ASSERT_NEAR(4380811.81191169, zN, kTol) << "ECEF Z difference > 1 mm";

  // reuse ccsGeodToGeoc instance to perform another
  // Geodetic (Ellipsoid Height) to Geocentric conversions
  ConvertGeodeticToGeocentric(ccsGeodToGeoc, lat, lon, 10000.0, xN, yN, zN);
  ASSERT_NEAR(855821.3025233384, xN, kTol) << "ECEF X difference > 1 mm";
  ASSERT_NEAR(-4549766.913228926, yN, kTol) << "ECEF Y difference > 1 mm";
  ASSERT_NEAR(4387024.194686857, zN, kTol) << "ECEF Z difference > 1 mm";

  // Geocentric to Geodetic (MSL EGM96 15M)
  double hgtMsl{};
  ConvertGeocentricToGeodeticMslEgm96(ccsGeocToGeodMslEgm96, x, y, z, latN, lonN, hgtMsl);
  ASSERT_NEAR(latN, lat, kTolR) << "Latitude difference > 1 mm";
  ASSERT_NEAR(lonN, lon, kTolR) << "Longitude difference > 1 mm";
  ASSERT_NEAR(hgtMsl, 37.2765088347372, kTol) << "Height difference > 1 mm";

  // Geodetic (MSL EGM96 15M) to Geodetic (Ellipsoid Height)   
  ConvertMslEgm96ToEllHeight(ccsMslEgm96ToEllHeight, lat, lon, hgtMsl, hgtN);
  ASSERT_NEAR(hgtN, hgt, kTol) << "Height difference > 1 mm";

  // Geodetic (Ellipsoid Height) to Geodetic (MSL EMG96 15M)
  double hgtMslN{};
  ConvertEllHeightToMslEgm96(ccsEllHeightToMslEgm96, lat, lon, hgtN, hgtMslN);
  ASSERT_NEAR(hgtMslN, hgtMsl, kTol) << "Height difference > 1 mm";

  // Geocentric to UTM
  long zone;
  char hemi;
  double easting, northing;
  ConvertGeocentricToUtm(ccsGeocToUtm, x, y, z, zone, hemi, easting, northing);
  ASSERT_EQ(zone, 17) << "zone 17";
  ASSERT_EQ(hemi, 'N') << "North hemisphere";
  ASSERT_NEAR(easting, 633301.7104163736, kTol) << "UTM easting difference > 1 mm";
  ASSERT_NEAR(northing, 4834446.627614005, kTol) << "UTM northing difference > 1 mm";

  // Geocentric to MGRS
  ccs::Precision::Enum precision;
  std::string mgrsString = 
    ConvertGeocentricToMgrs(ccsGeocToMgrs, x, y, z, precision);
  ASSERT_EQ(precision, 5);
  ASSERT_STREQ(mgrsString.c_str(), "17TPJ3330134446");
}

// WGS84 vs NAD83
TEST(geotrans, CCS_WGS84_NAD83) {
  const char* datumWgs84 = "WGE";
  const char* datumNad83 = "NAR-B"; // NAD83 canada

  ccs::GeodeticParameters ellParam(ccs_ct::geodetic, ccs_ht::ellipsoidHeight);
  ccs2 ccsWgs84ToNad83(datumWgs84, &ellParam, datumNad83, &ellParam);
  ccs2 ccsNad83ToWgs84(datumNad83, &ellParam, datumWgs84, &ellParam);

  ccs::Accuracy srcAcc, tgtAcc;

  double lat = Deg2Rad(43.651070); // [radian]
  double lon = Deg2Rad(-79.347015);
  double hgt = 10000.0;  // [m]

  ccs::GeodeticCoordinates lph84(ccs_ct::geodetic, lon, lat, hgt);
  ccs::GeodeticCoordinates nad83;
  ccsWgs84ToNad83.convertSourceToTarget(&lph84, &srcAcc, nad83, tgtAcc);
  ASSERT_NEAR(nad83.latitude(), 0.76185489353494085, kTolR) << "Latitude diff > 1 mm";
  ASSERT_NEAR(nad83.longitude(), -1.3848666633793285, kTolR) << "Latitude diff > 1 mm";
  ASSERT_NEAR(nad83.height(), 10000.000049856966, kTol) << "Height diff > 1 mm";

  lph84.setHeight(0.0);
  ccsWgs84ToNad83.convertSourceToTarget(&lph84, &srcAcc, nad83, tgtAcc);
  ASSERT_NEAR(nad83.latitude(), 0.76185489353494085, kTolR) << "Latitude diff > 1 mm";
  ASSERT_NEAR(nad83.longitude(), -1.3848666633793285, kTolR) << "Latitude diff > 1 mm";
  ASSERT_NEAR(nad83.height(), 4.9856966276674999e-05, kTol) << "Height diff > 1 mm";

  ccs::GeodeticCoordinates lph84N;
  ccsNad83ToWgs84.convertSourceToTarget(&nad83, &srcAcc, lph84N, tgtAcc);
  ASSERT_NEAR(lph84N.latitude(), lph84.latitude(), kTolR) << "Latitude diff > 1 cm";
  ASSERT_NEAR(lph84N.longitude(), lph84.longitude(), kTolR) << "Latitude diff > 1 cm";
  ASSERT_NEAR(lph84N.height(), lph84.height(), kTol) << "Height diff > 1 mm";
}

// WGS84 vs WGS72
TEST(geotrans, CCS_WGS84_WGS72) {
  const char* datumWgs84 = "WGE";
  const char* datumWgs72 = "WGC";

  ccs::GeodeticParameters ellParam(ccs_ct::geodetic, ccs_ht::ellipsoidHeight);
  ccs2 ccsWgs84ToWgs72(datumWgs84, &ellParam, datumWgs72, &ellParam);
  ccs2 ccsWgs72ToWgs84(datumWgs72, &ellParam, datumWgs84, &ellParam);

  ccs::Accuracy srcAcc, tgtAcc;

  double lat = Deg2Rad(43.651070); // [radian]
  double lon = Deg2Rad(-79.347015);
  double hgt = 10000.0;  // [m]

  ccs::GeodeticCoordinates lph84(ccs_ct::geodetic, lon, lat, hgt);
  ccs::GeodeticCoordinates lph72;
  ccsWgs84ToWgs72.convertSourceToTarget(&lph84, &srcAcc, lph72, tgtAcc);
  ASSERT_NEAR(lph72.latitude(), 0.76185435184769834, kTolR) << "Latitude diff > 1 mm";
  ASSERT_NEAR(lph72.longitude(), -1.3848693492471218, kTolR) << "Latitude diff > 1 mm";
  ASSERT_NEAR(lph72.height(), 9997.3989608526881, kTol) << "Height diff > 1 mm";

  lph84.setHeight(0.0);
  ccsWgs84ToWgs72.convertSourceToTarget(&lph84, &srcAcc, lph72, tgtAcc);
  ASSERT_NEAR(lph72.latitude(), 0.76185435184769834, kTolR) << "Latitude diff > 1 mm";
  ASSERT_NEAR(lph72.longitude(), -1.3848693492471218, kTolR) << "Latitude diff > 1 mm";
  ASSERT_NEAR(lph72.height(), -2.6010391473114263, kTol) << "Height diff > 1 mm";

  ccs::GeodeticCoordinates lph84N;
  ccsWgs72ToWgs84.convertSourceToTarget(&lph72, &srcAcc, lph84N, tgtAcc);
  ASSERT_NEAR(lph84N.latitude(), lph84.latitude(), kTolR) << "Latitude diff > 1 cm";
  ASSERT_NEAR(lph84N.longitude(), lph84.longitude(), kTolR) << "Latitude diff > 1 cm";
  ASSERT_NEAR(lph84N.height(), lph84.height(), kTol) << "Height diff > 1 mm";
}

// WGS84 vs NAD27
TEST(geotrans, CCS_WGS84_NAD27) {
  const char* datumWgs84 = "WGE";
  const char* datumNad27 = "NAS-E"; // NAD27 Canada

  ccs::GeodeticParameters ellParam(ccs_ct::geodetic, ccs_ht::ellipsoidHeight);
  ccs2 ccsWgs84ToNad27(datumWgs84, &ellParam, datumNad27, &ellParam);
  ccs2 ccsNad27ToWgs84(datumNad27, &ellParam, datumWgs84, &ellParam);

  ccs::Accuracy srcAcc, tgtAcc;

  double lat = Deg2Rad(43.651070); // [radian]
  double lon = Deg2Rad(-79.347015);
  double hgt = 10000.0;  // [m]

  ccs::GeodeticCoordinates lph84(ccs_ct::geodetic, lon, lat, hgt);
  ccs::GeodeticCoordinates lph27;
  ccsWgs84ToNad27.convertSourceToTarget(&lph84, &srcAcc, lph27, tgtAcc);
  ASSERT_NEAR(lph27.latitude(), 0.7618539283318597, kTolR) << "Latitude diff > 1 mm";
  ASSERT_NEAR(lph27.longitude(), -1.384870849582658, kTolR) << "Latitude diff > 1 mm";
  ASSERT_NEAR(lph27.height(), 10028.36701276633, kTol) << "Height diff > 1 mm";

  lph84.setHeight(0.0);
  ccsWgs84ToNad27.convertSourceToTarget(&lph84, &srcAcc, lph27, tgtAcc);
  ASSERT_NEAR(lph27.latitude(), 0.7618539268156705, kTolR) << "Latitude diff > 1 mm";
  ASSERT_NEAR(lph27.longitude(), -1.384870856135546, kTolR) << "Latitude diff > 1 mm";
  ASSERT_NEAR(lph27.height(), 28.3670127663266, kTol) << "Height diff > 1 mm";

  ccs::GeodeticCoordinates lph84N;
  ccsNad27ToWgs84.convertSourceToTarget(&lph27, &srcAcc, lph84N, tgtAcc);
  ASSERT_NEAR(lph84N.latitude(), lph84.latitude(), kTolR*10) << "Latitude diff > 1 cm";
  ASSERT_NEAR(lph84N.longitude(), lph84.longitude(), kTolR*10) << "Latitude diff > 1 cm";
  ASSERT_NEAR(lph84N.height(), lph84.height(), kTol) << "Height diff > 1 mm";
}
