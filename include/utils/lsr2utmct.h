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
  enum L2UCoeff {
    eS = 0,     // scale
    eO = 1,     // omega
    eP = 2,     // phi
    eK = 3,     // kappa
    eX = 4,     // X
    eY = 5,     // Y
    eZ = 6,     // Z
    NumCoef = 7 // number of coefficients
  };

  LsrToUtmCT(const Point3d& origin);
  ~LsrToUtmCT() = default;

  Point2d Convert(const Point3d* lsr, const double* pb) const;

private:
  EarthEllipsoid ee;

  std::unique_ptr<OGRSpatialRef> wgs84Srs;
  std::unique_ptr<OGRSpatialRef> utmSrs;
  std::unique_ptr<OgrCt> g2mCt;  // WGS84 -> UTM

  std::unique_ptr<ccs::GeodeticParameters> ellParam;
  std::unique_ptr<ccs::CoordinateSystemParameters> geocParam;
  std::unique_ptr<ccs::UTMParameters> utmParam;
  std::unique_ptr<ccs2> ccsGeodToGeoc;
  std::unique_ptr<ccs2> ccsGeocToUtm;

  void Initialize();
};

using luc = LsrToUtmCT::L2UCoeff;

using UPLsrToUtmCT = std::unique_ptr<LsrToUtmCT>;
using SPLsrToUtmCT = std::shared_ptr<LsrToUtmCT>;
} // namespace earthxpert
