/* ----------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/spatialref/srsfactory.h"
#include "core/exceptions/exexcept.h"

using namespace earthxpert;

static std::map<SrsInfo, std::weak_ptr<CcsSrs>> gCcsSrss;
static std::map<SrsInfo, std::weak_ptr<OgrSrs>> gOgrSrss;
static std::map<SrsInfo, std::weak_ptr<ProjSrs>> gProjSrss;

std::shared_ptr<ISrs> SrsFactory::CreateSrs(const SrsInfo* srsInfo) {
  if (eSB_CCS == srsInfo->backend)
    return CreateCcsSrs(srsInfo);
  else if (eSB_OGR == srsInfo->backend)
    return CreateOgrSrs(srsInfo);
  else if (eSB_PROJ == srsInfo->backend)
    return CreateProjSrs(srsInfo);
  else
    throw std::invalid_argument("srsInfo");
}

std::shared_ptr<CcsSrs> SrsFactory::CreateCcsSrs(const SrsInfo* srsInfo) {
  auto it = gCcsSrss.find(*srsInfo);
  if (gCcsSrss.end() != it) {
  }
  return {};
}

std::shared_ptr<OgrSrs> SrsFactory::CreateOgrSrs(const SrsInfo* srsInfo) {
  auto it = gOgrSrss.find(*srsInfo);
  if (gOgrSrss.end() != it) {
  }
  return {};
}

std::shared_ptr<ProjSrs> SrsFactory::CreateProjSrs(const SrsInfo* srsInfo) {
  auto it = gProjSrss.find(*srsInfo);
  if (gProjSrss.end() != it) {
  }
  return {};
}
