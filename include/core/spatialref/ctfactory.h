/* ----------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "core/spatialref/ccsct.h"
#include "core/spatialref/ogrct.h"
#include "core/spatialref/projct.h"
#include "core/spatialref/spatialdef.h"

namespace earthxpert {
class EXCORE_EXPORTS CTFactory {
public:
  static std::shared_ptr<ICT> CreateCT();

  static std::shared_ptr<OgrCT> CreateOgrCT();

  static std::shared_ptr<CcsCT> CreateCcsCT();

  static std::shared_ptr<ProjCT> CreateProjCT();
};
}
