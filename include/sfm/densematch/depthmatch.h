/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/densematch.h"
#include "sfm/densematch/censuscostfunction.h"
#include "sfm/densematch/circleneighbor.h"
#include "sfm/densematch/micostfunction.h"
#include "sfm/densematch/rectneighbor.h"

namespace watxtract {

/**
 *  The class DepthMatch implements a multi-view semi-global matching algorithm.
 *  The algorithm computes the dpeth/elevation map directly in image space and 
 *  is fullfilled simuteneously in ground space. The input images' geometry is
 *  represented by sensor models. A depth is considered valid iff the object point 
 *  occured on N-view (N is 2+) and is validated.
 *
 *  \param CostFunction defines the behaviour of calculating cost
 *  \param Neighborhood defines the behaviour of neighborhood selection
 *
 *  \remark the terms depth and elevation are used interchangably here.
 */
template <class CostFunction, class Neighborhood>
class WXSFM_EXPORTS DepthMatch final : public DenseMatch {
public:
  using CostPolicyType = CostFunction;
  using NeighborPolicyType = Neighborhood;

  ~DepthMatch() = default;

  /**
   *  \return Retruns the depth map where the element value represents the 
   *  the depth of the corresonding pixel.
   */
  std::unique_ptr<IRaster> Match();

  MatchType GetMatchType() const noexcept {
    return eDEPTH;
  }

private:
  void Preprocess();
  void CalculateCost();
  void AggreagateCost();
  void OptimizeDepth();
  void Postprocess();
};
}
