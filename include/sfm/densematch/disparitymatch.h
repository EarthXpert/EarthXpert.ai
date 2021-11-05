/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/densematch.h"
#include "epipolarmodel/iepipolarmodel.h"
#include "sensormodel/isensormodel.h"
#include "sfm/densematch/censuscostfunc.h"
#include "sfm/densematch/circleneighbor.h"
#include "sfm/densematch/disparitymatchtypes.h"
#include "sfm/densematch/micostfunc.h"
#include "sfm/densematch/rankcostfunc.h"
#include "sfm/densematch/rectneighbor.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <execution>
#include <omp.h>
#include <type_traits>

namespace earthxpert {
/**
 *  The class DisparityMatch implements a binocunar view semi-global matching algorithm.
 *  The algorithm computes the disparity map in image space. The input images determined
 *  the GeometryModel policy, respectively, for epipolar images with epipolar model, or
 *  for raw images with sensor models, or for ortho images with geocoding models.
 *  The latter also includes satellite OrthoReady images that are
 *  resampled epipolar images but with an RPC model.
 *
 *  \tparam GeometryModel defines the behaviour of geometry model of the imagery:
 *  epipolar model, sensor model, affine model
 *  \tparam CostFunction defines the behaviour of calculating cost
 *  \tparam Neighborhood defines the behaviour of neighborhood selection
 */
template <class GeometryModel, class CostFunc, class Neighborhood>
class EXSFM_EXPORTS DisparityMatch final : public DenseMatch {
  static_assert(std::is_base_of_v<IEpipolarModel, GeometryModel> || \
    std::is_base_of_v<ISensorModel, GeometryModel>, "IEpipolarModel or ISensorModel");
  static_assert(std::is_base_of_v<ICostFunc, CostFunc>, "ICostFunc");
  static_assert(std::is_base_of_v<INeighborhood, Neighborhood>, "INeighborhood");

public:
  using GeometryPolicyType = GeometryModel;
  using CostPolicyType = CostFunc;
  using NeighborPolicyType = Neighborhood;

  DisparityMatch();
  DisparityMatch(const DisparityMatch&) = delete;
  DisparityMatch(DisparityMatch&&) = delete;
  ~DisparityMatch() = default;

  DisparityMatch& operator=(const DisparityMatch&) = delete;
  DisparityMatch& operator=(DisparityMatch&&) = delete;

  MatchType GetMatchType() const noexcept override { return eDISPARITY; }

  bool ProcessCv();
  bool ProcessVxl();

private:
  std::vector<DisparityTile> mTiles;

  int LayoutTiles() override;
  int MatchTiles() override;
};
} // namespace earthxpert

#include "detail/disparitymatch.hh"
