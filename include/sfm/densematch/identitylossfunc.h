/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/ilossfunc.h"

namespace earthxpert {

/**
 *  The interface ILossFunc defines the math methods used to convert/activate
 *  cost/difference value to loss value.
 */
class EXSFM_EXPORTS IdentityLossFunc final : public ILossFunc {
public:
  ~IdentityLossFunc() = default;

  double operator()(const double cost) const {
    return cost;
  }

  LossType GetLossType() const noexcept override {
    return eIDENTITY;
  }
};
}
