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
// helper operators

/**
 * Function which uses the given Geodetic (Ellipsoid Height) to Geocentric
 * Coordinate Conversion Service, 'ccsGeodToGeoc', to
 * convert the given lat, lon, and hgt to x, y, z coordinates.
 **/
inline void ConvertGeodeticToGeocentric(ccs2& ccsGeodToGeoc,
  const Point3d& geod, Point3d& geoc) {
  ConvertGeodeticToGeocentric(ccsGeodToGeoc,
    geod.x, geod.y, geod.z, geoc.x, geoc.y, geoc.z);
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
}
