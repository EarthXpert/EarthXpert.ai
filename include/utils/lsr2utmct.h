/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "utils/exutils.h"
#include "3rdparty/ceres/exceres.h"
#include "3rdparty/gdal/exgdal.h"
#include "3rdparty/geotrans/exgeotrans.h"
#include "3rdparty/utils/constants.h"

#include "core/spatialref/ctfactory.h"
#include "core/spatialref/earthellipsoid.h"
#include "core/spatialref/srsfactory.h"
#include "core/utils/typehelper.h"
#include <cmath>
#include <memory>

enum ccs_ct::Enum;

namespace earthxpert {
/**
 *  \brief coordinate transform from LSR to UTM
 */
class EXUTILS_EXPORTS LsrToUtmCT {
public:
  LsrToUtmCT(const Point3d& origin) {
    ee.SetLsrOrigin(origin);
    Initialize();
  }
  ~LsrToUtmCT() = default;

  void Convert(const Point3d* lsr, const double* pb, Point2d& utm) const {
    const Point3d lph = ee.LSRToLPH(*lsr);

    double x{}, y{}, z{};
    ConvertGeodeticToGeocentric(*ccsGeodToGeoc, lph.y, lph.x, lph.z, x, y, z);

    long zone;
    char hemi;
    ConvertGeocentricToUtm(*ccsGeocToUtm, x, y, z, zone, hemi, utm.x, utm.y);
  }

private:
  EarthEllipsoid ee;

  ccs::GeodeticParameters ellParam;
  ccs::CoordinateSystemParameters geocParam;
  ccs::UTMParameters utmParam;

  std::unique_ptr<ccs2> ccsGeodToGeoc;
  std::unique_ptr<ccs2> ccsGeocToUtm;

  void Initialize() {
    const char* datum = "WGE";
    ellParam = ccs::GeodeticParameters(ccs_ct::geodetic, ccs_ht::ellipsoidHeight);
    geocParam = ccs::CoordinateSystemParameters(ccs_ct::geocentric);
    utmParam = ccs::UTMParameters(ccs_ct::universalTransverseMercator, 1, 0);

    ccsGeodToGeoc = std::make_unique<ccs2>(datum, &ellParam, datum, &geocParam);
    ccsGeocToUtm = std::make_unique<ccs2>(datum, &geocParam, datum, &utmParam);
  }
};

using UPLsrToUtmCT = std::unique_ptr<LsrToUtmCT>;
using SPLsrToUtmCT = std::shared_ptr<LsrToUtmCT>;
} // namespace earthxpert
