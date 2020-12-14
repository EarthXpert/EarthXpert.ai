/* ----------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/spatialref/srsfactory.h"

using namespace earthxpert;

static std::map<SrsInfo, std::weak_ptr<CcsSrs>> gCcsSrss;
static std::map<SrsInfo, std::weak_ptr<OgrSrs>> gOgrSrss;
static std::map<SrsInfo, std::weak_ptr<ProjSrs>> gProjSrss;

std::shared_ptr<ISrs> SrsFactory::CreateSrs() {
  return {};
}

std::shared_ptr<CcsSrs> SrsFactory::CreateCcsSrs() {
  return {};
}

std::shared_ptr<OgrSrs> SrsFactory::CreateOgrSrs() {
  return {};
}

std::shared_ptr<ProjSrs> SrsFactory::CreateProjSrs() {
  return {};
}
