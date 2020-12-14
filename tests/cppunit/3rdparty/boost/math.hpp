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
  // The lithium potential is given in Kohn's paper, Table I.
  // (We could equally easily use an unordered_map, a list of tuples or pairs, or a 2-dimensional array).
  std::map<double, double> r{
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

  // Let's discover the abscissa that will generate a potential of exactly 3.0,
  // start by creating 2 ranges for the x and y values:
  auto x_range = boost::adaptors::keys(r);
  auto y_range = boost::adaptors::values(r);
  bm::barycentric_rational<double> b(x_range.begin(), x_range.end(), y_range.begin());
  //
  // We'll use a lambda expression to provide the functor to our root finder, since we want
  // the abscissa value that yields 3, not zero.  We pass the functor b by value to the
  // lambda expression since barycentric_rational is trivial to copy.
  // Here we're using simple bisection to find the root:
  boost::uintmax_t iterations = (std::numeric_limits<boost::uintmax_t>::max)();
  double abscissa_3 = bmt::bisect([=](double x) { return b(x) - 3; }, 0.44, 1.24, boost::math::tools::eps_tolerance<double>(), iterations).first;
  std::cout << "Abscissa value that yields a potential of 3 = " << abscissa_3 << std::endl;
  std::cout << "Root was found in " << iterations << " iterations." << std::endl;
  
  // However, we have a more efficient root finding algorithm than simple bisection:
  iterations = (std::numeric_limits<boost::uintmax_t>::max)();
  abscissa_3 = bmt::bracket_and_solve_root([=](double x) { return b(x) - 3; }, 0.6, 1.2, false, boost::math::tools::eps_tolerance<double>(), iterations).first;
  std::cout << "Abscissa value that yields a potential of 3 = " << abscissa_3 << std::endl;
  std::cout << "Root was found in " << iterations << " iterations." << std::endl;
}

/**
 *  \brief The cardinal cubic B-spline class provided by Boost allows fast and accurate interpolation of a function which is known at equally spaced points.
 *
 *  \url https://www.boost.org/doc/libs/master/libs/math/doc/html/math_toolkit/cardinal_cubic_b.html
 */
TEST(boost, bmi_cardinal_cubic_b_spline) {
  std::vector<double> v(500);
  const double step = 0.01;

  // Initialize the vector with a function we'd like to interpolate:
  for (size_t i = 0; i < v.size(); ++i) {
    v[i] = sin(i * step);
  }
  // We could define an arbitrary start time, but for now we'll just use 0:
  bmi::cardinal_cubic_b_spline<double> spline(v.data(), v.size(), 0 /* start time */, step);

  // Now we can evaluate the spline wherever we please.
  std::mt19937 gen;
  brd::uniform_real_distribution<double> abscissa(0, v.size() * step);
  for (size_t i = 0; i < 10; ++i) {
    double x = abscissa(gen);
    std::cout << "sin(" << x << ") = " << sin(x) << ", spline interpolation gives " << spline(x) << std::endl;
    std::cout << "cos(" << x << ") = " << cos(x) << ", spline derivative interpolation gives " << spline.prime(x) << std::endl;
  }

  // The next example is less trivial:
  // We will try to figure out when the population of the United States crossed 100 million.
  // Since the census is taken every 10 years, the data is equally spaced, so we can use the cubic b spline.
  // Data taken from https://en.wikipedia.org/wiki/United_States_Census
  // We'll start at the year 1860:
  double t0 = 1860;
  double time_step = 10;
  std::vector<double> population{ 31443321,  /* 1860 */
                                 39818449,  /* 1870 */
                                 50189209,  /* 1880 */
                                 62947714,  /* 1890 */
                                 76212168,  /* 1900 */
                                 92228496,  /* 1910 */
                                 106021537, /* 1920 */
                                 122775046, /* 1930 */
                                 132164569, /* 1940 */
                                 150697361, /* 1950 */
                                 179323175 };/* 1960 */

  // An eyeball estimate indicates that the population crossed 100 million around 1915.
  // Let's see what interpolation says:
  bmi::cardinal_cubic_b_spline<double> p(population.data(), population.size(), t0, time_step);

  // Now create a function which has a zero at p = 100,000,000:
  auto f = [=](double t) { return p(t) - 100000000; };

  // Boost includes a bisection algorithm, which is robust, though not as fast as some others
  // we provide, but let's try that first.  We need a termination condition for it, which
  // takes the two endpoints of the range and returns either true (stop) or false (keep going),
  // we could use a predefined one such as boost::math::tools::eps_tolerance<double>, but that
  // won't stop until we have full double precision which is overkill, since we just need the
  // endpoint to yield the same month.  While we're at it, we'll keep track of the number of
  // iterations required too, though this is strictly optional:

  auto termination = [](double left, double right) {
    double left_month = std::round((left - std::floor(left)) * 12 + 1);
    double right_month = std::round((right - std::floor(right)) * 12 + 1);
    return (left_month == right_month) && (std::floor(left) == std::floor(right));
  };

  std::uintmax_t iterations = 1000;
  auto result = boost::math::tools::bisect(f, 1910.0, 1920.0, termination, iterations);
  auto time = result.first;  // termination condition ensures that both endpoints yield the same result
  auto month = std::round((time - std::floor(time)) * 12 + 1);
  auto year = std::floor(time);
  std::cout << "The population of the United States surpassed 100 million on the ";
  std::cout << month << "th month of " << year << std::endl;
  std::cout << "Found in " << iterations << " iterations" << std::endl;

  // Since the cubic B spline offers the first derivative, we could equally have used Newton iterations,
  // this takes "number of bits correct" as a termination condition - 20 should be plenty for what we need,
  // and once again, we track how many iterations are taken:

  auto f_n = [=](double t) { return std::make_pair(p(t) - 100000000, p.prime(t)); };
  iterations = 1000;
  time = bmt::newton_raphson_iterate(f_n, 1910.0, 1900.0, 2000.0, 20, iterations);
  month = std::round((time - std::floor(time)) * 12 + 1);
  year = std::floor(time);
  std::cout << "The population of the United States surpassed 100 million on the ";
  std::cout << month << "th month of " << year << std::endl;
  std::cout << "Found in " << iterations << " iterations" << std::endl;
}
