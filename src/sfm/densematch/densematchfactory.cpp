/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sfm/densematch/densematchfactory.h"
#include "sfm/densematch/depthmatch.h"
#include "sfm/densematch/disparitymatch.h"

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
  return nullptr;
}

/**
 *  \brief
 */
std::unique_ptr<IDenseMatch> DenseMatchFactory::CreateDisparityMatch() {
  return nullptr;
}
