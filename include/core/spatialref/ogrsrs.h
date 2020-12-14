/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/spatialref/isrs.h"
#include "3rdparty/gdal/exgdal.h"

#include <memory>

namespace earthxpert {
/**
 *  The class OgrSrs provides service of spatial coordinate system
 *  with GDAL & OGR as backend.
 */
class EXCORE_EXPORTS OgrSrs final : public ISrs
{
public:
  explicit OgrSrs(int epsg);
  explicit OgrSrs(const std::string& wkt);
  ~OgrSrs() = default;

  void GetDatum() override {}

private:
  friend class OgrCt;

  UPSpatialRefSys mSrs;

  // cached info
};

using UPOgrSrs = std::unique_ptr<OgrSrs>;
using SPOgrSrs = std::shared_ptr<OgrSrs>;
}
