/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/costfunc.h"

namespace earthxpert {
/**
 *  The class CensusCostFunc defines the census method used to calculate cost/difference.
 *
 *  \tparam LossFunc defines the behaviour of converting cost to loss
 */
template <class LossFunc = IdentityLossFunc>
class EXSFM_EXPORTS CensusCostFunc final : public CostFunc<LossFunc> {
public:
  ~CensusCostFunc() = default;

  CostType GetCostType() const noexcept override {
    return eCENSUS;
  }

private:
  double CalcCostImpl() const override {
    return 0;
  }
};
}
