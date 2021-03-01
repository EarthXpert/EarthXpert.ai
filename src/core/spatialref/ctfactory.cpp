/* ----------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/spatialref/ctfactory.h"
#include "core/exceptions/exexcept.h"

#include <cassert>

using namespace earthxpert;

namespace {
  std::map<CtInfo, std::weak_ptr<ICt>> gCCsCTs;
  std::map<CtInfo, std::weak_ptr<ICt>> gOgrCTs;
  std::map<CtInfo, std::weak_ptr<ICt>> gProjCTs;
}

std::shared_ptr<ICt> CtFactory::CreateCt(const CtInfo* ctInfo) {
  const auto backend = ctInfo->srcSrs.backend;
  assert(backend == ctInfo->tgtSrs.backend);
  
  if (eSB_CCS == backend)
    return CreateCcsCt(ctInfo);
  else if (eSB_OGR == backend)
    return CreateOgrCt(ctInfo);
  else if (eSB_PROJ == backend)
    return CreateProjCt(ctInfo);
  else
    throw std::invalid_argument("ctInfo");
}

std::shared_ptr<CcsCt> CtFactory::CreateCcsCt(const CtInfo* ctInfo) {
  auto it = gCCsCTs.find(*ctInfo);
  if (gCCsCTs.end() != it) {
  }
  return {};
}

std::shared_ptr<OgrCt> CtFactory::CreateOgrCt(const CtInfo* ctInfo) {
  auto it = gOgrCTs.find(*ctInfo);
  if (gOgrCTs.end() != it) {
  }
  return {};
}

std::shared_ptr<ProjCt> CtFactory::CreateProjCt(const CtInfo* ctInfo) {
  auto it = gProjCTs.find(*ctInfo);
  if (gProjCTs.end() != it) {
  }
  return {};
}
