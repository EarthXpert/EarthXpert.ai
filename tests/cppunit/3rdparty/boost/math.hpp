/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <boost/math/interpolators/barycentric_rational.hpp>
#include <boost/math/interpolators/cardinal_cubic_b_spline.hpp>
#include <boost/math/tools/roots.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/range/adaptors.hpp>

#include <iostream>
#include <random>
#include <vector>

namespace bm = boost::math;
namespace bmi = bm::interpolators;
namespace bmt = bm::tools;
namespace brd = boost::random;

/**
 *  \brief Barycentric rational interpolation is a high-accuracy interpolation method for non-uniformly spaced samples.
 *
 *  \url https://www.boost.org/doc/libs/master/libs/math/doc/html/math_toolkit/barycentric.html
 */
TEST(boost, bmi_barycentric_rational) {
  // equally use an unordered_map, a list of tuples or pairs, or a 2D array.
  std::map<double, double> xy{
    {0.02, 5.727}, {0.04, 5.544}, {0.06, 5.450}, {0.08, 5.351},
    {0.10, 5.253}, {0.12, 5.157}, {0.14, 5.058}, {0.16, 4.960},
    {0.18, 4.862}, {0.20, 4.762}, {0.24, 4.563}, {0.28, 4.360},
    {0.32, 4.1584}, {0.36, 3.9463}, {0.40, 3.7360}, {0.44, 3.5429},
    {0.48, 3.3797}, {0.52, 3.2417}, {0.56, 3.1209}, {0.60, 3.0138},
    {0.68, 2.8342}, {0.76, 2.6881}, {0.84, 2.5662}, {0.92, 2.4242},
    {1.00, 2.3766}, {1.08, 2.3058}, {1.16, 2.2458}, {1.24, 2.2035},
    {1.32, 2.1661}, {1.40, 2.1350}, {1.48, 2.1090}, {1.64, 2.0697},
    {1.80, 2.0466}, {1.96, 2.0325}, {2.12, 2.0288}, {2.28, 2.0292},
    {2.44, 2.0228}, {2.60, 2.0124}, {2.76, 2.0065}, {2.92, 2.0031},
    {3.08, 2.0015}, {3.24, 2.0008}, {3.40, 2.0004}, {3.56, 2.0002},
    {3.72, 2.0001} };

  // the abscissa that will generate a potential of exactly 3.0,
  auto x_range = boost::adaptors::keys(xy);
  auto y_range = boost::adaptors::values(xy);
  bm::barycentric_rational<double> br(x_range.begin(), x_range.end(), y_range.begin());

  // simple bisection to find the root:
  boost::uintmax_t numIters = (std::numeric_limits<boost::uintmax_t>::max)();
  double abscissa_3 = bmt::bisect([=](double x) { return br(x) - 3; }, 
    0.44, 1.24, bmt::eps_tolerance<double>(), numIters).first;
  std::cout << "Abscissa value that yields a potential of 3 = " << abscissa_3 << std::endl;
  std::cout << "Root was found in " << numIters << " iterations." << std::endl;
  
  // a more efficient root finding algorithm than simple bisection:
  numIters = (std::numeric_limits<boost::uintmax_t>::max)();
  abscissa_3 = bmt::bracket_and_solve_root([=](double x) { return br(x) - 3; },
    0.6, 1.2, false, bmt::eps_tolerance<double>(), numIters).first;
  std::cout << "Abscissa value that yields a potential of 3 = " << abscissa_3 << std::endl;
  std::cout << "Root was found in " << numIters << " iterations." << std::endl;
}

/**
 *  \brief The cardinal cubic B-spline class provided by Boost allows fast and accurate interpolation of a function which is known at equally spaced points.
 *
 *  \url https://www.boost.org/doc/libs/master/libs/math/doc/html/math_toolkit/cardinal_cubic_b.html
 */
TEST(boost, bmi_cardinal_cubic_b_spline) {
  constexpr double step = 0.01;
  std::vector<double> y(500);

  for (size_t i = 0; i < y.size(); ++i) {
    y[i] = sin(i * step);
  }

  bmi::cardinal_cubic_b_spline<double> spline(y.data(), y.size(), 0, step);

  std::mt19937 gen;
  brd::uniform_real_distribution<double> abscissa(0, y.size() * step);
  for (size_t i = 0; i < 10; ++i) {
    const double x = abscissa(gen);
    std::cout << "sin(" << x << ") = " << sin(x) << ", spline(" << x << ") = " << spline(x) << std::endl;
    std::cout << "cos(" << x << ") = " << cos(x) << ", spline.prime(" << x << ") = " << spline.prime(x) << std::endl;
  }

  // figure out when the population of the United States crossed 100 million.
  // Since the census is taken every 10 years, the data is equally spaced, so use the cubic b spline.
  // Data taken from https://en.wikipedia.org/wiki/United_States_Census
  constexpr double t0 = 1860;  // start year
  constexpr double time_step = 10;
  std::vector<double> population{ 31443321,    // 1860
                                  39818449,    // 1870
                                  50189209,    // 1880
                                  62947714,    // 1890
                                  76212168,    // 1900
                                  92228496,    // 1910
                                  106021537,   // 1920
                                  122775046,   // 1930
                                  132164569,   // 1940
                                  150697361,   // 1950
                                  179323175 }; // 1960

  // An eyeball estimate indicates that the population crossed 100 million around 1915.
  bmi::cardinal_cubic_b_spline<double> p(population.data(), population.size(), t0, time_step);

  // create a function which has a zero at p = 100,000,000:
  auto f = [=](const double t) { return p(t) - 100000000; };

  // bisection algorithm
  auto tol = [](const double left, const double right) {
    double left_month = std::round((left - std::floor(left)) * 12 + 1);
    double right_month = std::round((right - std::floor(right)) * 12 + 1);
    return (left_month == right_month) && (std::floor(left) == std::floor(right)); };

  std::uintmax_t numIters = 1000;
  auto result = bmt::bisect(f, 1910.0, 1920.0, tol, numIters);
  auto time = result.first;  // termination condition ensures that both endpoints yield the same result
  auto month = std::round((time - std::floor(time)) * 12 + 1);
  auto year = std::floor(time);
  std::cout << "The population of the United States surpassed 100 million on the "
    << month << "th month of " << year << std::endl;
  std::cout << "Found in " << numIters << " iterations" << std::endl;

  // the cubic B spline offers the first derivative, or equally Newton iterations,
  // this takes "number of bits correct" as a termination condition - 20 should be plenty for what we need,
  auto f_n = [=](double t) { return std::make_pair(p(t) - 100000000, p.prime(t)); };
  numIters = 1000;
  time = bmt::newton_raphson_iterate(f_n, 1910.0, 1900.0, 2000.0, 20, numIters);
  month = std::round((time - std::floor(time)) * 12 + 1);
  year = std::floor(time);
  std::cout << "The population of the United States surpassed 100 million on the "
    << month << "th month of " << year << std::endl;
  std::cout << "Found in " << numIters << " iterations" << std::endl;
}
