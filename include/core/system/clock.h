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
  WallClock() { Start(); }

  /**
   *  \breif reset start time of the clock
   */
  void Start() { mStart = schrono::steady_clock::now(); }

  /**
   *  \return Return ellapsed time since the start [seconds]
   */
  double TimeSinceStart() const {
    const schrono::duration<double> elapsed = schrono::steady_clock::now() - mStart;
    return elapsed.count();
  }

private:
  schrono::time_point<schrono::steady_clock> mStart;
};
}
