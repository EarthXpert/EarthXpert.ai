/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"

#include "core/spatialref/earthellipsoid.h"
#include "core/utils/typehelper.h"
#include <cmath>

using namespace earthxpert;

constexpr double kTolD = 1.0e-10; // [10um]
constexpr double kTol = 1.0e-5; // [10um]

TEST(earthellipsoid, ECEF_LPH) {
  EarthEllipsoid ee;

  const Point3d lph(Deg2Rad(-79.347015), Deg2Rad(43.651070), 0);  // Toronto
  const Point3d xyz = ee.LPHToXYZ(lph);
  const Point3d lphN = ee.XYZToLPH(xyz);
  const Point3d xyzN = ee.LPHToXYZ(lphN);

  EqualD EqA(kTolD);
  EqualD EqM(kTol);  

  ASSERT_TRUE(EqA(lph.x, lphN.x)) << "Longitude difference > 10 um";
  ASSERT_TRUE(EqA(lph.y, lphN.y)) << "Latitude difference > 10 um";
  ASSERT_TRUE(EqM(lph.z, lphN.z)) << "Height difference > 10 um";

  ASSERT_TRUE(EqM(xyz.x, xyzN.x)) << "X difference > 10 um";
  ASSERT_TRUE(EqM(xyz.y, xyzN.y)) << "Y difference > 10 um";
  ASSERT_TRUE(EqM(xyz.z, xyzN.z)) << "Z difference > 10 um";
}

TEST(earthellipsoid, ECEF_LPH_Cube) {
  EarthEllipsoid ee;

  double diffLon{}, diffLat{}, diffHgt{};
  double diffX{}, diffY{}, diffZ{};

  double maxDiffLon{ -1.0 }, maxDiffLat{ -1.0 }, maxDiffHgt{ -1.0 };
  double maxDiffX{ -1.0 }, maxDiffY{ -1.0 }, maxDiffZ{ -1.0 };

  for (double lon = -180.0; lon < 180.0; lon += 6.0)
    for (double lat = -90.0; lat < 90.0; lat += 3.0)
      for (double hgt = -2000.0; hgt < 10000.0; hgt += 100.0) {
        const Point3d lph(Deg2Rad(lon), Deg2Rad(lat), hgt);
        const Point3d xyz = ee.LPHToXYZ(lph);
        const Point3d lphN = ee.XYZToLPH(xyz);
        const Point3d xyzN = ee.LPHToXYZ(lphN);

        diffLon = fabs(lph.x - lphN.x);
        if (maxDiffLon < diffLon)
          maxDiffLon = diffLon;
        diffLat = fabs(lph.y - lphN.y);
        if (maxDiffLat < diffLat)
          maxDiffLat = diffLat;
        diffHgt = fabs(lph.z - lphN.z);
        if (maxDiffHgt < diffHgt)
          maxDiffHgt = diffHgt;

        diffX = fabs(xyz.x - xyzN.x);
        if (maxDiffX < diffX)
          maxDiffX = diffX;
        diffY = fabs(xyz.y - xyzN.y);
        if (maxDiffY < diffY)
          maxDiffY = diffY;
        diffZ = fabs(xyz.z - xyzN.z);
        if (maxDiffZ < diffZ)
          maxDiffZ = diffZ;
      }

  ASSERT_LT(maxDiffLon, kTolD) << "Longitude difference > 10 um";
  ASSERT_LT(maxDiffLat, kTolD) << "Latitude difference > 10 um";
  ASSERT_LT(maxDiffHgt, kTol) << "Height difference > 10 um";

  ASSERT_LT(maxDiffX, kTol) << "X difference > 10 um";
  ASSERT_LT(maxDiffY, kTol) << "Y difference > 10 um";
  ASSERT_LT(maxDiffZ, kTol) << "Z difference > 10 um";
}

TEST(earthellipsoid, LSR) {
  const Point3d origin(Deg2Rad(-79.347015), Deg2Rad(43.651070), 0);  // Toronto

  EarthEllipsoid ee;
  ee.SetLsrOrigin(origin);

  const Point3d lsr(11.0, -25.0, 31.0);
  const Point3d xyz = ee.LSRToXYZ(lsr);
  const Point3d lsrN = ee.XYZToLSR(xyz);
  const Point3d xyzN = ee.LSRToXYZ(lsrN);

  EqualD EqM(kTol); 
  ASSERT_TRUE(EqM(lsr.x, lsrN.x)) << "X difference > 10 um";
  ASSERT_TRUE(EqM(lsr.y, lsrN.y)) << "Y difference > 10 um";
  ASSERT_TRUE(EqM(lsr.z, lsrN.z)) << "Z difference > 10 um";
  ASSERT_TRUE(EqM(xyz.x, xyzN.x)) << "X difference > 10 um";
  ASSERT_TRUE(EqM(xyz.y, xyzN.y)) << "Y difference > 10 um";
  ASSERT_TRUE(EqM(xyz.z, xyzN.z)) << "Z difference > 10 um";

  const Point3d lph = ee.LSRToLPH(lsr);
  const Point3d lsrN2 = ee.LPHToLSR(lph);
  const Point3d lphN = ee.LSRToLPH(lsrN2);

  ASSERT_TRUE(EqM(lsr.x, lsrN2.x)) << "X difference > 10 um";
  ASSERT_TRUE(EqM(lsr.y, lsrN2.y)) << "Y difference > 10 um";
  ASSERT_TRUE(EqM(lsr.z, lsrN2.z)) << "Z difference > 10 um";

  EqualD EqA(kTolD);  
  ASSERT_TRUE(EqA(lph.x, lphN.x)) << "Longitude difference > 10 um";
  ASSERT_TRUE(EqA(lph.y, lphN.y)) << "Latitude difference > 10 um";
  ASSERT_TRUE(EqM(lph.z, lphN.z)) << "Height difference > 10 um";
}

/**
 *  Test 4: estimate errors of the inflated ellipsoids for
 *          height range [-2000, 9000]
 */
TEST(earthellipsoid, Inflate_Octant) {
  EqualD EqM(kTol);
  EarthEllipsoid ee;

  const double a = ee.GetSemiMajor();
  const double b = ee.GetSemiMinor();

  double lon = 0.0;
  for (double hgt = -2000.0; hgt < 10000.0; hgt += 1000.0) {
    const double ah = a + hgt;
    const double bh = b + hgt;

    double maxDist{}, maxLat;
    for (double lat = 0.0; lat < 90.0; lat += 1.0) {
      const Point3d lph(Deg2Rad(lon), Deg2Rad(lat), hgt);
      const Point3d xyz = ee.LPHToXYZ(lph);
      ASSERT_TRUE(EqM(0, xyz.y)) << "Y difference > 10 um";

      const double xah = xyz.x / ah;
      const double zbh = xyz.z / bh;
      const double t = sqrt(1.0 / (xah * xah + zbh * zbh));

      const double dist = sqrt(xyz.x * xyz.x + xyz.z * xyz.z) * fabs(t - 1.0);
      if (maxDist < dist) {
        maxDist = dist;
        maxLat = lat;
      }
    }

    std::cout << "max distance at height " << hgt << " & lat " << maxLat 
      << ": " << maxDist << std::endl;
  }
}
