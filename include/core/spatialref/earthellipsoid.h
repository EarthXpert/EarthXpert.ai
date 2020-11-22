/* -----------------------------------------------------------------------------
*  Copyright(c) 2019.  All rights reserved.
* ---------------------------------------------------------------------------- -
*/
#pragma once

#include "core/excore.h"
#include "3rdparty/utils/constants.h"
#include "core/math/math.h"
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
  EarthEllipsoid(const std::string& wkt, const Point3d& origin=Point3d());
  EarthEllipsoid(double semiMajor = kWGS84_A, 
    double eccSq = kWGS84_ECCSQ, const Point3d& origin=Point3d());
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

  /**
   * Convert coordinates from LSR to XYZ in EXEF
   *
   * \param [in] lsr LSR XYZ coordinates [m]
   * \return Returns ECEF XYZ coordinates [m]
   */
  inline Point3d LSRToXYZ(const Point3d& lsr) const {
    const cv::Mat E = mL2E * cv::Mat(lsr);
    const Point3d lsrE(E.at<double>(0), E.at<double>(1), E.at<double>(2));
    return (mLsrOriginXYZ + lsrE);
  }
  inline Point3d LSRToXYZ(double X, double Y, double Z) const {
    return LSRToXYZ(Point3d(X, Y, Z));
  }

  /**
   * Convert coordinates from ECEF XYZ to LSR
   *
   * \param [in] ecf ECEF XYZ coordinates [m]
   * \return Returns LSR coordinates [m]
   */
  inline Point3d XYZToLSR(const Point3d& xyz) const {
    const cv::Mat L = mL2E.t() * cv::Mat(xyz - mLsrOriginXYZ);
    return { L.at<double>(0), L.at<double>(1), L.at<double>(2) };
  }
  inline Point3d XYZToLSR(double X, double Y, double Z) const {
    return XYZToLSR(Point3d(X, Y, Z));
  }

  /**
   * Convert coordinates from LSR to LPH.
   *
   * \param [in] lsr LSR XYZ coordinates [m]
   * \return Returns geographic longitude [rad], geographic latitude [rad], ellipsoidal height [m]
   */
  inline Point3d LSRToLPH(const Point3d& lsr) const {
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
  Point3d LPHToLSR(const Point3d& lph) const { return XYZToLSR(LPHToXYZ(lph)); }

  inline Point3d LPHToLSR(double lon, double lat, double height) const {
    return LPHToLSR(Point3d(lon, lat, height));
  }

  void SetLsrOrigin(const Point3d& origin);
  inline Point3d GetLsrOrigin() const { return mLsrOrigin; }

private:
  static constexpr double kTolH{1.0e-5}; // Height tolerance [10um]

  double mA{kWGS84_A};          // Semi-major axis length [m]
  double mEccSq{kWGS84_ECCSQ};  // first Eccentricity squared

  Point3d mLsrOrigin{0.0, 0.0, 0.0}; // LSR origin Longitude/Latitude/Height

  void Initialize(const Point3d& origin);

  /**
   * \param [in] lat Geodetic latitude [rad]

   * \return Returns radius [m] of curvature in the Prime Meridian
   */
  inline double RadiusInPrimeMeridian(const double lat) const {
    const double sp = sin(lat);
    return (mA * mBsq_Asq) / pow(1.0 - mEccSq * sp * sp, 1.5);
  }

  /**
   * \param [in] cp cosine(geodetic latitude)
   *
   * \return Returns radius [m] of curvature in the Prime Vertical terminated by minor axis
   */
  inline double RadiusInPrimeVertical(const double cp) const {
    const double cp2 = cp * cp;
    return mAsq / sqrt(mAsq * cp2 + mBsq * (1.0 - cp2));
  }

  /**
   * \param [in] lat Geodetic latitude [rad]
   *
   * \return Returns radius [m] of curvature in the Prime Vertical terminated by minor axis
   */
  //inline double RadiusInPrimeVertical(const double lat) const {
  //  const double cp = cos(lat);
  //  return RadiusInPrimeVertical(cp);
  //}

  // cahced qunatities for perforamnce
  double mAsq{};  // Semi-major axis length squared [m^2]
  double mB{};    // Semi-minor axis length [m]
  double mBsq{};  // Semi-minor axis length squared [m^2]
  double mBsq_Asq{};  // B^2 / A^2

  Point3d mLsrOriginXYZ{0.0, 0.0, 0.0}; // LSR origin XYZ position
  cv::Mat1d mL2E{ cv::Mat1d::eye(3, 3) }; // 3x3 rotation matrix from LSR to ECEF
};
} // namespace earthxpert