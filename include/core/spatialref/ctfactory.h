/* ----------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "core/spatialref/ccsct.h"
#include "core/spatialref/gdalct.h"
#include "core/spatialref/spatialdef.h"

namespace earthxpert {
class EXCORE_EXPORTS CTFactory {
public:
  static std::shared_ptr<ICT> CreateCT();

  static std::shared_ptr<GdalCT> CreateGdalCT();

  static std::shared_ptr<CcsCT> CreateCcsCT();
};
}
