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

#include <mutex>

namespace earthxpert {
class EXCORE_EXPORTS SrsFactory {
public:
  static std::shared_ptr<ISrs> CreateSrs(const SrsInfo* srsInfo);

  static std::shared_ptr<CcsSrs> CreateCcsSrs(const SrsInfo* srsInfo);

  static std::shared_ptr<OgrSrs> CreateOgrSrs(const SrsInfo* srsInfo);

  static std::shared_ptr<ProjSrs> CreateProjSrs(const SrsInfo* srsInfo);

private:
  inline static std::mutex mMtx;
};
}
