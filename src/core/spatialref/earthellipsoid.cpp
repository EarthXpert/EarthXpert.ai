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
  double hgt{};

  double h{ -mA };  // Height from previous iteration, default to earth center.
  double dh{ 1.0 };

  while (kTolH < fabs(dh)) {
    const double cp = cos(lat);
    const double N = RadiusInPrimeVertical(cp);
    
    hgt = p / cp - N;
    lat = atan(Z / (p * (1.0 - (N * mEccSq / (N + hgt)))));

    dh = hgt - h;
    h = hgt;
  }

  return {lon, lat, hgt};
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

/**
 *  Ray-Ellipsoid intersection.
 *
 *  \return Returns Cartesian XYZ coordinates [m]
 */
Point3d EarthEllipsoid::RayIntersect(const Point3d& origin, 
  const Point3d& ray, const double height) const {
  // step 1: closed form at cm
  Point3d xyz = RayIntersect0(origin, ray, height);
  
  // step 2: refined to mm
  int iter{};
  for (iter = 0; iter < kMaxIterRI; ++iter) {
    const auto lph = XYZToLPH(xyz);
    const double dh = lph.z - height;

    xyz += ray * dh;

    if (fabs(dh) < kTolHRI)
      break;
  }
  return xyz;
}

/**
 *  Ray-Ellipsoid intersection using infalted ellipsoid.
 * 
 *  \return Returns Cartesian XYZ coordinates [m]
 */
Point3d EarthEllipsoid::RayIntersect0(const Point3d& origin,
  const Point3d& ray, const double height) const {
  const Point3d org = origin * kMETER2KM;  // [km]

  // check the angle between ray and the vector from earth center to origin
  // to ensure the ray is pointed at least approximately at the earth
  const double det = org.dot(ray);
  if (0.0 <= det)
    return {};

  const double ah = (mA + height) * kMETER2KM;  // inflated semi-major axis [km]
  const double bh = (mB + height) * kMETER2KM;  // inflated semi-minor axis [km]
  const double ah2 = ah * ah;
  const double eh2 = (bh * bh) / ah2;  // 1.0 - modified eccentricity squared

  // quadratic equation of the form ar^2 + br + c = 0 where r is range
  const double a = (ray.x * ray.x + ray.y * ray.y) * eh2 + ray.z * ray.z;
  const double b = ((ray.x * org.x + ray.y * org.y) * eh2 + ray.z * org.z) * 2.0;
  const double c = (org.x * org.x + org.y * org.y - ah2) * eh2 + org.z * org.z;

  const double d = b * b - 4.0 * a * c;
  if (0.0 >= d)   //  ensure the quadratic equation has a non-imaginary solution
    return {};

  const double range = ((-b - sqrt(d)) / (2.0 * a)) * kKM2METER;   // [m]
  assert(0.0 < range);  //  check if in the right direction

  const Point3d xyz = origin + ray * range;
  return xyz;
}
