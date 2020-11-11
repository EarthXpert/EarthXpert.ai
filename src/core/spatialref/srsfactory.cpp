/* ----------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/spatialref/srsfactory.h"

using namespace earthxpert;

static std::map<SRSInfo, std::weak_ptr<ISRS>> gSRSs;

std::shared_ptr<ISRS> SRSFactory::CreateSRS() {
  return {};
}

std::shared_ptr<GdalSRS> SRSFactory::CreateGdalSRS() {
  return {};
}

std::shared_ptr<CcsSRS> SRSFactory::CreateCcsSRS() {
  return {};
}
