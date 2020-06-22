/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/sltbench/sltbench_main.h"
#include "testconfig.h"

#include "core/math/graphfit.h"
#include <cmath>
#include <iostream>
#include <omp.h>
#include <random>

using namespace earthxpert;

using namespace earthxpert;
using namespace earthxpert::math;
using namespace ceres;

static constexpr auto rngMax = static_cast<double>(std::mt19937::max());
static constexpr auto scale = 1.0 / rngMax;

static std::vector<Point2d> ObtainLinePoints();

/**
 *  TestCase 0: timing points population
 */
void GraphFit_Points() {
  auto points = ObtainLinePoints();
}

/**
 *  TestCase 1: fit line using Ceres Solver
 */
void GraphFit_Line_Ceres() {
  auto points = ObtainLinePoints();

  GraphFit graphFit;
  Solver::Summary summary;

  std::vector<double> pb;  // zero initial parameter valus
  for (int i = 0; i < 1000; ++i)
    graphFit.FitLineCeres(points, pb, summary);
}

/**
 *  TestCase 2: fit line using Eigen Solver
 */
void GraphFit_Line_Eigen() {
  auto points = ObtainLinePoints();

  GraphFit graphFit;

  std::vector<double> pb;  // zero initial parameter valus
  for (int i = 0; i < 1000; ++i)
    graphFit.FitLineEigen(points, pb);
}

/**
 *  TestCase 3: fit line using Ceres Solver
 */
void GraphFit_Line_CeresOMP() {
  auto points = ObtainLinePoints();

  GraphFit graphFit;
  
#pragma omp parallel num_threads(4)
  {
    Solver::Summary summary;
    std::vector<double> pb; // zero initial parameter valus

#pragma omp for
    for (int i = 0; i < 1000; ++i) {
      graphFit.FitLineCeres(points, pb, summary);
    }
  }
}

/**
 *  TestCase 4: fit line using Eigen Solver
 */
void GraphFit_Line_EigenOMP() {
  auto points = ObtainLinePoints();

  GraphFit graphFit;
 
#pragma omp parallel num_threads(4)
  {
    std::vector<double> pb; // zero initial parameter valus

#pragma omp for
    for (int i = 0; i < 1000; ++i) {
      graphFit.FitLineEigen(points, pb);
    }
  }
}

/**
 *  Data generated using the following octave code.
 *
 *    randn('seed', 23497);
 *    m = 0.3;
 *    c = 0.1;
 *    x=[0:0.05:5];
 *    y = m * x + c;
 *    noise = randn(size(x)) * 0.05 - 0.025;
 *    y_observed = y + noise;
 *    data = [x', y_observed'];
 */
std::vector<Point2d> ObtainLinePoints() {
  std::mt19937 rng;
  std::vector<Point2d> points;

  const double m{ 0.3 };
  const double c{ 0.1 };

  for (double x = 0.0; x <= 5.0; x += 0.25) {
    const double dy = 0.05 * (rng() * scale - 0.5);
    const double y = m * x + c + dy;

    points.emplace_back<Point2d>({ x, y });
  }
  return points;
}

SLTBENCH_FUNCTION(GraphFit_Points);
SLTBENCH_FUNCTION(GraphFit_Line_Ceres);
SLTBENCH_FUNCTION(GraphFit_Line_Eigen);
SLTBENCH_FUNCTION(GraphFit_Line_CeresOMP);
SLTBENCH_FUNCTION(GraphFit_Line_EigenOMP);
