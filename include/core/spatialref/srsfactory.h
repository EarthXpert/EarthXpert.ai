/* ----------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "core/spatialref/ccssrs.h"
#include "core/spatialref/ogrsrs.h"
#include "core/spatialref/projsrs.h"
#include "core/spatialref/spatialdef.h"

namespace earthxpert {
class EXCORE_EXPORTS SrsFactory {
public:
  static std::shared_ptr<ISrs> CreateSrs();

  static std::shared_ptr<CcsSrs> CreateCcsSrs();

  static std::shared_ptr<OgrSrs> CreateOgrSrs();

  static std::shared_ptr<ProjSrs> CreateProjSrs();
};
}
