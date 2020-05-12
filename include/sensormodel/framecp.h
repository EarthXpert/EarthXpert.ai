/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/wxsensormodel.h"

namespace watxtract {
/**
 *  The class defines correction parameters for external effects.
 *
 *  \detail
 */
class WXSENSORMODEL_EXPORTS FrameCP {
public:
  FrameCP();
  FrameCP(bool atRefract, double earthRadius);
  FrameCP(const FrameCP &rhs) = default;
  FrameCP(FrameCP &&rhs) = default;
  ~FrameCP() = default;

  FrameCP &operator=(const FrameCP &rhs) = default;
  FrameCP &operator=(FrameCP &&rhs) = default;

  bool operator==(const FrameCP &rhs) const;
  bool operator!=(const FrameCP &rhs) const;

  bool ApplyRefraction() const { return mAtRefract; }
  bool ApplyCurvature() const { return (0.0 < mEarthRadius); }

  double GetEarthRadius() const { return mEarthRadius; }

  double GetRefractionK(double H, double h) const;

private:
  bool mAtRefract{false};    // atmospheric refration
  double mEarthRadius{6378}; // earth radius [km]
};
} // namespace watxtract
