/* -----------------------------------------------------------------------------
 *  Copyright (c) 2022.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"
#include "testutils.h"

#include "3rdparty/stl/exstl.h"

#include "utils/lsr2utm.h"
#include <boost/format.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>

using namespace earthxpert;


TEST(lsr2utm, Solve) {
  std::string csvFile(TestConfig::DataPath("lsr2utm_africa.csv"));
  std::vector<Point3d> lsrs;
  std::vector<Point2d> utms;
  const auto n = testutils::ReadLsr2UtmCSV(csvFile, lsrs, utms);

  // scale
  double dl{}, du{};  // distance between points
  for (int i = 0; i < n-1; ++i) {
    const auto& lsr = lsrs[i];
    const auto& lsrn = lsrs[i+1]; // next
    const auto& utm = utms[i];
    const auto& utmn = utms[i+1];

    dl += sqrt((lsr.x - lsrn.x) * (lsr.x - lsrn.x) + (lsr.y - lsrn.y) * (lsr.y - lsrn.y));
    du += sqrt((utm.x - utmn.x) * (utm.x - utmn.x) + (utm.y - utmn.y) * (utm.y - utmn.y));
  }
  std::cout << dl << ", " << du << ": " << dl / du << std::endl << std::endl;

  const Point3d origin(Deg2Rad(39.02214166666666), Deg2Rad(5.453869444444445), 0);  // Africa 5°27?13.93?N, 39°1?19.71?E
  LsrToUtm l2u(origin);

  // solve six unknowns: three rotation angles and three translation
  std::array<double, 7> pb{ 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
  auto ret = l2u.Solve(lsrs, utms, pb.data());
  if (!ret) {
    std::cout << "Lsr2Utm: failed to solve" << std::endl;
  }

  std::stringstream ss;
  ss << boost::format("scale: %1%; angles: %2%, %3%, %4%; translation: %5%, %6%, %7%") % pb[0] % pb[1] % pb[2] % pb[3] % pb[4] % pb[5] % pb[6];
  std::cout << ss.str() << std::endl << std::endl;

  auto utmsT = l2u.Convert(lsrs, pb.data());  // predicate with solved design parameters
  std::vector<double> sqerr(lsrs.size());     // squared errors

  for (int i = 0; i < n; ++i) {
    const auto& lsr = lsrs[i];
    const auto& utm = utms[i];
    const auto& utmT = utmsT[i];
    sqerr[i] = (utmT.x - utm.x) * (utmT.x - utm.x) + (utmT.y - utm.y) * (utmT.y - utm.y);

    std::cout << lsr.x << ", " << lsr.y << ", " << lsr.z << "; "
      << utm.x << ", " << utm.y << "; " << utmT.x << ", " << utmT.y << "; "
      << utmT.x - utm.x << ", " << utmT.y - utm.y << std::endl;
  }

  // worst: a possible outlier
  const auto idx = arg_max(sqerr);
  const auto& lsr = lsrs[idx];
  const auto& utm = utms[idx];
  const auto& utmT = utmsT[idx];
  std::cout << std::endl << lsr.x << ", " << lsr.y << ", " << lsr.z << "; "
    << utm.x << ", " << utm.y << "; " << utmT.x << ", " << utmT.y << "; "
    << utmT.x - utm.x << ", " << utmT.y - utm.y << std::endl;
}
