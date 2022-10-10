/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/geotrans/exgeotrans.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/spatialref/ccsct.h"
#include "core/spatialref/ccssrs.h"

namespace earthxpert {
// helpers & operators
inline Point3d ToPoint3d(const ccs::GeodeticCoordinates& p) {
  return { p.longitude(), p.latitude(), p.height() };
}
inline Point3d ToPoint3d(const ccs::CartesianCoordinates& p) {
  return { p.x(), p.y(), p.z() };
}
inline Point3d ToPoint3d(const ccs::UTMCoordinates& p) {
  return { p.easting(), p.northing(), 0.0 };
}

inline ccs::GeodeticCoordinates
ToGeodeticCoordinates(const ccs::CoordinateType::Enum ct, const Point3d& p) {
  return ccs::GeodeticCoordinates(ct, p.x, p.y, p.z);
}
inline ccs::CartesianCoordinates
ToCartesianCoordinates(const ccs::CoordinateType::Enum ct, const Point3d& p) {
  return ccs::CartesianCoordinates(ct, p.x, p.y, p.z);
}
inline ccs::UTMCoordinates
ToUTMCoordinates(const ccs::CoordinateType::Enum ct, const Point3d& p) {
  return ccs::UTMCoordinates(ct, 0, 0, p.x, p.y);
}

/**
 * Function which uses the given Geodetic (Ellipsoid Height) to Geocentric
 * Coordinate Conversion Service, 'ccsGeodToGeoc', to
 * convert the given lat, lon, and hgt to x, y, z coordinates.
 **/
inline void ConvertGeodeticToGeocentric(ccs2& ccsGeodToGeoc,
  const Point3d& geod, Point3d& geoc) {
  ConvertGeodeticToGeocentric(ccsGeodToGeoc,
    geod.y, geod.x, geod.z, geoc.x, geoc.y, geoc.z);
}
inline Point3d ConvertGeodeticToGeocentric(ccs2& ccsGeodToGeoc, const Point3d& geod) {
  Point3d geoc;
  ConvertGeodeticToGeocentric(ccsGeodToGeoc,
    geod.y, geod.x, geod.z, geoc.x, geoc.y, geoc.z);
  return geoc;
}

/**
 * Function which uses the given Geodetic (Ellipsoid Height) to Geocentric
 * Coordinate Conversion Service, 'ccsGeodToGeoc', to
 * convert the given x, y, z coordinates to a lat, lon, and hgt.
 **/
inline void ConvertGeocentricToGeodetic(ccs2& ccsGeodToGeoc,
  const Point3d& geoc, Point3d& geod) {
  ConvertGeocentricToGeodetic(ccsGeodToGeoc,
    geoc.x, geoc.y, geoc.z, geod.x, geod.y, geod.z);
}
inline Point3d ConvertGeocentricToGeodetic(ccs2& ccsGeodToGeoc, const Point3d& geoc) {
  Point3d geod;
  ConvertGeocentricToGeodetic(ccsGeodToGeoc,
    geoc.x, geoc.y, geoc.z, geod.x, geod.y, geod.z);
  return geod;
}

/**
 * Function which uses the given Geocentric to Geodetic (MSL EGM 96 15M)
 * Coordinate Conversion Service, 'ccsGeocToGeodMslEgm96', to
 * convert the given x, y, z coordinates to a lat, lon, and hgt.
 **/
inline void ConvertGeocentricToGeodeticMslEgm96(ccs2& ccsGeocToGeodMslEgm96,
  const Point3d& geoc, Point3d& geod) {
  ConvertGeocentricToGeodeticMslEgm96(ccsGeocToGeodMslEgm96,
    geoc.x, geoc.y, geoc.z, geod.y, geod.x, geod.z);
}
inline Point3d ConvertGeocentricToGeodeticMslEgm96(ccs2& ccsGeocToGeodMslEgm96, const Point3d& geoc) {
  Point3d geod;
  ConvertGeocentricToGeodeticMslEgm96(ccsGeocToGeodMslEgm96, geoc, geod);
  return geod;
}

/**
 * Function which uses the given Geodetic (MSL EGM 96 15M) to Geodetic
 * (Ellipsoid Height) Coordinate Conversion Service,
 * 'ccsMslEgm96ToEllHeight', to convert the given MSL hgt at the
 * given lat, lon, to an Ellipsoid hgt.
 **/
inline void ConvertMslEgm96ToEllHeight(ccs2& ccsMslEgm96ToEllHeight, 
  const Point3d& geod, double& ellHeight) {
  ConvertMslEgm96ToEllHeight(ccsMslEgm96ToEllHeight, geod.y, geod.x, geod.z, ellHeight);
}
inline double ConvertMslEgm96ToEllHeight(ccs2& ccsMslEgm96ToEllHeight, const Point3d& geod) {
  return ConvertMslEgm96ToEllHeight(ccsMslEgm96ToEllHeight, geod.y, geod.x, geod.z);
}

/**
 * Function which uses the given Geodetic (Ellipsoid Height) to Geodetic
 * (MSL EGM 96 15M) Coordinate Conversion Service,
 * 'ccsEllHeightToMslEgm96', to convert the given Ellipsoid hgt at
 * the given lat, lon, to an MSL hgt.
 **/
inline void ConvertEllHeightToMslEgm96(ccs2& ccsEllHeightToMslEgm96, 
  const Point3d& geod, double& hgtMsl) {
  ConvertEllHeightToMslEgm96(ccsEllHeightToMslEgm96, geod.y, geod.x, geod.z, hgtMsl);
}
inline double ConvertEllHeightToMslEgm96(ccs2& ccsEllHeightToMslEgm96, const Point3d& geod) {
  return ConvertEllHeightToMslEgm96(ccsEllHeightToMslEgm96, geod.y, geod.x, geod.z);
}

/**
 * Function which uses the given Geocentric to UTM Coordinate Conversion
 * Service, 'ccsGeocToUtm', to convert the given x, y, z coordinates
 * a UTM zone, hemisphere, Easting and Northing.
 **/
inline void ConvertGeocentricToUtm(ccs2& ccsGeocToUtm,
  const Point3d& geoc, Point2d& utm,
  long* zone = nullptr, char* hemisphere = nullptr) {
  ConvertGeocentricToUtm(ccsGeocToUtm,
    geoc.x, geoc.y, geoc.z, utm.x, utm.y, zone, hemisphere);
}
inline Point2d ConvertGeocentricToUtm(ccs2& ccsGeocToUtm,
  const Point3d& geoc, long* zone = nullptr, char* hemisphere = nullptr) {
  Point2d utm;
  ConvertGeocentricToUtm(ccsGeocToUtm,
    geoc.x, geoc.y, geoc.z, utm.x, utm.y, zone, hemisphere);
  return utm;
}
}
