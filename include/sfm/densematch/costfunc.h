/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/icostfunc.h"
#include "sfm/densematch/identitylossfunc.h"
#include "sfm/densematch/normalizelossfunc.h"
#include "sfm/densematch/sigmoidlossfunc.h"

#include <type_traits>

namespace earthxpert {

/**
 *  The base class CostFunc implements the template methods used to calculate 
 *  cost/difference given a pixel location on one image and its disparity or depth.
 *
 *  \param LossFunc defines the behaviour of converting cost to loss
 */
template <class LossFunc = IdentityLossFunc>
class EXSFM_EXPORTS CostFunc : public ICostFunc {
  static_assert(std::is_base_of_v<ILossFunc, LossFunc>, "ILossFunc");

public:
  using PolicyType = LossFunc;

  CostFunc(const LossFunc& lossFunc) : mLossFunc(lossFunc) {}
  virtual ~CostFunc() = default;

  inline double CalcCost() const {
    auto cost = CalcCostImpl();

    return mLossFunc(cost);
  }

private:
  LossFunc mLossFunc;
};
}
