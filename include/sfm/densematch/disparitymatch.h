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

namespace earthxpert {

/**
 *  The class DisparityMatch implements a binocunar view semi-global matching algorithm.
 *  The algorithm computes the disparity map in image space. The input images determined
 *  the GeometryModel policy, respectively, for epipolar images with epipolar model, or
 *  for raw images with sensor models. The latter also includes satellite OrthoReady images
 *  that are resampled epipolar images but with an RPC model.
 *
 *  \param GeometryModel defines the behaviour of geometry model of the imagery: epipolar model, sensor model
 *  \param CostFunction defines the behaviour of calculating cost
 *  \param Neighborhood defines the behaviour of neighborhood selection
 */
template <class GeometryModel, class CostFunction, class Neighborhood>
class EXSFM_EXPORTS DisparityMatch final : public DenseMatch {
public:
  using GeometryType = GeometryModel;
  using CostPolicyType = CostFunction;
  using NeighborPolicyType = Neighborhood;

  ~DisparityMatch() = default;

  /**
   *  \return Retruns the disparity map where the element value is the x-parallax
   *  (assuming a perfect epipolar), or the x- and y-parallax pair (e.g. Point2f).
   */
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
} // namespace earthxpert
