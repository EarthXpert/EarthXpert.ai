/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/spatialref/isrs.h"
#include "3rdparty/proj/exproj.h"

#include <memory>

namespace earthxpert {
/**
 *  The class ProjSRS provides service of spatial coordinate system
 *  with GDAL & OGR as backend.
 */
class EXCORE_EXPORTS ProjSRS final : public ISRS
{
public:
  ~ProjSRS() = default;

  void GetDatum() override {}
};

using UPProjSRS = std::unique_ptr<ProjSRS>;
using SPProjSRS = std::shared_ptr<ProjSRS>;
}
