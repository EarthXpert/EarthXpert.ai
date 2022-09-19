/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"

#include <CoordinateConversionException.h>
#include <CoordinateConversionService.h>

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

#include <string>

namespace ccs = MSP::CCS;
using ccs2 = ccs::CoordinateConversionService;
using ccs_ct = ccs::CoordinateType;
using ccs_ht = ccs::HeightType;

/**
 *  \env MSPCCS_DATA=D:\home\workspace\install\windows\etc\dtcc
 */
namespace earthxpert {
namespace thirdparty {
}

/**
 * Function which uses the given Geodetic (Ellipsoid Height) to Geocentric
 * Coordinate Conversion Service, 'ccsGeodToGeoc', to
 * convert the given lat, lon, and hgt to x, y, z coordinates.
 **/
inline void ConvertGeodeticToGeocentric(ccs2& ccsGeodToGeoc,
  double lat, double lon, double hgt, double& x, double& y, double& z) {
  ccs::Accuracy srcAcc, tgtAcc;
  ccs::GeodeticCoordinates lph(ccs_ct::geodetic, lon, lat, hgt);
  ccs::CartesianCoordinates xyz(ccs_ct::geocentric);

  ccsGeodToGeoc.convertSourceToTarget(&lph, &srcAcc, xyz, tgtAcc);

  x = xyz.x();
  y = xyz.y();
  z = xyz.z();
}

/**
 * Function which uses the given Geodetic (Ellipsoid Height) to Geocentric
 * Coordinate Conversion Service, 'ccsGeodToGeoc', to
 * convert the given x, y, z coordinates to a lat, lon, and hgt.
 **/
inline void ConvertGeocentricToGeodetic(ccs2& ccsGeodToGeoc,
  double x, double y, double z, double& lat, double& lon, double& hgt) {
  ccs::Accuracy geocAcc, geodAcc;
  ccs::CartesianCoordinates xyz(ccs_ct::geocentric, x, y, z);
  ccs::GeodeticCoordinates lph;

  // Note that the Geodetic (Ellipsoid Height) to Geocentric Coordinate
  // Conversion Service is used here in conjunction with the
  // convertTargetToSource() method (as opposed to a Geocentric to
  // Geodetic (Ellipsoid Height) Coordinate Conversion Service in
  // conjunction with the convertSourceToTarget() method)
  ccsGeodToGeoc.convertTargetToSource(&xyz, &geocAcc, lph, geodAcc);

  lat = lph.latitude();
  lon = lph.longitude();
  hgt = lph.height();
}

/**
 * Function which uses the given Geocentric to Geodetic (MSL EGM 96 15M)
 * Coordinate Conversion Service, 'ccsGeocToGeodMslEgm96', to
 * convert the given x, y, z coordinates to a lat, lon, and hgt.
 **/
inline void ConvertGeocentricToGeodeticMslEgm96(ccs2& ccsGeocToGeodMslEgm96,
  double x, double y, double z, double& lat, double& lon, double& hgt) {
  ccs::Accuracy srcAcc, tgtAcc;
  ccs::CartesianCoordinates xyz(ccs_ct::geocentric, x, y, z);
  ccs::GeodeticCoordinates lph(ccs_ct::geodetic, lon, lat, hgt);

  ccsGeocToGeodMslEgm96.convertSourceToTarget(&xyz, &srcAcc, lph, tgtAcc);

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
inline void ConvertMslEgm96ToEllHeight(ccs2& ccsMslEgm96ToEllHeight,
  double lat, double lon, double hgtMsl, double& ellHeight) {
  ccs::Accuracy srcAcc, tgtAcc;
  ccs::GeodeticCoordinates lphMsl(ccs_ct::geodetic, lon, lat, hgtMsl);
  ccs::GeodeticCoordinates lph;

  ccsMslEgm96ToEllHeight.convertSourceToTarget(&lphMsl, &srcAcc, lph, tgtAcc);
  ellHeight = lph.height();
}
inline double ConvertMslEgm96ToEllHeight(ccs2& ccsMslEgm96ToEllHeight,
  double lat, double lon, double hgtMsl) {
  double ellHeight{};
  ConvertMslEgm96ToEllHeight(ccsMslEgm96ToEllHeight,
    lat, lon, hgtMsl, ellHeight);
  return ellHeight;
}

/**
 * Function which uses the given Geodetic (Ellipsoid Height) to Geodetic
 * (MSL EGM 96 15M) Coordinate Conversion Service,
 * 'ccsEllHeightToMslEgm96', to convert the given Ellipsoid hgt at
 * the given lat, lon, to an MSL hgt.
 **/
inline void ConvertEllHeightToMslEgm96(ccs2& ccsEllHeightToMslEgm96,
  double lat, double lon, double ellHeight, double& hgtMsl) {
  ccs::Accuracy srcAcc, tgtAcc;
  ccs::GeodeticCoordinates lph(ccs_ct::geodetic, lon, lat, ellHeight);
  ccs::GeodeticCoordinates lphMsl;

  ccsEllHeightToMslEgm96.convertSourceToTarget(&lph, &srcAcc, lphMsl, tgtAcc);
  hgtMsl = lphMsl.height();
}
inline double ConvertEllHeightToMslEgm96(ccs2& ccsEllHeightToMslEgm96,
  double lat, double lon, double ellHeight) {
  double hgtMsl{};
  ConvertEllHeightToMslEgm96(ccsEllHeightToMslEgm96,
    lat, lon, ellHeight, hgtMsl);
  return hgtMsl;
}

/**
 * Function which uses the given Geocentric to UTM Coordinate Conversion
 * Service, 'ccsGeocToUtm', to convert the given x, y, z coordinates
 * a UTM zone, hemisphere, Easting and Northing.
 **/
inline void ConvertGeocentricToUtm(ccs2& ccsGeocToUtm,
  double x, double y, double z, double& easting, double& northing,
  long* zone=nullptr, char* hemisphere=nullptr) {
  ccs::Accuracy srcAcc, tgtAcc;
  ccs::CartesianCoordinates xyz(ccs_ct::geocentric, x, y, z);
  ccs::UTMCoordinates utm;

  ccsGeocToUtm.convertSourceToTarget(&xyz, &srcAcc, utm, tgtAcc);

  easting = utm.easting();
  northing = utm.northing();
  if (zone)
    *zone = utm.zone();
  if (hemisphere)
    *hemisphere = utm.hemisphere();
}

/**
 * Function which uses the given Geocentric to MGRS Coordinate Conversion
 * Service, 'ccsGeocToMgrs', to convert the given x, y, z coordinates
 * to an MGRS string and precision.
 **/
inline std::string ConvertGeocentricToMgrs(ccs2& ccsGeocToMgrs,
  double x, double y, double z, ccs::Precision::Enum& precision) {
  ccs::Accuracy srcAcc, tgtAcc;
  ccs::CartesianCoordinates xyz(ccs_ct::geocentric, x, y, z);
  ccs::MGRSorUSNGCoordinates mgrs;

  ccsGeocToMgrs.convertSourceToTarget(&xyz, &srcAcc, mgrs, tgtAcc);
  precision = mgrs.precision();

  // Returned value, 'p', points to targetCoordinate's internal character
  // array so assign/copy the character array to mgrsString
  return std::string(mgrs.MGRSString());
}
} // namespace earthxpert::thirdparty
