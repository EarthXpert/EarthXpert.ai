/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/exsensormodel.h"

#include <utility>

namespace earthxpert {
/**
 *  The class defines correction parameters for external effects.
 *
 *  \detail
 */
class EXSENSORMODEL_EXPORTS FrameCP {
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

  void SetHeightOffScale(const double offset, const double scale) {
    mOHeight = offset;
    mSHeight = scale;
  }
  std::pair<double, double> GetHeightOffScale() const noexcept {
    return { mOHeight, mSHeight };
  }

private:
  bool mAtRefract{false};    // atmospheric refration
  double mEarthRadius{6378}; // earth radius [km]

  double mOHeight{};  // offset of ground surface
  double mSHeight{};  // scale of ground surface
};
} // namespace earthxpert
