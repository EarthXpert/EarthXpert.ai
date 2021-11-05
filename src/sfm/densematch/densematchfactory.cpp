/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sfm/densematch/densematchfactory.h"
#include "epipolarmodel/epipolarmodel.h"
#include "sensormodel/framemodel.h"
#include "sensormodel/rationalmodel.h"
#include "sfm/densematch/censuscostfunc.h"
#include "sfm/densematch/circleneighbor.h"
#include "sfm/densematch/depthmatch.h"
#include "sfm/densematch/disparitymatch.h"
#include "sfm/densematch/micostfunc.h"
#include "sfm/densematch/rankcostfunc.h"
#include "sfm/densematch/rectneighbor.h"

#include <memory>

using namespace earthxpert;

/**
 *  \brief
 */
std::unique_ptr<IDenseMatch> DenseMatchFactory::CreateDenseMatch() {
  return nullptr;
}

/**
 *  \brief
 */
std::unique_ptr<IDenseMatch> DenseMatchFactory::CreateDepthMatch() {
  auto matcher = std::make_unique<DepthMatch<CensusCostFunc<>, CircleNeighbor>>();
  return matcher;
}

/**
 *  \brief
 */
std::unique_ptr<IDenseMatch> DenseMatchFactory::CreateDisparityMatch() {
  return nullptr;
}

/**
 *  \brief
 */
std::unique_ptr<IDenseMatch> DenseMatchFactory::CreateEpipolarDisparityMatch() {
  auto matcher = std::make_unique<DisparityMatch<GeoEpipolarModel,
    MICostFunc<>, CircleNeighbor>>();
  return matcher;
}

/**
 *  \brief
 */
std::unique_ptr<IDenseMatch> DenseMatchFactory::CreateSensorDisparityMatch() {
  auto matcher = std::make_unique<DisparityMatch<RationalModel, 
    MICostFunc<>, CircleNeighbor>>();
  return matcher;
}
