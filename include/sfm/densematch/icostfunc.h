/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/exsfm.h"

namespace earthxpert {

/**
 *  The interface ICostFunc defines the math methods used to calculate cost/difference
 *  given a pixel location on one image and its disparity against anohter image.
 */
class EXSFM_EXPORTS ICostFunc {
public:
  enum CostType {
    eAD,        // absolute difference
    eCENSUS,    // census
    eMI,        // mutual information
    eNCC,       // normalized cross correlation
    eRANK       // rank transform
  };

  virtual ~ICostFunc() = default;

  virtual CostType GetCostType() const noexcept = 0;

private:
  virtual double CalcCostImpl() const = 0;
};
}
