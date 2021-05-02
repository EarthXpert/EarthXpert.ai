/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include <chrono>

namespace schrono = std::chrono;

namespace earthxpert {
class EXCORE_EXPORTS WallClock {
public:
  using ClockType = schrono::steady_clock;

  WallClock() { Start(); }

  /**
   *  \breif reset start time of the clock
   */
  void Start() { mStart = ClockType::now(); }

  /**
   *  \return Return ellapsed time since the start [seconds]
   */
  double TimeSinceStart() const {
    const schrono::duration<double> elapsed = ClockType::now() - mStart;
    return elapsed.count();
  }

private:
  schrono::time_point<ClockType> mStart;
};
}
