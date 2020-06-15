/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/spatialref/earthellipsoid.h"

#include <cmath>
#include <cstring>

using earthxpert::EarthEllipsoid;
using earthxpert::Point3d;

/**
 * constructor. 
 *
 * \param [in] wkt parse ellipsoid from a datum, default to be WGS84 ellipsoid if empty.
 */
EarthEllipsoid::EarthEllipsoid(const std::string& wkt) {

}

/**
 * constructor.
 *
 * \param [in] semiMajor Semi-major axis length [m]
 * \param [in] eccSq first Eccentricity squared
 *
 * \remark default to be WGS84 ellipsoid
 */
EarthEllipsoid::EarthEllipsoid(const double semiMajor, const double eccSq) :
  mA(semiMajor), mEccSq(eccSq) {
  mAsq = mA * mA;
  mBsq_Asq = 1.0 - mEccSq;
  mBsq = mAsq * mBsq_Asq;
  mB = sqrt(mBsq);
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
 * Convert a position from geodetic LPH in ECEF to Cartesian XYZ in ECEF.
 *
 * \param [in] lon Geographic longitude [rad]
 * \param [in] lat Geographic latitude [rad]
 * \param [in] height Height above ellipsoid [m]
 * \return Returns Cartesian XYZ coordinates [m]
 */
Point3d EarthEllipsoid::LPHToXYZ(const double lon, const double lat, 
  const double height) const {
  const double cp = cos(lat);
  const double sp = sin(lat);

  const double N = RadiusInPrimeVertical(cp, sp);
  const double NH = (N + height)* cp;

  const double cl = cos(lon);
  const double sl = sin(lon);

  return { NH * cl, NH * sl, (N * mBsq_Asq + height) * sp };
}

/**
 * Convert Cartesian XYZ in ECEF to geographic longitude, latitude, ellipsoidal height in ECEF
 *
 * \param [in] X Cartesian X-coordinate [m]
 * \param [in] Y Cartesian Y-coordinate [m]
 * \param [in] Z Cartesian Z-coordinate [m]
 * \return Returns geographic longitude [rad], geographic latitude [rad], ellipsoidal height [m]
 */
Point3d EarthEllipsoid::XYZToLPH(const double X, const double Y,
  const double Z) const {

  const double p = sqrt(X * X + Y * Y);

  double lon = 2.0 * atan2(Y, X + p);
  double lat = atan(Z / (p * (1.0 - mEccSq)));
  double height{};

  double hgt{};    // Height from previous iteration.
  double delta{ 1.0 };

  while (mHgtTol < fabs(delta))
  {
    const double cp = cos(lat);
    const double sp = sin(lat);
    const double N = RadiusInPrimeVertical(cp, sp);
    
    height = p / cp - N;
    lat = atan(Z / (p * (1.0 - (N * mEccSq / (N + height)))));

    delta = height - hgt;
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

  const double cl = cos(mLsrOrigin.x);
  const double sl = sin(mLsrOrigin.x);
  const double cp = cos(mLsrOrigin.y);
  const double sp = sin(mLsrOrigin.y);

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

/**
 * Convert coordinates from LSR to XYZ in EXEF 
 *
 * \param [in] lsr LSR XYZ coordinates [m]
 * \return Returns ECEF XYZ coordinates [m]
 */
Point3d EarthEllipsoid::LSRToXYZ(const Point3d& lsr) const {
  const cv::Mat E = mL2E * cv::Mat(lsr);
  const Point3d lsrE(E.at<double>(0), E.at<double>(1), E.at<double>(2));
  return (mLsrOriginXYZ + lsrE);
}

/**
 * Convert coordinates from ECEF XYZ to LSR
 *
 * \param [in] ecf ECEF XYZ coordinates [m]
 * \return Returns LSR coordinates [m]
 */
Point3d EarthEllipsoid::XYZToLSR(const Point3d& xyz) const {
  const cv::Mat L = mL2E.t() * cv::Mat(xyz - mLsrOriginXYZ);
  return {L.at<double>(0), L.at<double>(1), L.at<double>(2)};
}

/**
 * \param [in] lat Geodetic latitude [rad]

 * \return Returns radius [m] of curvature in the Prime Meridian
 */
double EarthEllipsoid::RadiusInPrimeMeridian(const double lat) const {
  const double sp = sin(lat);
  return (mA * mBsq_Asq) / pow(1.0 - mEccSq * sp * sp, 1.5);
}

/**
 * \param [in] lat Geodetic latitude [rad]
 *
 * \return Returns radius [m] of curvature in the Prime Vertical terminated by minor axis
 */
double EarthEllipsoid::RadiusInPrimeVertical(const double lat) const {
  const double cp = cos(lat);
  const double sp = sin(lat);
  return mAsq / sqrt(mAsq * cp * cp + mBsq * sp * sp);  // RadiusInPrimeVertical(cp, sp)
}
