#include "3rdparty/gtest/gtest_main_gdal.h"
#include "3rdparty/geotrans/exgeotrans.h"
#include "core/math/constants.h"

#include <Accuracy.h>
#include <CartesianCoordinates.h>
#include <CoordinateSystemParameters.h>
#include <CoordinateTuple.h>
#include <CoordinateType.h>
#include <GeodeticCoordinates.h>
#include <GeodeticParameters.h>
#include <HeightType.h>
#include <MGRSorUSNGCoordinates.h>
#include <UTMCoordinates.h>
#include <UTMParameters.h>
#include <iostream>
#include <memory>

using namespace earthxpert;

static constexpr double kTolR = Deg2Rad(1.0e-8);   // [mm]
static constexpr double kTol = 1.0e-3;   // [mm]

static void ConvertGeodeticToGeocentric(
  ccs::CoordinateConversionService& ccsGeodeticToGeocentric,
  double lat, double lon, double hgt, double& x, double& y, double& z);
static void ConvertGeocentricToGeodetic(
  ccs::CoordinateConversionService& ccsGeodeticToGeocentric,
  double x, double y, double z, double& lat, double& lon, double& hgt);
static void ConvertGeocentricToGeodeticMslEgm96(
  ccs::CoordinateConversionService& ccsGeocentricToGeodeticMslEgm96,
  double x, double y, double z, double& lat, double& lon, double& hgt);
static void ConvertMslEgm96ToEllHeight(
  ccs::CoordinateConversionService& ccsMslEgm96ToEllHeight,
  double lat, double lon, double hgtMsl, double& ellHeight);
static void ConvertEllHeightToMslEgm96(
  ccs::CoordinateConversionService& ccsEllHeightToMslEgm96,
  double lat, double lon, double ellHeight, double& hgtMsl);
static void ConvertGeocentricToUtm(
  ccs::CoordinateConversionService& ccsGeocentricToUtm,
  double x, double y, double z, long& zone, char& hemisphere,
  double& easting, double& northing);
static std::string ConvertGeocentricToMgrs(
  ccs::CoordinateConversionService& ccsGeocentricToMgrs,
  double x, double y, double z, ccs::Precision::Enum& precision);

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
TEST(geotrans, CCS) {
  const char* WGE = "WGE";

  // Coordinate System Parameters 
  ccs::GeodeticParameters ellipsoidParameters(
    ccs::CoordinateType::geodetic,
    ccs::HeightType::ellipsoidHeight);

  ccs::CoordinateSystemParameters geocentricParameters(
    ccs::CoordinateType::geocentric);

  ccs::GeodeticParameters mslEgm96Parameters(
    ccs::CoordinateType::geodetic,
    ccs::HeightType::EGM96FifteenMinBilinear);

  ccs::UTMParameters utmParameters(
    ccs::CoordinateType::universalTransverseMercator,
    1, 0);

  ccs::CoordinateSystemParameters mgrsParameters(
    ccs::CoordinateType::militaryGridReferenceSystem);

  // Coordinate Conversion Services 
  ccs::CoordinateConversionService ccsGeodeticToGeocentric(
    WGE, &ellipsoidParameters, WGE, &geocentricParameters);

  ccs::CoordinateConversionService ccsGeocentricToGeodeticMslEgm96(
    WGE, &geocentricParameters, WGE, &mslEgm96Parameters);

  ccs::CoordinateConversionService ccsMslEgm96ToEllHeight(
    WGE, &mslEgm96Parameters, WGE, &ellipsoidParameters);
  ccs::CoordinateConversionService ccsEllHeightToMslEgm96(
    WGE, &ellipsoidParameters, WGE, &mslEgm96Parameters);

  ccs::CoordinateConversionService ccsGeocentricToUtm(
    WGE, &geocentricParameters, WGE, &utmParameters);
  ccs::CoordinateConversionService ccsGeocentricToMgrs(
    WGE, &geocentricParameters, WGE, &mgrsParameters);

  // Geodetic (Ellipsoid Height) to Geocentric
  double lat = Deg2Rad(43.651070); // [radian]
  double lon = Deg2Rad(-79.347015);
  double hgt = 0.0;  // [m]

  double x{}, y{}, z{};
  ConvertGeodeticToGeocentric(
    ccsGeodeticToGeocentric, lat, lon, hgt, x, y, z);
  ASSERT_NEAR(854483.7334228333, x, kTol) << "ECEF X difference > 1 mm";
  ASSERT_NEAR(-4542656.050692913, y, kTol) << "ECEF Y difference > 1 mm";
  ASSERT_NEAR(4380121.547158894, z, kTol) << "ECEF Z difference > 1 mm";

  // Geocentric to Geodetic (Ellipsoid Height)

  // function ConvertGeocentricToGeodetic() reuses the
  // ccsGeodeticToGeocentric instance to perform the reverse conversion
  double latN{}, lonN{}, hgtN{};
  ConvertGeocentricToGeodetic(
    ccsGeodeticToGeocentric, x, y, z, latN, lonN, hgtN);
  ASSERT_NEAR(latN, lat, kTolR) << "Latitude difference > 1 mm";
  ASSERT_NEAR(lonN, lon, kTolR) << "Longitude difference > 1 mm";
  ASSERT_NEAR(hgtN, hgt, kTol) << "Height difference > 1 mm";

  // reuse ccsGeodeticToGeocentric instance to perform another
  // Geodetic (Ellipsoid Height) to Geocentric conversions
  double xN{}, yN{}, zN{};
  ConvertGeodeticToGeocentric(
    ccsGeodeticToGeocentric, lat, lon, 1000.0, xN, yN, zN);
  ASSERT_NEAR(854617.4903328838, xN, kTol) << "ECEF X difference > 1 mm";
  ASSERT_NEAR(-4543367.136946514, yN, kTol) << "ECEF Y difference > 1 mm";
  ASSERT_NEAR(4380811.81191169, zN, kTol) << "ECEF Z difference > 1 mm";

  // reuse ccsGeodeticToGeocentric instance to perform another
  // Geodetic (Ellipsoid Height) to Geocentric conversions
  ConvertGeodeticToGeocentric(
    ccsGeodeticToGeocentric, lat, lon, 10000.0, xN, yN, zN);
  ASSERT_NEAR(855821.3025233384, xN, kTol) << "ECEF X difference > 1 mm";
  ASSERT_NEAR(-4549766.913228926, yN, kTol) << "ECEF Y difference > 1 mm";
  ASSERT_NEAR(4387024.194686857, zN, kTol) << "ECEF Z difference > 1 mm";

  // Geocentric to Geodetic (MSL EGM96 15M)
  double hgtMsl{};
  ConvertGeocentricToGeodeticMslEgm96(
    ccsGeocentricToGeodeticMslEgm96, x, y, z, latN, lonN, hgtMsl);
  ASSERT_NEAR(latN, lat, kTolR) << "Latitude difference > 1 mm";
  ASSERT_NEAR(lonN, lon, kTolR) << "Longitude difference > 1 mm";
  ASSERT_NEAR(hgtMsl, 37.2765088347372, kTol) << "Height difference > 1 mm";

  // Geodetic (MSL EGM96 15M) to Geodetic (Ellipsoid Height)   
  ConvertMslEgm96ToEllHeight(
    ccsMslEgm96ToEllHeight, lat, lon, hgtMsl, hgtN);
  ASSERT_NEAR(hgtN, hgt, kTol) << "Height difference > 1 mm";

  // Geodetic (Ellipsoid Height) to Geodetic (MSL EMG96 15M)
  double hgtMslN{};
  ConvertEllHeightToMslEgm96(
    ccsEllHeightToMslEgm96, lat, lon, hgtN, hgtMslN);
  ASSERT_NEAR(hgtMslN, hgtMsl, kTol) << "Height difference > 1 mm";

  // Geocentric to UTM
  long zone;
  char hemi;
  double easting, northing;
  ConvertGeocentricToUtm(
    ccsGeocentricToUtm, x, y, z, zone, hemi, easting, northing);
  ASSERT_EQ(zone, 17) << "zone 17";
  ASSERT_EQ(hemi, 'N') << "North hemisphere";
  ASSERT_NEAR(easting, 633301.7104163736, kTol) << "UTM easting difference > 1 mm";
  ASSERT_NEAR(northing, 4834446.627614005, kTol) << "UTM northing difference > 1 mm";

  // Geocentric to MGRS
  ccs::Precision::Enum precision;
  std::string mgrsString = ConvertGeocentricToMgrs(
    ccsGeocentricToMgrs, x, y, z, precision);
  ASSERT_EQ(precision, 5);
  ASSERT_STREQ(mgrsString.c_str(), "17TPJ3330134446");
}

 /**
  * Function which uses the given Geodetic (Ellipsoid Height) to Geocentric
  * Coordinate Conversion Service, 'ccsGeodeticToGeocentric', to
  * convert the given lat, lon, and hgt to x, y, z coordinates.
  **/
void ConvertGeodeticToGeocentric(
  ccs::CoordinateConversionService& ccsGeodeticToGeocentric,
  double lat, double lon, double hgt, double& x, double& y, double& z) {
  ccs::Accuracy sourceAccuracy;
  ccs::Accuracy targetAccuracy;
  ccs::GeodeticCoordinates lph(ccs::CoordinateType::geodetic, lon, lat, hgt);
  ccs::CartesianCoordinates xyz(ccs::CoordinateType::geocentric);

  ccsGeodeticToGeocentric.convertSourceToTarget(
    &lph, &sourceAccuracy, xyz, targetAccuracy);

  x = xyz.x();
  y = xyz.y();
  z = xyz.z();
}

/**
 * Function which uses the given Geodetic (Ellipsoid Height) to Geocentric
 * Coordinate Conversion Service, 'ccsGeodeticToGeocentric', to
 * convert the given x, y, z coordinates to a lat, lon, and hgt.
 **/
void ConvertGeocentricToGeodetic(
  ccs::CoordinateConversionService& ccsGeodeticToGeocentric,
  double x, double y, double z, double& lat, double& lon, double& hgt) {
  ccs::Accuracy geocentricAccuracy;
  ccs::Accuracy geodeticAccuracy;
  ccs::CartesianCoordinates xyz(ccs::CoordinateType::geocentric, x, y, z);
  ccs::GeodeticCoordinates lph;

  // Note that the Geodetic (Ellipsoid Height) to Geocentric Coordinate
  // Conversion Service is used here in conjunction with the
  // convertTargetToSource() method (as opposed to a Geocentric to
  // Geodetic (Ellipsoid Height) Coordinate Conversion Service in
  // conjunction with the convertSourceToTarget() method)
  ccsGeodeticToGeocentric.convertTargetToSource(
    &xyz, &geocentricAccuracy, lph, geodeticAccuracy);

  lat = lph.latitude();
  lon = lph.longitude();
  hgt = lph.height();
}

/**
 * Function which uses the given Geocentric to Geodetic (MSL EGM 96 15M)
 * Coordinate Conversion Service, 'ccsGeocentricToGeodeticMslEgm96', to
 * convert the given x, y, z coordinates to a lat, lon, and hgt.
 **/
void ConvertGeocentricToGeodeticMslEgm96(
  ccs::CoordinateConversionService& ccsGeocentricToGeodeticMslEgm96,
  double x, double y, double z, double& lat, double& lon, double& hgt) {
  ccs::Accuracy sourceAccuracy;
  ccs::Accuracy targetAccuracy;
  ccs::CartesianCoordinates xyz(ccs::CoordinateType::geocentric, x, y, z);
  ccs::GeodeticCoordinates lph(ccs::CoordinateType::geodetic, lon, lat, hgt);

  ccsGeocentricToGeodeticMslEgm96.convertSourceToTarget(
    &xyz, &sourceAccuracy, lph, targetAccuracy);

  lat = lph.latitude();
  lon = lph.longitude();
  hgt = lph.height();
}

/**
 * Function which uses the given Geodetic (MSL EGM 96 15M) to Geodetic
 * (Ellipsoid Height) Coordinate Conversion Service,
 * 'ccsMslEgm96ToEllHeight', to convert the given MSL hgt at the
 * given lat, lon, to an Ellipsoid hgt.
 **/
void ConvertMslEgm96ToEllHeight(
  ccs::CoordinateConversionService& ccsMslEgm96ToEllHeight,
  double lat, double lon, double hgtMsl, double& ellHeight) {
  ccs::Accuracy sourceAccuracy;
  ccs::Accuracy targetAccuracy;
  ccs::GeodeticCoordinates lphMsl(ccs::CoordinateType::geodetic, lon, lat, hgtMsl);
  ccs::GeodeticCoordinates lph;

  ccsMslEgm96ToEllHeight.convertSourceToTarget(
    &lphMsl, &sourceAccuracy, lph, targetAccuracy);

  ellHeight = lph.height();
}

/**
 * Function which uses the given Geodetic (Ellipsoid Height) to Geodetic
 * (MSL EGM 96 15M) Coordinate Conversion Service,
 * 'ccsEllHeightToMslEgm96', to convert the given Ellipsoid hgt at
 * the given lat, lon, to an MSL hgt.
 **/
void ConvertEllHeightToMslEgm96(
  ccs::CoordinateConversionService& ccsEllHeightToMslEgm96,
  double lat, double lon, double ellHeight, double& hgtMsl) {
  ccs::Accuracy sourceAccuracy;
  ccs::Accuracy targetAccuracy;
  ccs::GeodeticCoordinates lph(ccs::CoordinateType::geodetic, lon, lat, ellHeight);
  ccs::GeodeticCoordinates lphMsl;

  ccsEllHeightToMslEgm96.convertSourceToTarget(
    &lph, &sourceAccuracy, lphMsl, targetAccuracy);

  hgtMsl = lphMsl.height();
}

/**
 * Function which uses the given Geocentric to UTM Coordinate Conversion
 * Service, 'ccsGeocentricToUtm', to convert the given x, y, z coordinates
 * a UTM zone, hemisphere, Easting and Northing.
 **/
void ConvertGeocentricToUtm(
  ccs::CoordinateConversionService& ccsGeocentricToUtm,
  double x, double y, double z, long& zone,
  char& hemisphere, double& easting, double& northing) {
  ccs::Accuracy sourceAccuracy;
  ccs::Accuracy targetAccuracy;
  ccs::CartesianCoordinates xyz(ccs::CoordinateType::geocentric, x, y, z);
  ccs::UTMCoordinates utm;

  ccsGeocentricToUtm.convertSourceToTarget(
    &xyz, &sourceAccuracy, utm, targetAccuracy);

  zone = utm.zone();
  hemisphere = utm.hemisphere();
  easting = utm.easting();
  northing = utm.northing();
}

/**
 * Function which uses the given Geocentric to MGRS Coordinate Conversion
 * Service, 'ccsGeocentricToMgrs', to convert the given x, y, z coordinates
 * to an MGRS string and precision.
 **/
std::string ConvertGeocentricToMgrs(
  ccs::CoordinateConversionService& ccsGeocentricToMgrs,
  double x, double y, double z, ccs::Precision::Enum& precision) {
  ccs::Accuracy sourceAccuracy;
  ccs::Accuracy targetAccuracy;
  ccs::CartesianCoordinates xyz(ccs::CoordinateType::geocentric, x, y, z);
  ccs::MGRSorUSNGCoordinates mgrs;

  ccsGeocentricToMgrs.convertSourceToTarget(
    &xyz, &sourceAccuracy, mgrs, targetAccuracy);

  precision = mgrs.precision();

  // Returned value, 'p', points to targetCoordinate's internal character
  // array so assign/copy the character array to mgrsString
  std::string mgrsString = mgrs.MGRSString();

  return mgrsString;
}
