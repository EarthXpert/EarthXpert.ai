/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/stl/exstl.h"
#include "testconfig.h"

#include "core/math/graphfit.h"
#include <cmath>
#include <iostream>
#include <random>

using namespace earthxpert;
using namespace earthxpert::math;
using namespace ceres;

constexpr auto kScale = 1.0 / static_cast<double>(std::mt19937::max());

std::vector<Point2d> ObtainLinePoints();
std::vector<Point3d> ObtainLinePoints3D();
std::vector<Point2d> ObtainCurvePoints();
std::vector<Point3d> ObtainPlanePoints();

/**
 *  TestCase 1: fit 2D straight line
 *  \todo examine the final parameter values
 */
TEST(graphfit, FitLine_Ceres) {
  auto points = ObtainLinePoints();

  GraphFit graphFit;
  Solver::Summary summary;

  // zero initial parameter valus
  std::vector<double> pb;
  //std::cout << "Initial m: " << 0.0 << " c: " << 0.0 << "\n";

  graphFit.FitLineCeres(points, pb, summary);
  ASSERT_TRUE(summary.IsSolutionUsable());
  ASSERT_EQ(2, pb.size()) << "2 parameters";
  
  //std::cout << summary.BriefReport() << "\n";
  //std::cout << "Final   m: " << pb[0] << " c: " << pb[1] << "\n";

  // initial parameter valus estimated using the 1st & last points
  auto& p1 = points.front();
  auto& pn = points.back();
  pb[0] = (pn.y - p1.y) / (pn.x - p1.x);
  pb[1] = p1.y - pb[0] * p1.x;
  //std::cout << "Initial m: " << pb[0] << " c: " << pb[1] << "\n";

  graphFit.FitLineCeres(points, pb, summary);
  ASSERT_TRUE(summary.IsSolutionUsable());

  //std::cout << summary.BriefReport() << "\n";
  //std::cout << "Final   m: " << pb[0] << " c: " << pb[1] << "\n";

  // known parameter valus
  pb[0] = 0.3;
  pb[1] = 0.1;
  //std::cout << "Initial m: " << pb[0] << " c: " << pb[1] << "\n";

  graphFit.FitLineCeres(points, pb, summary);
  ASSERT_TRUE(summary.IsSolutionUsable());

  //std::cout << summary.BriefReport() << "\n";
  //std::cout << "Final   m: " << pb[0] << " c: " << pb[1] << "\n";
}

/**
 *  TestCase 2: fit 3D straight line
 */
TEST(graphfit, FitLine3D) {
  const auto points = ObtainLinePoints3D();
  const auto n = points.size();

  Point3d p0 = points[0];
  Point3d v = (points[n-1] - points[0]);
  Point3d nv = v / sqrt(v.ddot(v));

  double dsq{};   // sum of squared errors
  for (auto& p : points) {
    const Point3d dp = p - p0;
    const double dn = nv.ddot(dp);
    dsq += dp.ddot(dp) - dn * dn;
  }

  const double rmse = sqrt(dsq / points.size());
  //std::cout << "FitLine3D RMSE: " << rmse << std::endl;
  ASSERT_LE(rmse, 1.1e-3) << "FitLine3D RMSE";
}

TEST(graphfit, FitLine3D_Cv) {
  auto points = ObtainLinePoints3D();

  GraphFit graphFit;

  cv::Vec6d pb;
  graphFit.FitLineCv(points, pb);
  //std::cout << "FitLine3D: " << pb << std::endl;

  Point3d p0{ pb[3], pb[4], pb[5] };
  Point3d nv{ pb[0], pb[1], pb[2] };

  double dsq{};   // sum of squared errors
  for (auto& p : points) {
    const Point3d dp = p - p0;
    const double dn = nv.ddot(dp);
    dsq += dp.ddot(dp) - dn * dn;
  }

  const double rmse = sqrt(dsq / points.size());
  //std::cout << "FitLine3D RMSE: " << rmse << std::endl;
}

/**
 *  \todo fit 3D line not plane
 */
TEST(graphfit, FitLine3D_Eigen) {
  auto points = ObtainLinePoints3D();

  GraphFit graphFit;

  std::vector<double> pb;
  graphFit.FitLineEigen(points, pb);  
  //std::cout << "FitLine3D: " << pb << std::endl;

  double dsq{};   // sum of squared errors
  for (auto& p : points) {
    const double dz = pb[0] * p.x + pb[1] * p.y + pb[2] - p.z;
    dsq += dz * dz;
  }

  const double rmse = sqrt(dsq / points.size());
  //std::cout << "FitLine3D RMSE: " << rmse << std::endl;
  ASSERT_LE(rmse, 1.0e-4) << "FitLine3D RMSE";
}

/**
 *  TestCase 3: fit 2D exponetial curve
 */
TEST(graphfit, FitCurve_Ceres) {
  auto points = ObtainCurvePoints();

  GraphFit graphFit;
  Solver::Summary summary;

  // zero initial parameter valus
  std::vector<double> pb;
  //std::cout << "Initial m: " << 0.0 << " c: " << 0.0 << "\n";

  graphFit.FitCurveCeres(points, pb, summary);
  ASSERT_TRUE(summary.IsSolutionUsable());
  ASSERT_EQ(2, pb.size()) << "2 parameters";

  //std::cout << summary.BriefReport() << "\n";
  //std::cout << "Final   m: " << pb[0] << " c: " << pb[1] << "\n";

  // good initial parameter valus estimated using the 1st & last points
  auto& p1 = points.front();
  auto& pn = points.back();
  pb[0] = (log(pn.y) - log(p1.y)) / (pn.x - p1.x);
  pb[1] = log(p1.y) - pb[0] * p1.x;
  //std::cout << "Initial m: " << pb[0] << " c: " << pb[1] << "\n";
  
  graphFit.FitCurveCeres(points, pb, summary);
  ASSERT_TRUE(summary.IsSolutionUsable());

  //std::cout << summary.BriefReport() << "\n";
  //std::cout << "Final   m: " << pb[0] << " c: " << pb[1] << "\n";

  // known parameter valus
  pb[0] = 0.3;
  pb[1] = 0.1;
  //std::cout << "Initial m: " << pb[0] << " c: " << pb[1] << "\n";

  graphFit.FitCurveCeres(points, pb, summary);
  ASSERT_TRUE(summary.IsSolutionUsable());

  //std::cout << summary.BriefReport() << "\n";
  //std::cout << "Final   m: " << pb[0] << " c: " << pb[1] << "\n";
}

/**
 *  TestCase 4: fit 2D line by Eigen Solver
 */
TEST(graphfit, FitLine_Eigen) {
  auto points = ObtainLinePoints();

  GraphFit graphFit;

  // zero initial parameter valus
  std::vector<double> pb;
  //std::cout << "Initial m: " << 0.0 << " n: " << 0.0 << " c: " << 0.0 << "\n";

  graphFit.FitLineEigen(points, pb);
  ASSERT_EQ(2, pb.size()) << "2 parameters";

  //std::cout << "Final   m: " << pb[0] << " c: " << pb[1] << "\n";
}

/**
 *  TestCase 5: fit 3D plane
 */
TEST(graphfit, FitPlane_Ceres) {
  auto points = ObtainPlanePoints();

  GraphFit graphFit;
  Solver::Summary summary;

  // zero initial parameter valus
  std::vector<double> pb;
  //std::cout << "Initial m: " << 0.0 << " n: " << 0.0 << " c: " << 0.0 << "\n";

  graphFit.FitPlaneCeres(points, pb, summary);
  ASSERT_TRUE(summary.IsSolutionUsable());
  ASSERT_EQ(3, pb.size()) << "3 parameters";

  //std::cout << summary.BriefReport() << "\n";
  //std::cout << "Final   m: " << pb[0] << " n: " << pb[1] << " c: " << pb[2] << "\n";

  // good initial parameter valus 
  pb[0] = 0.15;
  pb[1] = 0.3;
  pb[2] = 0.05;
  //std::cout << "Initial m: " << pb[0] << " n: " << pb[1] << " c: " << pb[2] << "\n";

  graphFit.FitPlaneCeres(points, pb, summary);
  ASSERT_TRUE(summary.IsSolutionUsable());

  //std::cout << summary.BriefReport() << "\n";
  //std::cout << "Final   m: " << pb[0] << " n: " << pb[1] << " c: " << pb[2] << "\n";

  // known parameter valus
  pb[0] = 0.3;
  pb[1] = 0.6;
  pb[2] = 0.1;
  //std::cout << "Initial m: " << pb[0] << " n: " << pb[1] << " c: " << pb[2] << "\n";

  graphFit.FitPlaneCeres(points, pb, summary);
  ASSERT_TRUE(summary.IsSolutionUsable());

  //std::cout << summary.BriefReport() << "\n";
  //std::cout << "Final   m: " << pb[0] << " n: " << pb[1] << " c: " << pb[2] << "\n";
}

/**
 *  Data generated using the following octave code.
 *
 *    randn('seed', 23497);
 *    m = 0.3;
 *    c = 0.1;
 *    x=[0:0.075:5];
 *    y = m * x + c;
 *    noise = randn(size(x)) * 0.05;
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

// SAR RPC
std::vector<Point3d> ObtainLinePoints3D() {
  std::vector<Point3d> points = {
    {-8839.318481, 7769.793985, -511.854990}, {-8842.822459, 7772.924984, -501.843643},
    {-8846.326438, 7776.055982, -491.832299}, {-8849.830415, 7779.186981, -481.820959},
    {-8853.334393, 7782.317979, -471.809622}, {-8856.838371, 7785.448977, -461.798289},
    {-8860.342348, 7788.579974, -451.786959}, {-8863.846326, 7791.710971, -441.775633},
    {-8867.350303, 7794.841968, -431.764309}, {-8870.854280, 7797.972965, -421.752990},
    {-8874.358257, 7801.103961, -411.741674}, {-8877.862234, 7804.234957, -401.730361},
    {-8881.366211, 7807.365953, -391.719052}, {-8884.870188, 7810.496949, -381.707746},
    {-8888.374164, 7813.627944, -371.696443}, {-8891.878141, 7816.758939, -361.685144},
    {-8895.382117, 7819.889933, -351.673848}, {-8898.886093, 7823.020928, -341.662556},
    {-8902.390069, 7826.151922, -331.651268}, {-8905.894045, 7829.282915, -321.639982},
    {-8909.398021, 7832.413909, -311.628700}, {-8912.901997, 7835.544902, -301.617422},
    {-8916.405972, 7838.675894, -291.606147}, {-8919.909948, 7841.806887, -281.594875},
    {-8923.413923, 7844.937879, -271.583607}, {-8926.917898, 7848.068871, -261.572342},
    {-8930.421873, 7851.199862, -251.561081}, {-8933.925848, 7854.330853, -241.549823},
    {-8937.429823, 7857.461844, -231.538569}, {-8940.933798, 7860.592835, -221.527318},
    {-8944.437773, 7863.723825, -211.516070}, {-8947.941748, 7866.854815, -201.504826},
    {-8951.445722, 7869.985804, -191.493586}, {-8954.949696, 7873.116794, -181.482348},
    {-8958.453671, 7876.247782, -171.471115}, {-8961.957645, 7879.378771, -161.459884},
    {-8965.461619, 7882.509759, -151.448657}, {-8968.965593, 7885.640747, -141.437434},
    {-8972.469567, 7888.771735, -131.426214}, {-8975.973541, 7891.902722, -121.414997},
    {-8979.477514, 7895.033709, -111.403784}, {-8982.981488, 7898.164695, -101.392574},
    {-8986.485461, 7901.295681, -91.381368},  {-8989.989435, 7904.426667, -81.370165},
    {-8993.493408, 7907.557652, -71.358965},  {-8996.997381, 7910.688638, -61.347769},
    {-9000.501355, 7913.819622, -51.336577},  {-9004.005328, 7916.950607, -41.325387},
    {-9007.509300, 7920.081591, -31.314202},  {-9011.013273, 7923.212574, -21.303019},
    {-9014.517246, 7926.343558, -11.291840},  {-9018.021219, 7929.474540, -1.280665 },
    {-9021.525191, 7932.605523, 8.730507 }, {-9025.029164, 7935.736505, 18.741676},
    {-9028.533136, 7938.867487, 28.752841}, {-9032.037109, 7941.998468, 38.764003},
    {-9035.541081, 7945.129450, 48.775161}, {-9039.045053, 7948.260430, 58.786316},
    {-9042.549025, 7951.391411, 68.797467}, {-9046.052997, 7954.522390, 78.808615},
    {-9049.556969, 7957.653370, 88.819760}, {-9053.060941, 7960.784349, 98.830901},
    {-9056.564913, 7963.915328, 108.842039}, {-9060.068884, 7967.046306, 118.853173},
    {-9063.572856, 7970.177284, 128.864304}, {-9067.076827, 7973.308262, 138.875431},
    {-9070.580799, 7976.439239, 148.886555}, {-9074.084770, 7979.570216, 158.897676},
    {-9077.588742, 7982.701192, 168.908793}, {-9081.092713, 7985.832168, 178.919906},
    {-9084.596684, 7988.963144, 188.931017}, {-9088.100655, 7992.094119, 198.942123},
    {-9091.604626, 7995.225094, 208.953227}, {-9095.108597, 7998.356068, 218.964327},
    {-9098.612568, 8001.487042, 228.975423}, {-9102.116539, 8004.618016, 238.986516},
    {-9105.620510, 8007.748989, 248.997606}, {-9109.124480, 8010.879962, 259.008692},
    {-9112.628451, 8014.010934, 269.019775}, {-9116.132421, 8017.141906, 279.030854},
    {-9119.636392, 8020.272877, 289.041930}, {-9123.140362, 8023.403848, 299.053003},
    {-9126.644333, 8026.534819, 309.064072}, {-9130.148303, 8029.665789, 319.075137},
    {-9133.652273, 8032.796759, 329.086200}, {-9137.156244, 8035.927728, 339.097258},
    {-9140.660214, 8039.058697, 349.108314}, {-9144.164184, 8042.189666, 359.119366},
    {-9147.668154, 8045.320634, 369.130414}, {-9151.172124, 8048.451601, 379.141459},
    {-9154.676094, 8051.582568, 389.152501}, {-9158.180064, 8054.713535, 399.163539},
    {-9161.684033, 8057.844501, 409.174574}, {-9165.188003, 8060.975467, 419.185605},
    {-9168.691973, 8064.106432, 429.196633}, {-9172.195943, 8067.237397, 439.207657},
    {-9175.699912, 8070.368362, 449.218678}, {-9179.203882, 8073.499326, 459.229696},
    {-9182.707851, 8076.630289, 469.240710}, {-9186.211821, 8079.761252, 479.251721},
    {-9189.715790, 8082.892215, 489.262728} };

  for (auto& p : points) {
    p.x += 9000;
    p.y -= 8000;
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
 *    noise = randn(size(x)) * 0.2;
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
 *    n = 0.6;
 *    c = 0.1;
 *    x=[0:0.5:5];
 *    y=[-2.5:0.5:2.5];
 *    z = m * x + n * y + c;
 *    noise = randn(size(x)) * 0.05 - 0.025;
 *    z_observed = z + noise;
 *    data = [x', y', z_observed'];
 */
std::vector<Point3d> ObtainPlanePoints() {
  std::mt19937 rng;
  std::vector<Point3d> points;

  const double m{ 0.3 };
  const double n{ 0.6 };
  const double c{ 0.1 };

  for (double x = 0.0; x <= 5.0; x += 0.5) {
    for (double y = -2.5; y <= 2.5; y += 0.5) {
      const double dz = 0.05 * (rng() * kScale - 0.5);
      const double z = m * x + n * y + c + dz;

      points.emplace_back<Point3d>({ x, y, z });
    }
  }
  return points;
}
