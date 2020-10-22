/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/utils/distortiongrid.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/utils/typehelper.h"

#include <array>
#include <bitset>
#include <tuple>

namespace earthxpert {
/**
 *  The class defines intrinsic parameters for interior orientation.
 *
 *  \detail
 */
class EXSENSORMODEL_EXPORTS FrameIP {
public:
  // index of radial distortion coefficients
  enum RadialCoeff {
    eK0 = 0,        // r balanced
    eK1 = 1,        // r^3
    eK2 = 2,        // r^5
    eK3 = 3,        // r^7
    eK4 = 4,        // r^9  fisheye
    eK5 = 5,        // r^11
    NumRadCoef = 6  // number of coefficients
  };

  // index of decentering distortion coefficients
  enum DecenterCoeff {
    eP1 = 0,        //
    eP2 = 1,        //
    eP3 = 2,        //
    eP4 = 3,        //
    NumDecCoef = 4  // number of coefficients
  };

  // index of linear distortion coefficients
  enum LinearCoeff {
    eB1 = 0,        // non-square
    eB2 = 1,        // non-orthogonal
    NumLinCoef = 2  // number of coefficients
  };

  // position of distortion flags in bitset
  static constexpr size_t kRADDIST = 0; // radial
  static constexpr size_t kDECDIST = 1; // decentering
  static constexpr size_t kLINDIST = 2; // linear
  static constexpr size_t kGRDDIST = 3; // grid

  // self-calibration: distortion correction algorithm
  enum CalibAlgo {
    eSTANDARD, // standard correction with full focal plane coordinates [mm]
    eENHANCED  // enhanced correction with normalized focal plane coordinates
  };

  /**
   * Constructor
   *
   * \param [in] focalLength Focal length [mm]
   * \param [in] ppa principla point of autocollimation [mm]
   */
  FrameIP(std::string camName, double focalLength,
          const std::array<double, 2> &ppa);
  FrameIP(std::string camName, const double focalLength,
          const std::array<double, 2> &ppa,
          const std::array<double, NumRadCoef> &radialDist,
          const std::array<double, NumDecCoef> &decentDist);
  FrameIP(std::string camName, const double focalLength,
          const std::array<double, 2> &ppa,
          const std::array<double, NumRadCoef> &radialDist,
          const std::array<double, NumDecCoef> &decentDist,
          const std::array<double, NumLinCoef> &linearDist);
  FrameIP(const double focalLength, const std::array<double, 2> &ppa,
          std::unique_ptr<DistortionGridF> &&gridDist);

  FrameIP(const FrameIP &rhs);
  FrameIP(FrameIP &&rhs) noexcept;
  ~FrameIP() = default;

  FrameIP &operator=(const FrameIP &rhs);
  FrameIP &operator=(FrameIP &&rhs) noexcept;

  bool operator==(const FrameIP &rhs) const;
  bool operator!=(const FrameIP &rhs) const;

  void SetMisalignment(const std::array<double, 3> &pc,
                       const std::array<double, 3> &imu);

  double GetFocalLength() const noexcept { return mFocalLength; }
  std::array<double, 2> GetPPA() const noexcept { return mPPA; }
  auto GetPerspectiveCenter() const noexcept { return mPC.data(); }
  auto GetMisalignment() const noexcept { return mIMU.data(); }
  auto GetRadialDist() const noexcept { return mRadialDist.data(); }
  auto GetDecentDist() const noexcept { return mDecentDist.data(); }
  auto GetLinearDist() const noexcept { return mLinearDist.data(); }
  const auto GetGridDist() const noexcept { return mGridDist.get(); }

  Point2d CalcRadialCorr(const Point2d& sensor) const;
  Point2d CalcDecentCorr(const Point2d& sensor) const;
  Point2d CalcLinearCorr(const Point2d& sensor) const;
  Point2d CalcGridCorr(const Point2d& sensor) const;

  Point2d PPSToPPA(const Point2d& sensor) const {
    return { sensor.x - mPPA[0], sensor.y - mPPA[1] };
  }

  Point2d PPAToPPS(const Point2d& sensor) const {
    return { sensor.x + mPPA[0], sensor.y + mPPA[1] };
  }

  /**
   *  \return Return distortion flags of radial, decentering, grid.
   */
  Tuple4<bool> GetDistFlags() const noexcept {
    return { mDistFlags[kRADDIST], mDistFlags[kDECDIST],
      mDistFlags[kLINDIST], mDistFlags[kGRDDIST] };
  }

private:
  std::string mCamName;         // camera name
  double mFocalLength{};        // calibrated principal distance
  std::array<double, 2> mPPA{}; // principla point of autocollimation

  std::array<double, 3> mPC{};  // perspective center
  std::array<double, 3> mIMU{}; // IMU misalignment for boresighting [degree]

  std::array<double, NumRadCoef> mRadialDist{};  // radial distortion [K0 K1 K2 K3 K4 K5]
  std::array<double, NumDecCoef> mDecentDist{};  // decentering distortion [P1 P2 P3 P4]
  std::array<double, NumLinCoef> mLinearDist{};  // linear distortion for non-square, non-orthogonality [B1 B2]
  std::unique_ptr<DistortionGridF> mGridDist;   // grid distortion

  // self-calibration model with added parameters

  // uncertainties [mm]
  double mStdFL{};
  std::array<double, 2> mStdPPA{};
  std::array<double, NumRadCoef> mStdRadialDist{};
  std::array<double, NumDecCoef> mStdDecentDist{};
  std::array<double, NumLinCoef> mStdLinearDist{};
  // \todo covariance between calibration parameters
  std::array<double, 3> mStdPC{};
  std::array<double, 3> mStdIMU{};

  void CheckCamParms() const;
  void CheckDistortions() const;
  void CheckMisalignment() const;

  void CopyFrom(const FrameIP &rhs);
  void MoveFrom(FrameIP &&rhs) noexcept;
  void Clear() noexcept;

  // cached info
  mutable std::bitset<4> mDistFlags; // [Grid Linear Dec Rad]
};
} // namespace earthxpert
