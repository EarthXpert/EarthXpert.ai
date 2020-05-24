/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/icostfunction.h"

namespace earthxpert {

/**
 *  The class RankCostFunction defines the rank transform method used to calculate cost/difference.
 */
class EXSFM_EXPORTS RankCostFunction : public ICostFunction {
public:
  ~RankCostFunction() = default;

  CostType GetCostType() const noexcept {
    return eRANK;
  }
};
}
