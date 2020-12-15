/* ----------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/spatialref/ctfactory.h"

using namespace earthxpert;

static std::map<CtInfo, std::weak_ptr<ICt>> gCCsCTs;
static std::map<CtInfo, std::weak_ptr<ICt>> gOgrCTs;
static std::map<CtInfo, std::weak_ptr<ICt>> gProjCTs;

std::shared_ptr<ICt> CtFactory::CreateCt() {
  return {};
}

std::shared_ptr<CcsCt> CtFactory::CreateCcsCt() {
  return {};
}

std::shared_ptr<OgrCt> CtFactory::CreateOgrCt() {
  return {};
}

std::shared_ptr<ProjCt> CtFactory::CreateProjCt() {
  return {};
}
