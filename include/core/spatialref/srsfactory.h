/* ----------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "core/spatialref/ccssrs.h"
#include "core/spatialref/ogrsrs.h"
#include "core/spatialref/spatialdef.h"

namespace earthxpert {
class EXCORE_EXPORTS SrsFactory {
public:
  static std::shared_ptr<ISRS> CreateSRS();

  static std::shared_ptr<OgrSRS> CreateOgrSRS();

  static std::shared_ptr<CcsSRS> CreateCcsSRS();
};
}
