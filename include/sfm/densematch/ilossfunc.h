/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/exsfm.h"

namespace earthxpert {
/**
 *  The interface ILossFunc defines the math methods used to convert/activate
 *  cost/difference value to loss value.
 */
class EXSFM_EXPORTS ILossFunc {
public:
  // loss: cost activation functions
  enum LossType {
    eIDENTITY,   // identical, no transformation
    eNORMALIZE,  // offset and scale
    eSIGMOID     // sigmoid function
  };

  virtual ~ILossFunc() = default;

  virtual LossType GetLossType() const noexcept = 0;
};
}
