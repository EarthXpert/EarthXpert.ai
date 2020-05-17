/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/wxsfm.h"

namespace watxtract {

/**
 *  The interface ICostFunction defines the math methods used to calculate cost/difference
 *  given a pixel location on one image and its disparity against anohter image.
 */
class WXSFM_EXPORTS ICostFunction {
public:
  enum CostType {
    eAD,        // absolute difference
    eCENSUS,    // census
    eMI,        // mutual information
    eNCC,       // normalized cross correlation
    eRANK       // rank transform
  };

  virtual ~ICostFunction() = default;

  virtual CostType GetCostType() const noexcept = 0;
};
}
