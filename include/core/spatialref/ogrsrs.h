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
 *  The class OgrSRS provides service of spatial coordinate system
 *  with GDAL & OGR as backend.
 */
class EXCORE_EXPORTS OgrSRS final : public ISRS
{
public:
  explicit OgrSRS(int epsg);
  explicit OgrSRS(const std::string& wkt);
  ~OgrSRS() = default;

  void GetDatum() override {}

private:
  friend class OgrCT;

  UPSpatialRefSys mSRS;

  // cached info
};

using UPOgrSRS = std::unique_ptr<OgrSRS>;
using SPOgrSRS = std::shared_ptr<OgrSRS>;
}
