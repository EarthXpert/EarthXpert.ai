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
 *  The class GdalSRS provides service of spatial coordinate system
 *  with GDAL & OGR as backend.
 */
class EXCORE_EXPORTS GdalSRS final : public ISRS
{
public:
  ~GdalSRS() = default;

  void GetDatum() override {}
};

using UPGdalSRS = std::unique_ptr<GdalSRS>;
using SPGdalSRS = std::shared_ptr<GdalSRS>;
}
