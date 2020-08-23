/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/spatialref/isrs.h"
#include "3rdparty/geotrans/exgeotrans.h"

#include <memory>

namespace earthxpert {
/**
 *  The class CcsSRS provides service of spatial coordinate system
 *  with Geotrans & dtcc as backend.
 */
class EXCORE_EXPORTS CcsSRS final : public ISRS
{
public:
  ~CcsSRS() = default;
};
}
