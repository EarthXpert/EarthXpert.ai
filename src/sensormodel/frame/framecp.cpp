/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/framecp.h"

using watxtract::FrameCP;

/**
 * constructor
 */
FrameCP::FrameCP(const bool atRefract, const double earthRadius)
  : mAtRefract(atRefract), mEarthRadius(earthRadius) {}

FrameCP::FrameCP()
  : FrameCP(false, 6378) {}

/**
 * Comparison operator
 *
 * \param [in] rhs The object to compare with
 * \return true if they are the same, false otherwise
 */
bool FrameCP::operator==(const FrameCP &rhs) const {
  if (this != &rhs) {
  }
  return true;
}

/**
 * Comparison operator
 *
 * \param [in] rhs The object to compare with
 * \return true if they are different, true otherwise
 */
bool FrameCP::operator!=(const FrameCP &rhs) const {
  return !(operator==(rhs));
}

/**
 * \param [in] H The flying height [km]
 * \param [in] h The ground height [km]
 * \return Return refraction factor
 */
double FrameCP::GetRefractionK(const double H, const double h) const {
  const double K = 2410.0e-6 * ((H / (H * H - 6.0 * H + 250.0)) -
    (h * h / ((h * h - 6.0 * h + 250.0) * H)));
  return K;
}
