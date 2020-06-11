/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"

#include "core/math/graphfit.h"
#include <cmath>
#include <iostream>
#include <random>

using namespace earthxpert;
using namespace earthxpert::math;
using namespace ceres;

static constexpr auto rngMax = static_cast<double>(std::mt19937::max());
static constexpr auto scale = 1.0 / rngMax;

static std::vector<Point2d> ObtainLinePoints();
static std::vector<Point2d> ObtainCurvePoints();
static std::vector<Point3d> ObtainPlanePoints();

/**
 *  TestCase 1: fit straight line
 */
TEST(graphfit, FitCurve_T1) {
  auto points = ObtainLinePoints();

  GraphFit graphFit;
  Solver::Summary summary;

  // zero initial parameter valus
  std::vector<double> pb;
  std::cout << "Initial m: " << 0.0 << " c: " << 0.0 << "\n";

  graphFit.FitStraightLine(points, pb, summary);

  ASSERT_EQ(2, pb.size()) << "2 parameters";

  std::cout << summary.BriefReport() << "\n";
  std::cout << "Final   m: " << pb[0] << " c: " << pb[1] << "\n";

  // initial parameter valus estimated using the 1st & last points
  auto& p1 = points.front();
  auto& pn = points.back();
  pb[0] = (pn.y - p1.y) / (pn.x - p1.x);
  pb[1] = p1.y - pb[0] * p1.x;
  std::cout << "Initial m: " << pb[0] << " c: " << pb[1] << "\n";

  graphFit.FitStraightLine(points, pb, summary);

  std::cout << summary.BriefReport() << "\n";
  std::cout << "Final   m: " << pb[0] << " c: " << pb[1] << "\n";

  // known parameter valus
  pb[0] = 0.3;
  pb[1] = 0.1;
  std::cout << "Initial m: " << pb[0] << " c: " << pb[1] << "\n";

  graphFit.FitStraightLine(points, pb, summary);

  std::cout << summary.BriefReport() << "\n";
  std::cout << "Final   m: " << pb[0] << " c: " << pb[1] << "\n";
}

/**
 *  TestCase 2: fit exponetial curve
 */
TEST(graphfit, FitCurve_T2) {
  auto points = ObtainCurvePoints();

  GraphFit graphFit;
  Solver::Summary summary;

  // zero initial parameter valus
  std::vector<double> pb;
  std::cout << "Initial m: " << 0.0 << " c: " << 0.0 << "\n";

  graphFit.FitExponentialCurve(points, pb, summary);

  ASSERT_EQ(2, pb.size()) << "2 parameters";

  std::cout << summary.BriefReport() << "\n";
  std::cout << "Final   m: " << pb[0] << " c: " << pb[1] << "\n";

  // good initial parameter valus estimated using the 1st & last points
  auto& p1 = points.front();
  auto& pn = points.back();
  pb[0] = (log(pn.y) - log(p1.y)) / (pn.x - p1.x);
  pb[1] = log(p1.y) - pb[0] * p1.x;
  std::cout << "Initial m: " << pb[0] << " c: " << pb[1] << "\n";
  
  graphFit.FitExponentialCurve(points, pb, summary);

  std::cout << summary.BriefReport() << "\n";
  std::cout << "Final   m: " << pb[0] << " c: " << pb[1] << "\n";

  // known parameter valus
  pb[0] = 0.3;
  pb[1] = 0.1;
  std::cout << "Initial m: " << pb[0] << " c: " << pb[1] << "\n";

  graphFit.FitExponentialCurve(points, pb, summary);

  std::cout << summary.BriefReport() << "\n";
  std::cout << "Final   m: " << pb[0] << " c: " << pb[1] << "\n";
}

/**
 *  TestCase 3: fit plane
 */
TEST(graphfit, FitPlane_T3) {
  auto points = ObtainPlanePoints();

  GraphFit graphFit;
  Solver::Summary summary;

  // zero initial parameter valus
  std::vector<double> pb;
  std::cout << "Initial m: " << 0.0 << " n: " << 0.0 << " c: " << 0.0 << "\n";

  graphFit.FitPlane(points, pb, summary);

  ASSERT_EQ(3, pb.size()) << "3 parameters";

  std::cout << summary.BriefReport() << "\n";
  std::cout << "Final   m: " << pb[0] << " n: " << pb[1] << " c: " << pb[2] << "\n";

  // good initial parameter valus 
  pb[0] = 0.15;
  pb[1] = 0.3;
  pb[2] = 0.05;
  std::cout << "Initial m: " << pb[0] << " n: " << pb[1] << " c: " << pb[2] << "\n";

  graphFit.FitPlane(points, pb, summary);

  std::cout << summary.BriefReport() << "\n";
  std::cout << "Final   m: " << pb[0] << " n: " << pb[1] << " c: " << pb[2] << "\n";

  // known parameter valus
  pb[0] = 0.3;
  pb[1] = 0.6;
  pb[2] = 0.1;
  std::cout << "Initial m: " << pb[0] << " n: " << pb[1] << " c: " << pb[2] << "\n";

  graphFit.FitPlane(points, pb, summary);

  std::cout << summary.BriefReport() << "\n";
  std::cout << "Final   m: " << pb[0] << " n: " << pb[1] << " c: " << pb[2] << "\n";
}

/**
 *  Data generated using the following octave code.
 *
 *    randn('seed', 23497);
 *    m = 0.3;
 *    c = 0.1;
 *    x=[0:0.075:5];
 *    y = m * x + c;
 *    noise = randn(size(x)) * 0.05 - 0.025;
 *    y_observed = y + noise;
 *    data = [x', y_observed'];
 */
std::vector<Point2d> ObtainLinePoints() {
  std::vector<Point2d> points = {
    {0.00000, 0.10718}, {0.07500, 0.17365}, {0.15000, 0.15938}, {0.22500, 0.18492},
    {0.30000, 0.23575}, {0.37500, 0.23192}, {0.45000, 0.28691}, {0.52500, 0.31813},
    {0.60000, 0.28814}, {0.67500, 0.33008}, {0.75000, 0.35154}, {0.82500, 0.40829},
    {0.90000, 0.27496}, {0.97500, 0.38849}, {1.05000, 0.38707}, {1.12500, 0.47858},
    {1.20000, 0.44574}, {1.27500, 0.50313}, {1.35000, 0.44673}, {1.42500, 0.48960},
    {1.50000, 0.53269}, {1.57500, 0.56245}, {1.65000, 0.52757}, {1.72500, 0.64381},
    {1.80000, 0.58716}, {1.87500, 0.66554}, {1.95000, 0.71289}, {2.02500, 0.79064},
    {2.10000, 0.75351}, {2.17500, 0.73736}, {2.25000, 0.77701}, {2.32500, 0.76859},
    {2.40000, 0.89849}, {2.47500, 0.74050}, {2.55000, 0.86334}, {2.62500, 0.95177},
    {2.70000, 0.96702}, {2.77500, 0.92216}, {2.85000, 0.94506}, {2.92500, 0.89034},
    {3.00000, 1.03781}, {3.07500, 1.10663}, {3.15000, 1.10788}, {3.22500, 0.95841},
    {3.30000, 1.10071}, {3.37500, 1.16011}, {3.45000, 1.09336}, {3.52500, 1.20834},
    {3.60000, 1.16636}, {3.67500, 1.22615}, {3.75000, 1.27935}, {3.82500, 1.26691},
    {3.90000, 1.26247}, {3.97500, 1.27247}, {4.05000, 1.26996}, {4.12500, 1.35211},
    {4.20000, 1.39840}, {4.27500, 1.35769}, {4.35000, 1.41374}, {4.42500, 1.47176},
    {4.50000, 1.42453}, {4.57500, 1.47332}, {4.65000, 1.42056}, {4.72500, 1.53223},
    {4.80000, 1.55821}, {4.87500, 1.55178}, {4.95000, 1.53248} };

  for (auto& pt : points) {
    pt.y -= 0.025;
  }
  return points;
}

/**
 *  Data generated using the following octave code.
 *
 *    randn('seed', 23497);
 *    m = 0.3;
 *    c = 0.1;
 *    x=[0:0.075:5];
 *    y = exp(m * x + c);
 *    noise = randn(size(x)) * 0.2 - 0.1;
 *    y_observed = y + noise;
 *    data = [x', y_observed'];
 */
std::vector<Point2d> ObtainCurvePoints() {
  std::vector<Point2d> points = {
    {0.000000e+00, 1.133898e+00}, {7.500000e-02, 1.334902e+00}, {1.500000e-01, 1.213546e+00},
    {2.250000e-01, 1.252016e+00}, {3.000000e-01, 1.392265e+00}, {3.750000e-01, 1.314458e+00},
    {4.500000e-01, 1.472541e+00}, {5.250000e-01, 1.536218e+00}, {6.000000e-01, 1.355679e+00},
    {6.750000e-01, 1.463566e+00}, {7.500000e-01, 1.490201e+00}, {8.250000e-01, 1.658699e+00},
    {9.000000e-01, 1.067574e+00}, {9.750000e-01, 1.464629e+00}, {1.050000e+00, 1.402653e+00},
    {1.125000e+00, 1.713141e+00}, {1.200000e+00, 1.527021e+00}, {1.275000e+00, 1.702632e+00},
    {1.350000e+00, 1.423899e+00}, {1.425000e+00, 1.543078e+00}, {1.500000e+00, 1.664015e+00},
    {1.575000e+00, 1.732484e+00}, {1.650000e+00, 1.543296e+00}, {1.725000e+00, 1.959523e+00},
    {1.800000e+00, 1.685132e+00}, {1.875000e+00, 1.951791e+00}, {1.950000e+00, 2.095346e+00},
    {2.025000e+00, 2.361460e+00}, {2.100000e+00, 2.169119e+00}, {2.175000e+00, 2.061745e+00},
    {2.250000e+00, 2.178641e+00}, {2.325000e+00, 2.104346e+00}, {2.400000e+00, 2.584470e+00},
    {2.475000e+00, 1.914158e+00}, {2.550000e+00, 2.368375e+00}, {2.625000e+00, 2.686125e+00},
    {2.700000e+00, 2.712395e+00}, {2.775000e+00, 2.499511e+00}, {2.850000e+00, 2.558897e+00},
    {2.925000e+00, 2.309154e+00}, {3.000000e+00, 2.869503e+00}, {3.075000e+00, 3.116645e+00},
    {3.150000e+00, 3.094907e+00}, {3.225000e+00, 2.471759e+00}, {3.300000e+00, 3.017131e+00},
    {3.375000e+00, 3.232381e+00}, {3.450000e+00, 2.944596e+00}, {3.525000e+00, 3.385343e+00},
    {3.600000e+00, 3.199826e+00}, {3.675000e+00, 3.423039e+00}, {3.750000e+00, 3.621552e+00},
    {3.825000e+00, 3.559255e+00}, {3.900000e+00, 3.530713e+00}, {3.975000e+00, 3.561766e+00},
    {4.050000e+00, 3.544574e+00}, {4.125000e+00, 3.867945e+00}, {4.200000e+00, 4.049776e+00},
    {4.275000e+00, 3.885601e+00}, {4.350000e+00, 4.110505e+00}, {4.425000e+00, 4.345320e+00},
    {4.500000e+00, 4.161241e+00}, {4.575000e+00, 4.363407e+00}, {4.650000e+00, 4.161576e+00},
    {4.725000e+00, 4.619728e+00}, {4.800000e+00, 4.737410e+00}, {4.875000e+00, 4.727863e+00},
    {4.950000e+00, 4.669206e+00} };

  for (auto& pt : points) {
    pt.y -= 0.1;
  }
  return points;
}

/**
 *  Data generated using the following octave code.
 *
 *    randn('seed', 23497);
 *    m = 0.3;
 *    m = 0.6;
 *    c = 0.1;
 *    x=[0:0.5:5];
 *    y=[-2.5:0.5:2.5];
 *    z = m * x + n * y + c;
 *    noise = randn(size(x)) * 0.05 - 0.025;
 *    z_observed = z + noise;
 *    data = [x', y', z_observed'];
 */
std::vector<Point3d> ObtainPlanePoints() {
  std::vector<Point3d> points;

  const double m{ 0.3 };
  const double n{ 0.6 };
  const double c{ 0.1 };
  const double noise{ 0.08 };

  std::mt19937 rng;

  for (double x = 0.0; x <= 5.0; x += 0.5) {
    for (double y = -2.5; y <= 2.5; y += 0.5) {
      const double dz = noise * (rng() * scale - 0.5);
      const double z = m * x + n * y + c + dz;

      points.emplace_back<Point3d>({ x, y, z });
    }
  }

  return points;
}
