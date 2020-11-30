/* ----------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/spatialref/srsfactory.h"

using namespace earthxpert;

static std::map<SrsInfo, std::weak_ptr<CcsSRS>> gCcsSRSs;
static std::map<SrsInfo, std::weak_ptr<OgrSRS>> gOgrSRSs;
static std::map<SrsInfo, std::weak_ptr<ProjSRS>> gProjSRSs;

std::shared_ptr<ISRS> SrsFactory::CreateSRS() {
  return {};
}

std::shared_ptr<CcsSRS> SrsFactory::CreateCcsSRS() {
  return {};
}

std::shared_ptr<OgrSRS> SrsFactory::CreateOgrSRS() {
  return {};
}

std::shared_ptr<ProjSRS> SrsFactory::CreateProjSRS() {
  return {};
}
