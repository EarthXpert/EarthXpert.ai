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

namespace earthxpert {
/**
 *  \brief coordinate transform from LSR to UTM
 */
struct EXUTILS_EXPORTS LsrToUtmCT {
  EarthEllipsoid ee;

  void Convert(const Point3d* lsr, const double* pb, Point2d& utm) const {}
};

using UPLsrToUtmCT = std::unique_ptr<LsrToUtmCT>;
using SPLsrToUtmCT = std::shared_ptr<LsrToUtmCT>;
} // namespace earthxpert
