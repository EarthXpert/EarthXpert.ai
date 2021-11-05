#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/interpolate/exinterpolate.h"

#include <cmath>
#include <iostream>

using namespace earthxpert;

constexpr double kStep = 0.01;
constexpr double kTol = 2.0e-11;

void FillData(size_t count, std::vector<double>& x, std::vector<double>& y);

TEST(interpolate, Interpolator1D) {
  std::vector<double> x, y;
  FillData(500, x, y);

  auto interp = CreateInterpFunc<eIA_Cubic>(x, y);
  ASSERT_DOUBLE_EQ(sin(2.5), interp(2.5)) << "interpolated value";
  ASSERT_NEAR(sin(2.505), interp(2.505), kTol) << "interpolated value";
}

TEST(interpolate, AnyInterpolator1D) {
  std::vector<double> x, y;
  FillData(500, x, y);

  auto interp = CreateAnyInterp<eIA_Cubic>(x, y);
  ASSERT_DOUBLE_EQ(sin(2.5), interp(2.5)) << "interpolated value";
  ASSERT_NEAR(sin(2.505), interp(2.505), kTol) << "interpolated value";
}

void FillData(const size_t count, std::vector<double>& x, std::vector<double>& y) {
  assert(0 < count);
  x.resize(count);
  y.resize(count);

  for (size_t i = 0; i < count; ++i) {
    x[i] = kStep * i;
    y[i] = sin(x[i]);
  }
}
