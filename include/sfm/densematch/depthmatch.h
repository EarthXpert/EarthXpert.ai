/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/densematch.h"
#include "sfm/densematch/icostfunc.h"
#include "sfm/densematch/ineighborhood.h"

#include <algorithm>
#include <cmath>
#include <cassert>
#include <execution>
#include <omp.h>
#include <type_traits>

namespace earthxpert {

/**
 *  The class DepthMatch implements a multi-view semi-global matching algorithm.
 *  The algorithm computes the dpeth/elevation map directly in image space and
 *  is fullfilled simuteneously in ground space. The input images' geometry is
 *  represented by sensor models. A depth is considered valid iff the object point
 *  occured on N-view (N >= 2) and is validated.
 *
 *  \param CostFunc defines the behaviour of calculating cost
 *  \param Neighborhood defines the behaviour of neighborhood selection
 *
 *  \remark the terms depth and elevation are used interchangably here.
 */
template <class CostFunc, class Neighborhood>
class EXSFM_EXPORTS DepthMatch final : public DenseMatch{
  static_assert(std::is_base_of_v<ICostFunc, CostFunc>, "ICostFunc");
  static_assert(std::is_base_of_v<INeighborhood, Neighborhood>, "INeighborhood");

public:
  using CostPolicyType = CostFunc;
  using NeighborPolicyType = Neighborhood;

  ~DepthMatch() = default;

  /**
   *  \param [out] depth the depth map where the element value represents the
   *  the depth of the corresonding pixel.
   *
   *  \return Retruns true if successful, false otherwise.
   */
  bool Match(IRaster* depth) override {
    return true;
  }

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
} // namespace earthxpert
