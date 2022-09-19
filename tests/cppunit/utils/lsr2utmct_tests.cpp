/* -----------------------------------------------------------------------------
 *  Copyright (c) 2022.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"
#include "testutils.h"

#include "utils/lsr2utmct.h"

using namespace earthxpert;

TEST(lsr2utmct, Convert) {
  std::string csvFile(TestConfig::DataPath("lsr2utm_africa.csv"));
  std::vector<Point3d> lsrs;
  std::vector<Point2d> utms;
  const auto n = testutils::ReadLsr2UtmCSV(csvFile, lsrs, utms);

  const Point3d origin(Deg2Rad(39.02214166666666), Deg2Rad(5.453869444444445), 0);  // Africa 5°27?13.93?N, 39°1?19.71?E
  LsrToUtmCT ct(origin);

  double pb[luc::NumCoef] = {1.0, 0, 0, 0, 0, 0, 0};   // scale, three rotation angles and three translation

  double de{}, dn{};
  for (int i = 0; i < n; ++i) {
    const auto& lsr = lsrs[i];
    const auto& utm = utms[i];

    const auto utmT = ct.Convert(&lsr, pb); // easting, northing;

    std::cout << lsr.x << ", " << lsr.y << ", " << lsr.z << "; "
      << utm.x << ", " << utm.y << "; " << utmT.x << ", " << utmT.y << "; "
      << utmT.x - utm.x << ", " << utmT.y - utm.y << std::endl;
    de += utmT.x - utm.x;
    dn += utmT.y - utm.y;
  }
  std::cout << de / n << ", " << dn / n << std::endl;
}
