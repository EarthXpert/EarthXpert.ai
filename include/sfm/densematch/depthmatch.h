/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/densematch.h"

namespace watxtract {

/**
 *  The class DenseMatch implements a multi-view semi-global matching algorithm.
 *  The algorithm computes the dpeth/elevation map directly in image space and 
 *  is fullfilled simuteneously in ground space.
 *
 *  \param CostFunction defines the behaviour of calculating cost
 *  \param Neighborhood defines the behaviour of neighborhood selection
 */
template <class CostFunction, class Neighborhood>
class WXSFM_EXPORTS DepthMatch final : public DenseMatch {
public:
  using CostPolicyType = CostFunction;
  using NeighborPolicyType = Neighborhood;

  ~DepthMatch() = default;

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
