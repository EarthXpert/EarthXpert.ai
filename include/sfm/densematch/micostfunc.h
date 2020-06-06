/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/costfunc.h"

namespace earthxpert {
/**
 *  The class MICostFunc defines the MI method used to calculate cost/difference.
 *
 *  \param LossFunc defines the behaviour of converting cost to loss
 */
template <class LossFunc = IdentityLossFunc>
class EXSFM_EXPORTS MICostFunc final : public CostFunc<LossFunc> {
public:
  ~MICostFunc() = default;

  CostType GetCostType() const noexcept {
    return eMI;
  }

private:
  double CalcCostImpl() const {
    return 0;
  }
};
}
