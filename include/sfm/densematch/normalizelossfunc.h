/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/ilossfunc.h"

#include <cassert>

namespace earthxpert {

/**
 *  The class NormalizeLossFunc defines the math methods used to convert/activate
 *  cost/difference value to loss value.
 */
class EXSFM_EXPORTS NormalizeLossFunc final : public ILossFunc {
public:
  NormalizeLossFunc(const double offset = 0.0, const double scale = 1.0)
    : mOffset(offset), mScale(scale) {
    assert(0.0 != mScale);
    mInvScale = 1.0 / mScale;
  }
  ~NormalizeLossFunc() = default;

  void SetNormalFactors(const double offset, const double scale) {
    mOffset = offset;
    mScale = scale;
    mInvScale = 1.0 / mScale;
  }

  double operator()(const double cost) const {
    return (cost - mOffset) * mInvScale;
  }

  LossType GetLossType() const noexcept {
    return eNORMALIZE;
  }

private:
  double mOffset{};
  double mScale{ 1.0 };

  // cache
  double mInvScale{1.0};
}
