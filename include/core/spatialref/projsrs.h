/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/spatialref/isrs.h"
#include "3rdparty/proj/exproj.h"

#include <memory>
#include <string>

namespace earthxpert {
/**
 *  The class ProjSrs provides service of spatial coordinate system
 *  with GDAL & OGR as backend.
 */
class EXCORE_EXPORTS ProjSrs final : public ISrs
{
public:
  explicit ProjSrs(const std::string& wkt);
  ~ProjSrs() = default;

  void GetDatum() override {}

  const PJ* GetPJ() const { return mSRS.get(); }

private:
  friend class ProjCT;

  UPPJ mSRS;
};

using UPProjSRS = std::unique_ptr<ProjSrs>;
using SPProjSRS = std::shared_ptr<ProjSrs>;
}
