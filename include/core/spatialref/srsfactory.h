/* ----------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "core/spatialref/ccssrs.h"
#include "core/spatialref/gdalsrs.h"
#include "core/spatialref/spatialdef.h"

namespace earthxpert {
class EXCORE_EXPORTS SRSFactory {
public:
  static std::shared_ptr<ISRS> CreateSRS();

  static std::shared_ptr<GdalSRS> CreateGdalSRS();

  static std::shared_ptr<CcsSRS> CreateCcsSRS();
};
}
