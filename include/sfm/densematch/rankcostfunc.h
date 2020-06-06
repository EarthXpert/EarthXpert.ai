/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/costfunc.h"

namespace earthxpert {

/**
 *  The class RankCostFunc defines the rank transform method used to calculate cost/difference.
 *
 *  \param LossFunc defines the behaviour of converting cost to loss
 */
template <class LossFunc = IdentityLossFunc>
class EXSFM_EXPORTS RankCostFunc final : public CostFunc<LossFunc>{
public:
  ~RankCostFunc() = default;

  CostType GetCostType() const noexcept override {
    return eRANK;
  }

private:
  double CalcCostImpl() const override {
    return 0;
  }
};
}
