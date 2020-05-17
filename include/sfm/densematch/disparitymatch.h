/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/densematch.h"

namespace watxtract {

/**
 *  The class DenseMatch implements a binocunar view semi-global matching algorithm.
 *  The algorithm computes the disparity map in image space.
 *
 *  \param GeometryModel defines the behaviour of geometry model of the imagery: epipolar model, sensor model
 *  \param CostFunction defines the behaviour of calculating cost
 *  \param Neighborhood defines the behaviour of neighborhood selection
 */
template <class GeometryModel, class CostFunction, class Neighborhood>
class WXSFM_EXPORTS DisparityMatch final : public DenseMatch {
public:
  using GeometryType = GeometryModel;
  using CostPolicyType = CostFunction;
  using NeighborPolicyType = Neighborhood;

  ~DisparityMatch() = default;

  std::unique_ptr<IRaster> Match();

  MatchType GetMatchType() const noexcept {
    return eDISPARITY;
  }

private:
  void Preprocess();
  void CalculateCost();
  void AggreagateCost();
  void OptimizeDisparity();
  void Postprocess();
};
}
