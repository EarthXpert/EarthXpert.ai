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
class EXCORE_EXPORTS CtFactory {
public:
  static std::shared_ptr<ICt> CreateCt();

  static std::shared_ptr<CcsCt> CreateCcsCt();

  static std::shared_ptr<OgrCt> CreateOgrCt();

  static std::shared_ptr<ProjCt> CreateProjCt();
};
}
