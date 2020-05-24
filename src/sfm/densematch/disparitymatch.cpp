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

using namespace earthxpert;

/**
 *  \brief
 *
 *  \return Retruns the disparity map where the element value is the x-parallax
 *  (assuming a perfect epipolar), or the x- and y-parallax pair (e.g. Point2f).
 */
template <class GeometryModel, class CostFunction, class Neighborhood>
std::unique_ptr<IRaster> DisparityMatch<GeometryModel, CostFunction, Neighborhood>::Match() {
  Preprocess();
  CalculateCost();
  AggreagateCost();
  OptimizeDisparity();
  Postprocess();

  return nullptr;
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
