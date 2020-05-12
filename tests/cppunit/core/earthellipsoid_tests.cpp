/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rd_party/gtest/gtest_main.h"
#include "testconfig.h"

#include "core/spatialref/earthellipsoid.h"
#include "core/utils/typehelper.h"

using namespace watxtract;

TEST(earthellipsoid, ECEF_LPH) {
  EarthEllipsoid ee;

  const Point3d lph(Deg2Rad(-79.347015), Deg2Rad(43.651070), 0);  // Toronto
  const Point3d xyz = ee.LPHToXYZ(lph);
  const Point3d lphN = ee.XYZToLPH(xyz);
  const Point3d xyzN = ee.LPHToXYZ(lphN);

  EqualD EqA(1.0e-8);  // [mm]
  EqualD EqM(1.0e-3);  // [mm]

  ASSERT_TRUE(EqA(lph.x, lphN.x)) << "Longitude difference > 1 mm";
  ASSERT_TRUE(EqA(lph.y, lphN.y)) << "Latitude difference > 1 mm";
  ASSERT_TRUE(EqM(lph.z, lphN.z)) << "Height difference > 1 mm";

  ASSERT_TRUE(EqM(xyz.x, xyzN.x)) << "X difference > 1 mm";
  ASSERT_TRUE(EqM(xyz.y, xyzN.y)) << "Y difference > 1 mm";
  ASSERT_TRUE(EqM(xyz.z, xyzN.z)) << "Z difference > 1 mm";
}

TEST(earthellipsoid, LSR) {
  const Point3d origin(Deg2Rad(-79.347015), Deg2Rad(43.651070), 0);  // Toronto

  EarthEllipsoid ee;
  ee.SetLsrOrigin(origin);

  const Point3d lsr(11.0, -25.0, 31.0);
  const Point3d xyz = ee.LSRToXYZ(lsr);
  const Point3d lsrN = ee.XYZToLSR(xyz);
  const Point3d xyzN = ee.LSRToXYZ(lsrN);

  EqualD EqM(1.0e-3);  // [mm]
  ASSERT_TRUE(EqM(lsr.x, lsrN.x)) << "X difference > 1 mm";
  ASSERT_TRUE(EqM(lsr.y, lsrN.y)) << "Y difference > 1 mm";
  ASSERT_TRUE(EqM(lsr.z, lsrN.z)) << "Z difference > 1 mm";
  ASSERT_TRUE(EqM(xyz.x, xyzN.x)) << "X difference > 1 mm";
  ASSERT_TRUE(EqM(xyz.y, xyzN.y)) << "Y difference > 1 mm";
  ASSERT_TRUE(EqM(xyz.z, xyzN.z)) << "Z difference > 1 mm";

  const Point3d lph = ee.LSRToLPH(lsr);
  const Point3d lsrN2 = ee.LPHToLSR(lph);
  const Point3d lphN = ee.LSRToLPH(lsrN2);

  ASSERT_TRUE(EqM(lsr.x, lsrN2.x)) << "X difference > 1 mm";
  ASSERT_TRUE(EqM(lsr.y, lsrN2.y)) << "Y difference > 1 mm";
  ASSERT_TRUE(EqM(lsr.z, lsrN2.z)) << "Z difference > 1 mm";

  EqualD EqA(1.0e-8);  // [mm]
  ASSERT_TRUE(EqA(lph.x, lphN.x)) << "Longitude difference > 1 mm";
  ASSERT_TRUE(EqA(lph.y, lphN.y)) << "Latitude difference > 1 mm";
  ASSERT_TRUE(EqM(lph.z, lphN.z)) << "Height difference > 1 mm";
}
