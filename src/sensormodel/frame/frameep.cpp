/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/frameep.h"
#include "3rd_party/gdal/exgdal.h"

#include <cmath>
#include <limits>
#include <string>

using earthxpert::FrameEP;
using earthxpert::SpatialReferenceSystem;

/**
 * Constructor
 *
 * \param [in] position Sensor body position in given SRS
 * \param [in] rotation world-to-body rotation angles (Roll/Pitch/Yaw) [degree]
 * \param [in] groundSRS ground coordinate system
 */
FrameEP::FrameEP(const std::array<double, 3> &position,
                 const std::array<double, 3> &rotation,
                 std::shared_ptr<SpatialReferenceSystem> groundSRS)
    : mPosition(position), mRotation(rotation), mGroundSRS(groundSRS) {
  CheckSRS();
}

/**
 * Copy constructor
 *
 * \param [in] rhs Object to copy from
 */
FrameEP::FrameEP(const FrameEP &rhs) {
  CopyFrom(rhs);
}

/**
 * Move constructor
 *
 * \param [in] rhs Object to move from
 */
FrameEP::FrameEP(FrameEP &&rhs) noexcept {
  MoveFrom(std::move(rhs));
}

/**
 * Copy assignment operator
 *
 * \param [in] rhs Object to copy from
 * \return A reference to the modified object
 */
FrameEP &FrameEP::operator=(const FrameEP &rhs) {
  if (this != &rhs) {
    CopyFrom(rhs);
  }
  return *this;
}

/**
 * Move assignment operator
 *
 * \param [in] rhs Object to move from
 * \return A reference to the modified object
 */
FrameEP &FrameEP::operator=(FrameEP &&rhs) noexcept {
  if (this != &rhs) {
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
bool FrameEP::operator==(const FrameEP &rhs) const {
  if (this != &rhs) {
    constexpr double tol = std::numeric_limits<double>::min();

    for (int i = 0; i < 3; ++i) {
      if (tol < fabs(mPosition[i] - rhs.mPosition[i]))
        return false;
      if (tol < fabs(mRotation[i] - rhs.mRotation[i]))
        return false;
    }

    if ((!mGroundSRS) != (!rhs.mGroundSRS))
      return false;
    if (bool(mGroundSRS) && (*mGroundSRS != *rhs.mGroundSRS))
      return false;
  }
  return true;
}

/**
 * Inequality test
 *
 * \param [in] rhs The object to compare with
 * \return true if they are different, false otherwise
 */
bool FrameEP::operator!=(const FrameEP &rhs) const {
  return !(operator==(rhs));
}

void FrameEP::CheckSRS() const { }

/**
 * Copy values from another object
 *
 * \param [in] rhs Object to copy from
 */
void FrameEP::CopyFrom(const FrameEP &rhs) {
  mPosition = rhs.mPosition;
  mRotation = rhs.mRotation;
  mGroundSRS = rhs.mGroundSRS;
}

/**
 * Move values from another object
 *
 * \param [in/out] rhs Object to move from
 */
void FrameEP::MoveFrom(FrameEP &&rhs) noexcept {
  mPosition = rhs.mPosition;
  mRotation = rhs.mRotation;
  mGroundSRS = std::move(rhs.mGroundSRS);
}
