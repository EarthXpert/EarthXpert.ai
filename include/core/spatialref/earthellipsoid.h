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
class EXCORE_EXPORTS EarthEllipsoid final {
public:
  EarthEllipsoid(const std::string& wkt, const Point3d& origin=Point3d());
  EarthEllipsoid(double semiMajor = kWGS84_A, 
    double eccSq = kWGS84_ECCSQ, const Point3d& origin=Point3d());
  //EarthEllipsoid(double semiMajor = kWGS84_A, double semiMinor = kWGS84_B);
  EarthEllipsoid(const EarthEllipsoid& rhs) = default;
  EarthEllipsoid(EarthEllipsoid&& rhs) noexcept = default;
  ~EarthEllipsoid() = default;

  EarthEllipsoid& operator=(const EarthEllipsoid& rhs) = default;
  EarthEllipsoid& operator=(EarthEllipsoid&& rhs) noexcept = default;

  bool operator==(const EarthEllipsoid& rhs) const;
  bool operator!=(const EarthEllipsoid& rhs) const;

  std::unique_ptr<EarthEllipsoid> Clone() const;

  double GetSemiMajor() const noexcept;
  double GetSemiMinor() const noexcept;
  double GetEccSquared() const noexcept;

  inline Point3d LPHToXYZ(const Point3d& lph) const;
  Point3d LPHToXYZ(double lon, double lat, double height) const;

  inline Point3d XYZToLPH(const Point3d& xyz, int* niters=nullptr) const;
  Point3d XYZToLPH(double X, double Y, double Z, int* niters=nullptr) const;

  /**
   * Convert coordinates from LSR to XYZ in EXEF
   *
   * \param [in] lsr LSR XYZ coordinates [m]
   * \return Returns ECEF XYZ coordinates [m]
   */
  inline Point3d LSRToXYZ(const Point3d& lsr) const;
  inline Point3d LSRToXYZ(double X, double Y, double Z) const;

  /**
   * Convert coordinates from ECEF XYZ to LSR
   *
   * \param [in] ecf ECEF XYZ coordinates [m]
   * \return Returns LSR coordinates [m]
   */
  inline Point3d XYZToLSR(const Point3d& xyz) const;
  inline Point3d XYZToLSR(double X, double Y, double Z) const;

  /**
   * Convert coordinates from LSR to LPH.
   *
   * \param [in] lsr LSR XYZ coordinates [m]
   * \return Returns geographic longitude [rad], geographic latitude [rad], ellipsoidal height [m]
   */
  inline Point3d LSRToLPH(const Point3d& lsr) const;

  inline Point3d LSRToLPH(double X, double Y, double Z) const;

  /**
   * Convert coordinates from the Geographic LPH to LSR
   *
   * \param [in] lph Longitude [rad], Latitude [rad], Ellipsoidal height [m]
   * \return Returns LSR XYZ coordinates [m]
   */
  Point3d LPHToLSR(const Point3d& lph) const;

  inline Point3d LPHToLSR(double lon, double lat, double height) const;

  void SetLsrOrigin(const Point3d& origin);
  inline Point3d GetLsrOrigin() const { return mLsrOrigin; }

  Point3d RayIntersect(const Point3d& origin, const Point3d& ray, double height) const;
  Point3d RayIntersect0(const Point3d& origin, const Point3d& ray, double height) const;

private:
  static constexpr double kTolH{ 1.0e-5 }; // Height tolerance [10um]
  static constexpr double kTolHRI{ 1.0e-4 }; // Height tolerance [0.1mm]
  static constexpr int kMaxIterRI{ 5 };   // 

  double mA{kWGS84_A};          // Semi-major axis length [m]
  double mEccSq{kWGS84_ECCSQ};  // first Eccentricity squared

  Point3d mLsrOrigin{0.0, 0.0, 0.0}; // LSR origin Longitude/Latitude/Height

  void Initialize(const Point3d& origin);

  /**
   * \param [in] lat Geodetic latitude [rad]

   * \return Returns radius [m] of curvature in the Prime Meridian
   */
  inline double RadiusInPrimeMeridian(const double lat) const;

  /**
   * \param [in] cp cosine(geodetic latitude)
   *
   * \return Returns radius [m] of curvature in the Prime Vertical terminated by minor axis
   */
  inline double RadiusInPrimeVertical(const double cp) const;

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

#include "detail/earthellipsoid.hh"
} // namespace earthxpert