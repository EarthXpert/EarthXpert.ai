/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/ilossfunc.h"

//#include <cmath>

namespace earthxpert {
// sigmoid type
constexpr int kST_ARCTAN = 0;
constexpr int kST_LOGISTIC = 1;

/**
 *  The class SigmoidLossFunc defines the math methods used to convert/activate
 *  cost/difference value to loss value.
 *
 *  \param sigmoidType type of the S-shape
 */
template <int sigmoidType = kST_LOGISTIC>
class EXSFM_EXPORTS SigmoidLossFunc final : public ILossFunc {
  static_assert(kST_ARCTAN <= sigmoidType && sigmoidType <= kST_LOGISTIC, "sigmoidType");

public:
  ~SigmoidLossFunc() = default;

  double operator()(const double cost) const {
    if constexpr (eST_ARCTAN == sigmoidType) {
      return atan(cost);
    }
    else {  //if constexpr (eST_LOGISTIC == sigmoidType)
      return 1.0 / (1.0 + exp(-cost));
    }
  }

  LossType GetLossType() const noexcept override {
    return eSIGMOID;
  }
};

using SigmoidLossFuncArctan = SigmoidLossFunc<kST_ARCTAN>;
using SigmoidLossFuncLogistic = SigmoidLossFunc<kST_LOGISTIC>;
}
