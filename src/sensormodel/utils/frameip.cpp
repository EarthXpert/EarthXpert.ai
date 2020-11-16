/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/utils/frameip.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/exceptions/exexcept.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>

using namespace earthxpert;

/**
 * Constructor
 *
 * \param [in] focalLength Focal length [mm]
 * \param [in] ppa principla point of autocollimation [mm]
 */
FrameIP::FrameIP(std::string camName, const double focalLength,
  const std::array<double, 2> &ppa) : mCamName(std::move(camName)), 
    mFocalLength(focalLength), mPPA(ppa) {
  CheckCamParms();
}

/**
 * Constructor
 *
 * \param [in] focalLength Focal length [mm]
 * \param [in] ppa principla point of autocollimation [mm]
 * \param [in] radialDist Eight coefficients of radial distortion
 * \param [in] decentDist Four coefficients of decentering distortion
 */
FrameIP::FrameIP(std::string camName, const double focalLength,
                 const std::array<double, 2> &ppa,
                 const std::array<double, NumRadCoef> &radialDist,
                 const std::array<double, NumDecCoef> &decentDist)
    : FrameIP(std::move(camName), focalLength, ppa,
              radialDist, decentDist, {0.0, 0.0}) {
}

/**
 * Constructor
 *
 * \param [in] focalLength Focal length [mm]
 * \param [in] ppa principla point of autocollimation [mm]
 * \param [in] radialDist Eight coefficients of radial distortion
 * \param [in] decentDist Four coefficients of decentering distortion
 * \param [in] linearDist Two coefficients of linear distortion
 */
FrameIP::FrameIP(std::string camName, const double focalLength,
                 const std::array<double, 2> &ppa,
                 const std::array<double, NumRadCoef> &radialDist,
                 const std::array<double, NumDecCoef> &decentDist,
                 const std::array<double, NumLinCoef> &linearDist)
    : FrameIP(std::move(camName), focalLength, ppa) {
  mRadialDist = radialDist;
  mDecentDist = decentDist;
  mLinearDist = linearDist;
  CheckDistortions();
}

/**
 * Constructor
 *
 * \param [in] focalLength Focal length [mm]
 * \param [in] ppa principla point of autocollimation [mm]
 * \param [in] radialDist Eight coefficients of radial distortion
 * \param [in] decentDist Four coefficients of decentering distortion
 */
FrameIP::FrameIP(const double focalLength, const std::array<double, 2> &ppa,
                 std::unique_ptr<DistortionGridF> &&gridDist)
    : FrameIP("", focalLength, ppa) {
  mGridDist = std::move(gridDist);
  CheckDistortions();
}

/**
 * Copy constructor
 *
 * \param [in] rhs The object to copy from
 */
FrameIP::FrameIP(const FrameIP &rhs) {
  CopyFrom(rhs);
}

/**
 * Move constructor
 *
 * \param [in/out] rhs The object to move from
 */
FrameIP::FrameIP(FrameIP &&rhs) noexcept {
  MoveFrom(std::move(rhs));
}

/**
 * Assignment operator
 *
 * \param [in] rhs The object to copy from
 * \return A reference to the modified object
 */
FrameIP &FrameIP::operator=(const FrameIP &rhs) {
  if (&rhs != this) {
    CopyFrom(rhs);
  }
  return *this;
}

/**
 * Move assignment operator
 *
 * \param [in] rhs The object to move from
 * \return A reference to the modified object
 */
FrameIP &FrameIP::operator=(FrameIP &&rhs) noexcept {
  if (&rhs != this) {
    MoveFrom(std::move(rhs));
  }
  return *this;
}

/**
 * Comparison operator
 *
 * \param [in] rhs The object to compare with
 * \return true if they are the same, false otherwise
 */
bool FrameIP::operator==(const FrameIP &rhs) const {
  if (this != &rhs) {
    const EqualD equalD(std::numeric_limits<double>::min());

    if (!equalD(mFocalLength, rhs.mFocalLength))
      return false;
    if (!std::equal(std::cbegin(mPPA), std::cend(mPPA),
      std::cbegin(rhs.mPPA), equalD))
      return false;
    if (!std::equal(std::cbegin(mPC), std::cend(mPC),
      std::cbegin(rhs.mPC), equalD))
      return false;
    if (!std::equal(std::cbegin(mIMU), std::cend(mIMU),
      std::cbegin(rhs.mIMU), equalD))
      return false;
    if (!std::equal(std::cbegin(mRadialDist), std::cend(mRadialDist),
      std::cbegin(rhs.mRadialDist), equalD))
      return false;
    if (!std::equal(std::cbegin(mDecentDist), std::cend(mDecentDist),
      std::cbegin(rhs.mDecentDist), equalD))
      return false;
    if (!std::equal(std::cbegin(mLinearDist), std::cend(mLinearDist),
      std::cbegin(rhs.mLinearDist), equalD))
      return false;
    if (!mGridDist != !rhs.mGridDist)
      return false;
    if (mGridDist && mGridDist != rhs.mGridDist)
      return false;
  }
  return true;
}

/**
 * Inequality operator
 *
 * \param [in] rhs The object to compare with
 * \return true if they are different, false otherwise
 */
bool FrameIP::operator!=(const FrameIP &rhs) const {
  return !(operator==(rhs));
}

/**
 *  \brief set the position of perspective center in body coordinate system
 *      and the  misalignment between boresighting and IMU.
 *
 * \param [in] pc XYZ coordinates of the perspective centre
 *      as expressed in the body coordinate system
 * \param [in] imu the angular rotation between the boresight and IMU
 */
void FrameIP::SetMisalignment(const std::array<double, 3> &pc,
                              const std::array<double, 3> &imu) {
  mPC = pc;
  mIMU = imu;
}

/**
 *  \brief Compute correction to radial distortion.
 *
 * \param [in] sensor the measured position where the ray actually hits the
 * focal plane
 * \return the radial correction.
 */
Point2d FrameIP::CalcRadialCorr(const Point2d& sensor) const {
  if (!mDistFlags[kRADDIST])
    return { 0.0, 0.0 };

  const auto rc2 = cv::normL2Sqr<double>(sensor);

  double rcPow = 1.0;
  auto dRrad = mRadialDist[0];
  for (int i = 1; i < NumRadCoef; ++i) {
    rcPow *= rc2;
    dRrad += mRadialDist[i] * rcPow;
  }

  return (sensor * dRrad);
}

/**
 *  \brief Compute correction to decentering distortion.
 *
 * \param [in] sensor the measured position where the ray actually hits the
 * focal plane
 * \return the decentering correction.
 */
Point2d FrameIP::CalcDecentCorr(const Point2d& sensor) const {
  if (!mDistFlags[kDECDIST])
    return { 0.0, 0.0 };

  const auto x2 = sensor.x * sensor.x;
  const auto y2 = sensor.y * sensor.y;
  const auto rc2 = x2 + y2;
  const auto rc4 = rc2 * rc2;

  const auto factor = (1.0 + mDecentDist[eP3] * rc2 + mDecentDist[eP4] * rc4);
  const auto xy = sensor.x * sensor.y;

  return { (mDecentDist[eP1] * (rc2 + 2.0 * x2) + 2.0 * mDecentDist[eP2] * xy) * factor,
           (mDecentDist[eP2] * (rc2 + 2.0 * y2) + 2.0 * mDecentDist[eP1] * xy) * factor };
}

/**
 * Validate the paramters defining the transformation between the focal plane
 * and the camera coordinate frame.
 *
 * @throw if focalLength is non-positive.
 */
void FrameIP::CheckCamParms() const {
  if (0.0 >= mFocalLength) 
    throw std::invalid_argument(_("Focal length must be a positive number"));
}

/**
 *  \brief Validate the paramters defining radial, decentring, and/or grid
 * distortions.
 */
void FrameIP::CheckDistortions() const {
  auto NotZero = [](const double val) -> bool { return (0.0 != val); };

  mDistFlags.set(kRADDIST, std::any_of(std::cbegin(mRadialDist),
    std::cend(mRadialDist), NotZero));
  mDistFlags.set(kDECDIST, std::any_of(std::cbegin(mDecentDist),
    std::cend(mDecentDist), NotZero));
  mDistFlags.set(kLINDIST, std::any_of(std::cbegin(mLinearDist),
    std::cend(mLinearDist), NotZero));
  if (mGridDist)
    mDistFlags.set(kGRDDIST, mGridDist->HasGrid());

  // the radial distortion formula is invalid outside the frame,
  // use linear extrapolation to obtain a rough distortion value
  if (mDistFlags.test(kRADDIST)) {

  }
}

/**
 *  \brief Validate the boresighting.
 */
void FrameIP::CheckMisalignment() const { }

/**
 * Copy data from one object to another
 *
 * \param [in] rhs The object to copy from
 */
void FrameIP::CopyFrom(const FrameIP &rhs) {
  mCamName = rhs.mCamName;
  mFocalLength = rhs.mFocalLength;
  mPPA = rhs.mPPA;
  mPC = rhs.mPC;
  mIMU = rhs.mIMU;
  mRadialDist = rhs.mRadialDist;
  mDecentDist = rhs.mDecentDist;
  mLinearDist = rhs.mLinearDist;
  mGridDist = rhs.mGridDist->Clone();
}

/**
 * Move data from one object to another
 *
 * \param [in] rhs The object to move from
 */
void FrameIP::MoveFrom(FrameIP &&rhs) noexcept {
  mCamName = std::move(rhs.mCamName);
  mFocalLength = rhs.mFocalLength;
  mPPA = rhs.mPPA;
  mPC = rhs.mPC;
  mIMU = rhs.mIMU;
  mRadialDist = rhs.mRadialDist;
  mDecentDist = rhs.mDecentDist;
  mLinearDist = rhs.mLinearDist;
  mGridDist = std::move(rhs.mGridDist);

  rhs.Clear();
}

void FrameIP::Clear() noexcept {
  mFocalLength = 0.0;
  mPPA.fill(0.0);

  mPC.fill(0.0);
  mIMU.fill(0.0);

  mRadialDist.fill(0.0);
  mDecentDist.fill(0.0);
  mLinearDist.fill(0.0);
}
