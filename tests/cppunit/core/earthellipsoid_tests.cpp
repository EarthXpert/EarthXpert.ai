/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"

#include "core/spatialref/earthellipsoid.h"
#include "core/utils/typehelper.h"
#include <cmath>
#include <iostream>

using namespace earthxpert;

constexpr double kTolD = 1.0e-10; // [10um]
constexpr double kTol = 1.0e-5; // [10um]
constexpr double kTolRI = 1.0e-3; // [mm]

Point3d XYZToLPH_olson(double x, double y, double z);


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

  //const Point3d lph1(Deg2Rad(-1.209795130836728e+02), Deg2Rad(4.690621990064444e+01), 4.892377904755995e+02);
  //const Point3d xyz1 = ee.LPHToXYZ(lph1);
  //std::cout << std::setprecision(10) << xyz1.x << ", " << xyz1.y << ", " << xyz1.z << std::endl;
}

TEST(earthellipsoid, ECEF_LPH_Cube) {
  EarthEllipsoid ee;

  double diffLon{}, diffLat{}, diffHgt{};
  double diffX{}, diffY{}, diffZ{};

  double maxDiffLon{ -1.0 }, maxDiffLat{ -1.0 }, maxDiffHgt{ -1.0 };
  double maxDiffX{ -1.0 }, maxDiffY{ -1.0 }, maxDiffZ{ -1.0 };

  int maxNumIters{};
  double lon_lon{}, lon_lat{}, lon_hgt{};
  double lat_lon{}, lat_lat{}, lat_hgt{};
  double hgt_lon{}, hgt_lat{}, hgt_hgt{};

  for (double lon = -180.0; lon < 180.0; lon += 6.0)
    for (double lat = -90.0; lat < 90.0; lat += 3.0)
      for (double hgt = -20000.0; hgt < 2000000.0; hgt += 100.0) {
        int niters{};
        const Point3d lph(Deg2Rad(lon), Deg2Rad(lat), hgt);
        const Point3d xyz = ee.LPHToXYZ(lph);
        //const Point3d lphN = ee.XYZToLPH(xyz, &niters);
        const Point3d lphN = XYZToLPH_olson(xyz.x, xyz.y, xyz.z);
        const Point3d xyzN = ee.LPHToXYZ(lphN);

        diffLon = fabs(lph.x - lphN.x);
        if (maxDiffLon < diffLon) {
          maxDiffLon = diffLon;
          lon_lon = lon;
          lon_lat = lat;
          lon_hgt = hgt;
        }
        diffLat = fabs(lph.y - lphN.y);
        if (maxDiffLat < diffLat) {
          maxDiffLat = diffLat;
          lat_lon = lon;
          lat_lat = lat;
          lat_hgt = hgt;
        }
        diffHgt = fabs(lph.z - lphN.z);
        if (maxDiffHgt < diffHgt) {
          maxDiffHgt = diffHgt;
          hgt_lon = lon;
          hgt_lat = lat;
          hgt_hgt = hgt;
        }

        diffX = fabs(xyz.x - xyzN.x);
        if (maxDiffX < diffX)
          maxDiffX = diffX;
        diffY = fabs(xyz.y - xyzN.y);
        if (maxDiffY < diffY)
          maxDiffY = diffY;
        diffZ = fabs(xyz.z - xyzN.z);
        if (maxDiffZ < diffZ)
          maxDiffZ = diffZ;

        if (niters > maxNumIters)
          maxNumIters = niters;
      }

  std::cout << "Difference in LPH " << maxDiffLon << ", " << maxDiffLat << ", " << maxDiffHgt << std::endl;
  std::cout << "Difference in XYZ " << maxDiffX << ", " << maxDiffY << ", " << maxDiffZ << std::endl;
  std::cout << "maxNumIters in XYZToLPH " << maxNumIters << std::endl;
  std::cout << "Worst lon location " << lon_lon << ", " << lon_lat << ", " << lon_hgt << std::endl;
  std::cout << "Worst lat location " << lat_lon << ", " << lat_lat << ", " << lat_hgt << std::endl;
  std::cout << "Worst hgt location " << hgt_lon << ", " << hgt_lat << ", " << hgt_hgt << std::endl;

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

/**
 *  Test 5: ray intersection
 */
TEST(earthellipsoid, RayIntersect) {
  EarthEllipsoid ee;
  const Point3d org = ee.LPHToXYZ(Point3d{ Deg2Rad(0.0), Deg2Rad(45.0), 400'000.0 });

  double diffX{}, diffY{}, diffZ{};
  double maxDiffX0{ -1.0 }, maxDiffY0{ -1.0 }, maxDiffZ0{ -1.0 };
  double maxDiffX{ -1.0 }, maxDiffY{ -1.0 }, maxDiffZ{ -1.0 };

  for (double hgt = -2000.0; hgt < 10000.0; hgt += 1000.0) {
    const Point3d grd = ee.LPHToXYZ(Point3d{ Deg2Rad(1.0), Deg2Rad(46.0), hgt });

    const Point3d los = grd - org;  // line of sight poiting from satellite to ground
    const Point3d ray = los * (1.0 / thirdparty::normL2(los)); // unit view vector

    // intial
    const Point3d grdN0 = ee.RayIntersect0(org, ray, hgt);

    diffX = fabs(grd.x - grdN0.x);
    if (maxDiffX0 < diffX)
      maxDiffX0 = diffX;
    diffY = fabs(grd.y - grdN0.y);
    if (maxDiffY0 < diffY)
      maxDiffY0 = diffY;
    diffZ = fabs(grd.z - grdN0.z);
    if (maxDiffZ0 < diffZ)
      maxDiffZ0 = diffZ;

    std::cout << "Difference at height (initial) " << hgt << ": "
      << diffX << ", " << diffY << ", " << diffZ << std::endl;

    // refined
    const Point3d grdN = ee.RayIntersect(org, ray, hgt);

    diffX = fabs(grd.x - grdN.x);
    if (maxDiffX < diffX)
      maxDiffX = diffX;
    diffY = fabs(grd.y - grdN.y);
    if (maxDiffY < diffY)
      maxDiffY = diffY;
    diffZ = fabs(grd.z - grdN.z);
    if (maxDiffZ < diffZ)
      maxDiffZ = diffZ;

    std::cout << "Difference at height (refined) " << hgt << ": "
      << diffX << ", " << diffY << ", " << diffZ << std::endl;
  }

  ASSERT_LT(maxDiffX, kTolRI) << "X difference > 1 mm";
  ASSERT_LT(maxDiffY, kTolRI) << "Y difference > 1 mm";
  ASSERT_LT(maxDiffZ, kTolRI) << "Z difference > 1 mm";
}

/**
 * Olson, D. K. (1996). Converting Earth-Centered, Earth-Fixed Coordinates to Geodetic Coordinates. IEEE Transactions on Aerospace and Electronic Systems, 32(1), 473–476. https://doi.org/10.1109/7.481290
 * 
 * Note: 4x faster than iterative algorithm.

 * \param x X coordinate (meters)
 * \param y Y coordinate (meters)
 * \param z Z coordinate (meters)
 */
Point3d XYZToLPH_olson(double x, double y, double z) {
  double a = 6378137.0; /*wgs-84*/
  double e2 = 6.6943799901377997e-3;
  double a1 = 4.2697672707157535e+4;
  double a2 = 1.8230912546075455e+9;
  double a3 = 1.4291722289812413e+2;
  double a4 = 4.5577281365188637e+9;
  double a5 = 4.2840589930055659e+4;
  double a6 = 9.9330562000986220e-1;

  /* a1 = a*e2, a2 = a1*a1, a3 = a1*e2/2, */
  /* a4 = (5/2)*a2, a5 = a1 + a3, a6 = 1 - e2 */

  double zp = fabs(z);
  double w2 = x * x + y * y;
  double w = sqrt(w2);
  double z2 = z * z;
  double r2 = w2 + z2;
  double r = sqrt(r2);

  if (r < 100000.)
    return { 0., 0., -1.e7 };

  double lon = atan2(y, x);

  double s2 = z2 / r2;
  double c2 = w2 / r2;
  double u = a2 / r;
  double v = a3 - a4 / r;

  double lat{};
  double s, c, ss;
  if (c2 > .3) {
    s = (zp / r) * (1. + c2 * (a1 + u + s2 * v) / r);
    lat = asin(s);
    ss = s * s;
    c = sqrt(1. - ss);
  }
  else {
    c = (w / r) * (1. - s2 * (a5 - u - c2 * v) / r);
    lat = acos(c);
    ss = 1. - c * c;
    s = sqrt(ss);
  }

  double g = 1. - e2 * ss;
  double rg = a / sqrt(g);
  double rf = a6 * rg;
  u = w - rg * c;
  v = zp - rf * s;
  double f = c * u + s * v;
  double m = c * v - s * u;
  double p = m / (rf / g + f);
  lat = lat + p;
  double hgt = f + m * p / 2.;
  if (z < 0.)
    lat = -lat;

  return { lon, lat, hgt };
}
