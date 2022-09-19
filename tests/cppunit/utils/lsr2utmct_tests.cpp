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
  testutils::ReadLsr2UtmCSV(csvFile, lsrs, utms);

  const Point3d origin(Deg2Rad(39.02214166666666), Deg2Rad(5.453869444444445), 0);  // Africa 5°27?13.93?N, 39°1?19.71?E
  LsrToUtmCT ct(origin);

  double pb[6] = {0, 0, 0, 0, 0, 0};   // three rotation angles and three translation

  double de{}, dn{};
  for (int i = 0; i < lsrs.size(); ++i) {
    const auto& lsr = lsrs[i];
    const auto& utm = utms[i];

    Point2d utmT; // easting, northing;
    ct.Convert(&lsr, pb, utmT);

    std::cout << lsr.x << ", " << lsr.y << ", " << lsr.z << "; "
      << utm.x << ", " << utm.y << "; " << utmT.x << ", " << utmT.y << "; "
      << utmT.x - utm.x << ", " << utmT.y - utm.y << std::endl;
    de += utmT.x - utm.x;
    dn += utmT.y - utm.y;
  }
  std::cout << de / lsrs.size() << ", " << dn / lsrs.size() << std::endl;
}
