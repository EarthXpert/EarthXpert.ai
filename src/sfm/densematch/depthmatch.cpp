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

using namespace earthxpert;

/**
 *  \brief
 *
 *  \return Retruns the depth map where the element value represents the
 *  the depth of the corresonding pixel.
 */
template <class CostFunction, class Neighborhood>
std::unique_ptr<IRaster> DepthMatch<CostFunction, Neighborhood>::Match() {
  return nullptr;
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
