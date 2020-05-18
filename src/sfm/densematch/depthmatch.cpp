/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sfm/densematch/depthmatch.h"

#include <algorithm>
#include <cmath>
#include <cassert>
#include <execution>
#include <omp.h>

using namespace watxtract;

/**
 *  \return Retruns x- and y-disparity maps.
 */
template <class CostFunction, class Neighborhood>
std::pair<std::unique_ptr<IRaster>, std::unique_ptr<IRaster>> DepthMatch<CostFunction, Neighborhood>::Match() {
  return { nullptr, nullptr };
}

template <class CostFunction, class Neighborhood>
void DepthMatch<CostFunction, Neighborhood>::Preprocess() {
}

template <class CostFunction, class Neighborhood>
void DepthMatch<CostFunction, Neighborhood>::CalculateCost() {
}

template <class CostFunction, class Neighborhood>
void DepthMatch<CostFunction, Neighborhood>::AggreagateCost() {
}

template <class CostFunction, class Neighborhood>
void DepthMatch<CostFunction, Neighborhood>::OptimizeDepth() {
}

template <class CostFunction, class Neighborhood>
void DepthMatch<CostFunction, Neighborhood>::Postprocess() {
}
