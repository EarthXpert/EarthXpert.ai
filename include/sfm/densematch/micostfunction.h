/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/icostfunction.h"

namespace earthxpert {

/**
 *  The class MICostFunction defines the MI method used to calculate cost/difference.
 */
class EXSFM_EXPORTS MICostFunction : public ICostFunction {
public:
  ~MICostFunction() = default;

  CostType GetCostType() const noexcept {
    return eMI;
  }
};
}
