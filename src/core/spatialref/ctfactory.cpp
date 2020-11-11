/* ----------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/spatialref/ctfactory.h"

using namespace earthxpert;

static std::map<CTInfo, std::weak_ptr<ICT>> gCTs;

std::shared_ptr<ICT> CTFactory::CreateCT() {
  return {};
}

std::shared_ptr<GdalCT> CTFactory::CreateGdalCT() {
  return {};
}

std::shared_ptr<CcsCT> CTFactory::CreateCcsCT() {
  return {};
}
