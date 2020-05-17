/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/icostfunction.h"

namespace watxtract {

/**
 *  The class CensusCostFunction defines the census method used to calculate cost/difference.
 */
class WXSFM_EXPORTS CensusCostFunction : public ICostFunction {
public:
  ~CensusCostFunction() = default;

  CostType GetCostType() const noexcept {
    return eCENSUS;
  }
};
}
