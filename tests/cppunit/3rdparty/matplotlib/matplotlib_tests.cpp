/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"
#include "3rdparty/eigen/exeigen.h"
#include "3rdparty/matplotlib/exmatplotlib.h"

#include <cmath>
#include <iostream>

namespace plt = matplotlibcpp;
using namespace earthxpert;

constexpr int n = 5000;

/**
 *  TestCase 0:
 */
TEST(matplotlib, plot) {
  std::vector<double> x(n), y(n), z(n), w(n, 2);
  for (int i = 0; i < n; ++i) {
    x.at(i) = i * i;
    y.at(i) = sin(2 * M_PI * i / 360.0);
    z.at(i) = log(i);
  }

  plt::figure_size(1200, 780);
  plt::plot(x, y);
  plt::plot(x, w, "r--");  // red dashed line
  plt::named_plot("log(x)", x, z);
  plt::xlim(0, 1000 * 1000);  // x-axis interval [0,1000000]
  plt::title("Sample figure");
  plt::legend();
  plt::show();

  const std::string imgFile(TestConfig::OutputPath("gnuplot_basic.png"));
  plt::save(imgFile);
}

/**
 *  TestCase 1:
 */
TEST(matplotlib, eigen) {
  Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(200, 0, 6);
  Eigen::VectorXd y = x.array().sin().exp().matrix();  // exp(sin(x))
  Eigen::VectorXd z = x.array().cos().exp().matrix();  // exp(cos(x))
  
  std::vector<double> vx = VecEigen2Std(x);
  std::vector<double> vy = VecEigen2Std(y);
  std::vector<double> vz = VecEigen2Std(z);

  plt::semilogx(vx, vy, std::string("tab:red")); 
  plt::semilogx(vx, vz, std::string("tab:blue"));

  plt::xlabel("Time in lecture");
  plt::ylabel("Student confusion");

  plt::grid(true);
  plt::show();
}
