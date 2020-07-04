/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/densematch.h"
#include "sfm/densematch/depthmatchtypes.h"
#include "sfm/densematch/icostfunc.h"
#include "sfm/densematch/ineighborhood.h"

#include <algorithm>
#include <cassert>
#include <cmath>
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
 *  \tparam CostFunc defines the behaviour of calculating cost
 *  \tparam Neighborhood defines the behaviour of neighborhood selection
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

  MatchType GetMatchType() const noexcept override {
    return eDEPTH;
  }

private:
  std::vector<DepthTile> mTiles;

  int LayoutTiles() override;
  int MatchTiles() override;
};
} // namespace earthxpert

#include "detail/depthmatch.hh"
