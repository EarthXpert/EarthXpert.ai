/* ----------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/spatialref/ctfactory.h"

using namespace earthxpert;

static std::map<CTInfo, std::weak_ptr<ICT>> gCCsCTs;
static std::map<CTInfo, std::weak_ptr<ICT>> gOgrCTs;
static std::map<CTInfo, std::weak_ptr<ICT>> gProjCTs;

std::shared_ptr<ICT> CTFactory::CreateCT() {
  return {};
}

std::shared_ptr<OgrCT> CTFactory::CreateOgrCT() {
  return {};
}

std::shared_ptr<CcsCT> CTFactory::CreateCcsCT() {
  return {};
}

std::shared_ptr<ProjCT> CTFactory::CreateProjCT() {
  return {};
}
