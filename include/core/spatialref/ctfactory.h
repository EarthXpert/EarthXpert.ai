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

#include <mutex>

namespace earthxpert {
/**
 *  \brief monostate factory to create coordinate transformation objects.
 */  
class EXCORE_EXPORTS CtFactory {
public:
  static std::shared_ptr<ICt> CreateCt(const CtInfo* ctInfo);

  static std::shared_ptr<CcsCt> CreateCcsCt(const CtInfo* ctInfo);

  static std::shared_ptr<OgrCt> CreateOgrCt(const CtInfo* ctInfo);

  static std::shared_ptr<ProjCt> CreateProjCt(const CtInfo* ctInfo);

private:
  inline static std::mutex mMtx;
};
}
