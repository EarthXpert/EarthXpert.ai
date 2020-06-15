/* -----------------------------------------------------------------------------
*  Copyright(c) 2019.  All rights reserved.
* ---------------------------------------------------------------------------- -
*/
#pragma once

#include "core/excore.h"
#include "core/math/constants.h"
#include "core/utils/typehelper.h"
#include "3rdparty/opencv/exopencv.h"
#include "opencv2/core/mat.hpp"

#include <memory>

namespace earthxpert {
/**
 * The class EarthEllipsoid implements coordinate conversion of the ellipsoids
 * for ECEF/ECF coordinate frame.
 */
class EXCORE_EXPORTS EarthEllipsoid
{
public:
  EarthEllipsoid(const std::string& wkt);
  EarthEllipsoid(double semiMajor = kWGS84_A, double eccSq = kWGS84_ECCSQ);
  //EarthEllipsoid(double semiMajor = kWGS84_A, double semiMinor = kWGS84_B);
  ~EarthEllipsoid() = default;

  bool operator==(const EarthEllipsoid& rhs) const;

  std::unique_ptr<EarthEllipsoid> Clone() const {
    return std::make_unique<EarthEllipsoid>(*this);
  }

  double GetSemiMajor() const noexcept { return mA; }
  double GetEccSquared() const noexcept { return mEccSq; }

  inline Point3d LPHToXYZ(const Point3d& lph) const {
    return LPHToXYZ(lph.x, lph.y, lph.z);
  }
  Point3d LPHToXYZ(double lon, double lat, double height) const;

  inline Point3d XYZToLPH(const Point3d& xyz) const {
    return XYZToLPH(xyz.x, xyz.y, xyz.z);
  }
  Point3d XYZToLPH(double X, double Y, double Z) const;

  Point3d LSRToXYZ(const Point3d& lsr) const;
  inline Point3d LSRToXYZ(double X, double Y, double Z) const {
    return LSRToXYZ(Point3d(X, Y, Z));
  }

  Point3d XYZToLSR(const Point3d& xyz) const;
  inline Point3d XYZToLSR(double X, double Y, double Z) const {
    return XYZToLSR(Point3d(X, Y, Z));
  }

  /**
   * Convert coordinates from LSR to LPH.
   *
   * \param [in] lsr LSR XYZ coordinates [m]
   * \return Returns geographic longitude [rad], geographic latitude [rad], ellipsoidal height [m]
   */
  Point3d LSRToLPH(const Point3d& lsr) const {
    const Point3d xyz = LSRToXYZ(lsr);
    return XYZToLPH(xyz);
  }

  inline Point3d LSRToLPH(double X, double Y, double Z) const {
    return LSRToLPH(Point3d(X, Y, Z));
  }

  /**
   * Convert coordinates from the Geographic LPH to LSR
   *
   * \param [in] lph Longitude [rad], Latitude [rad], Ellipsoidal height [m]
   * \return Returns LSR XYZ coordinates [m]
   */
  Point3d LPHToLSR(const Point3d& lph) const {
    const Point3d xyz = LPHToXYZ(lph);
    return XYZToLSR(xyz);
  }

  inline Point3d LPHToLSR(double lon, double lat, double height) const {
    return LPHToLSR(Point3d(lon, lat, height));
  }

  void SetLsrOrigin(const Point3d& origin);
  inline Point3d GetLsrOrigin() const {
    return mLsrOrigin;
  }

private:
  double mA{kWGS84_A};          // Semi-major axis length [m]
  double mEccSq{kWGS84_ECCSQ};  // first Eccentricity squared

  Point3d mLsrOrigin{0.0, 0.0, 0.0}; // LSR origin Longitude/Latitude/Height
  double mHgtTol{1.0e-4}; // Height tolerance

  double RadiusInPrimeMeridian(double) const;
  double RadiusInPrimeVertical(double) const;

  /**
   * \param [in] cp cosine(geodetic latitude)
   * \param [in] sp sine(geodetic latitude)
   *
   * \return Returns radius [m] of curvature in the Prime Vertical terminated by minor axis
   */
  double RadiusInPrimeVertical(const double cp, const double sp) const {
    return mAsq / sqrt(mAsq * cp * cp + mBsq * sp * sp);
  }

  // cahced qunatities for perforamnce
  double mAsq;  // Semi-major axis length squared [m^2]
  double mB;    // Semi-minor axis length [m]
  double mBsq;  // Semi-minor axis length squared [m^2]
  double mBsq_Asq;  // B^2 / A^2

  Point3d mLsrOriginXYZ{0.0, 0.0, 0.0}; // LSR origin XYZ position
  cv::Mat1d mL2E{ cv::Mat1d::eye(3, 3) }; // 3x3 rotation matrix from LSR to ECEF
};
} // namespace earthxpert