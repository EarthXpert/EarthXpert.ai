/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sfm/densematch/disparitymatch.h"

#include <algorithm>
#include <cmath>
#include <cassert>
#include <execution>
#include <omp.h>

using namespace watxtract;

/**
 *  \return Retruns x- and y-disparity maps.
 */
template <class GeometryModel, class CostFunction, class Neighborhood>
std::pair<std::unique_ptr<IRaster>, std::unique_ptr<IRaster>> DisparityMatch<GeometryModel, CostFunction, Neighborhood>::Match() {
  Preprocess();
  CalculateCost();
  AggreagateCost();
  OptimizeDisparity();
  Postprocess();

  return { nullptr, nullptr };
}

template <class GeometryModel, class CostFunction, class Neighborhood>
void DisparityMatch<GeometryModel, CostFunction, Neighborhood>::Preprocess() {
}

template <class GeometryModel, class CostFunction, class Neighborhood>
void DisparityMatch<GeometryModel, CostFunction, Neighborhood>::CalculateCost() {
}

template <class GeometryModel, class CostFunction, class Neighborhood>
void DisparityMatch<GeometryModel, CostFunction, Neighborhood>::AggreagateCost() {
}

template <class GeometryModel, class CostFunction, class Neighborhood>
void DisparityMatch<GeometryModel, CostFunction, Neighborhood>::OptimizeDisparity() {
}

template <class GeometryModel, class CostFunction, class Neighborhood>
void DisparityMatch<GeometryModel, CostFunction, Neighborhood>::Postprocess() {
}
