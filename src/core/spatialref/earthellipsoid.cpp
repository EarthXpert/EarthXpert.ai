/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/spatialref/earthellipsoid.h"

#include <cstring>

using earthxpert::EarthEllipsoid;
using earthxpert::Point3d;

/**
 * constructor. 
 *
 * \param [in] wkt parse ellipsoid from a datum, default to be WGS84 ellipsoid if empty.
 */
EarthEllipsoid::EarthEllipsoid(const std::string& wkt, const Point3d& origin) {
}

/**
 * constructor.
 *
 * \param [in] semiMajor Semi-major axis length [m]
 * \param [in] eccSq first Eccentricity squared
 *
 * \remark default to be WGS84 ellipsoid
 */
EarthEllipsoid::EarthEllipsoid(const double semiMajor, const double eccSq,
  const Point3d& origin) : mA(semiMajor), mEccSq(eccSq) {
  Initialize(origin);
}

void EarthEllipsoid::Initialize(const Point3d& origin) {
  mAsq = mA * mA;
  mBsq_Asq = 1.0 - mEccSq;
  mBsq = mAsq * mBsq_Asq;
  mB = sqrt(mBsq);
  SetLsrOrigin(origin);
}

/**
 * constructor.
 *
 * \param [in] semiMajor Semi-major axis length [m]
 * \param [in] semiMinor Semi-minor axis length [m]
 *
 * \remark default to be WGS84 ellipsoid
 */
//EarthEllipsoid::EarthEllipsoid(const double semiMajor, const double semiMinor) :
//  mA(semiMajor), mB(semiMinor) {
//  mAsq = mA * mA;
//  mBsq = mB * mB;
//  mBsq_Asq = mBsq / mAsq;
//  mEccSq = 1.0 - mBsq_Asq;
//}

/**
 * Comparison operator
 *
 * \param [in] rhs Object to compare with
 * \return true if they are identical, false otherwise
 */
bool
EarthEllipsoid::operator==(const EarthEllipsoid& rhs) const {
  if (this != &rhs) {
    EqualD Eq;
    if (!Eq(mA, rhs.mA) || !Eq(mEccSq, rhs.mEccSq))
      return false;

    if (mLsrOriginXYZ != rhs.mLsrOriginXYZ)
      return false;
  }
  return true;
}

/**
 * Test inequality.
 *
 * \param [in] rhs Object to compare with
 * \return true if not equal.
 */
bool EarthEllipsoid::operator!=(const EarthEllipsoid& rhs) const {
  return !(operator==(rhs));
}

/**
 * Convert a position from geodetic LPH in ECEF to Cartesian XYZ in ECEF.
 *
 * \param [in] lon Geographic longitude [rad]
 * \param [in] lat Geographic latitude [rad]
 * \param [in] height Height above ellipsoid [m]
 * \return Returns Cartesian XYZ coordinates [m]
 *
 * \remark how accurate in XYZ?
 */
Point3d EarthEllipsoid::LPHToXYZ(const double lon, const double lat, 
  const double height) const {
  const auto [sp, cp] = sincos(lat);
  const double N = RadiusInPrimeVertical(cp);
  const double NH = (N + height) * cp;

  const auto [sl, cl] = sincos(lon);
  return { NH * cl, NH * sl, (N * mBsq_Asq + height) * sp };
}

/**
 * Convert Cartesian XYZ in ECEF to geographic longitude, latitude, ellipsoidal height in ECEF
 *
 * \param [in] X Cartesian X-coordinate [m]
 * \param [in] Y Cartesian Y-coordinate [m]
 * \param [in] Z Cartesian Z-coordinate [m]
 * \return Returns geographic longitude [rad], geographic latitude [rad], ellipsoidal height [m]
 *
 * \remark how accurate in LPH?
 */
Point3d EarthEllipsoid::XYZToLPH(const double X, const double Y,
  const double Z) const {
  const double p = sqrt(X * X + Y * Y);

  const double lon = 2.0 * atan2(Y, X + p);
  double lat = atan(Z / (p * (1.0 - mEccSq)));
  double height{};

  double hgt{ -mA };  // Height from previous iteration, default to earth center.
  double dh{ 1.0 };

  while (kTolH < fabs(dh)) {
    const double cp = cos(lat);
    const double N = RadiusInPrimeVertical(cp);
    
    height = p / cp - N;
    lat = atan(Z / (p * (1.0 - (N * mEccSq / (N + height)))));

    dh = height - hgt;
    hgt = height;
  }

  return {lon, lat, height};
}

/**
 * Set the LSR origin used for converting coordinates to and from the LSR.
 *
 * \param [in] origin Origin longitude [rad], latitude [rad], height [m]
 */
void EarthEllipsoid::SetLsrOrigin(const Point3d& origin) {
  mLsrOrigin = origin;
  mLsrOriginXYZ = LPHToXYZ(mLsrOrigin);

  const auto [sl, cl] = sincos(mLsrOrigin.x);
  const auto [sp, cp] = sincos(mLsrOrigin.y);
  mL2E(0, 0) = -sl;
  mL2E(0, 1) = -sp * cl;
  mL2E(0, 2) = cp * cl;
  mL2E(1, 0) = cl;
  mL2E(1, 1) = -sp * sl;
  mL2E(1, 2) = cp * sl;
  mL2E(2, 0) = 0.0;
  mL2E(2, 1) = cp;
  mL2E(2, 2) = sp;
}
