/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/spatialref/isrs.h"
#include "3rdparty/geotrans/exgeotrans.h"

namespace earthxpert {
/**
 *  The class CcsSrs provides service of spatial coordinate system
 *  with Geotrans & dtcc as backend.
 */
class EXCORE_EXPORTS CcsSrs final : public ISrs
{
public:
  ~CcsSrs() = default;

  void GetDatum() override {}

private:
  friend class CcsCT;
};

using UPCcsSrs = std::unique_ptr<CcsSrs>;
using SPCcsSrs = std::shared_ptr<CcsSrs>;
}
