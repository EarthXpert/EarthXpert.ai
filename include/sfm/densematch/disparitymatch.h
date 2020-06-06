/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/densematch.h"
#include "sfm/densematch/censuscostfunc.h"
#include "sfm/densematch/circleneighbor.h"
#include "sfm/densematch/micostfunc.h"
#include "sfm/densematch/rectneighbor.h"

#include <algorithm>
#include <cmath>
#include <cassert>
#include <execution>
#include <omp.h>
#include <type_traits>

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
template <class GeometryModel, class CostFunc, class Neighborhood>
class EXSFM_EXPORTS DisparityMatch final : public DenseMatch {
  static_assert(std::is_base_of_v<ICostFunc, CostFunc>, "ICostFunc");
  static_assert(std::is_base_of_v<INeighborhood, Neighborhood>, "INeighborhood");

public:
  using GeometryPolicyType = GeometryModel;
  using CostPolicyType = CostFunc;
  using NeighborPolicyType = Neighborhood;

  ~DisparityMatch() = default;

  /**
   *  \param [out] disparity the disparity map where the element value represents the
   *  the disparity of the corresonding pixel.
   *
   *  \return Retruns true if successful, false otherwise.
   */
  bool Match(IRaster* disparity) override {
    return true;
  }

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
