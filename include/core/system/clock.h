/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include <chrono>

namespace chrono = std::chrono;

namespace earthxpert {
class EXCORE_EXPORTS Clock {
public:
  Clock() { Start(); }

  /**
   *  \breif reset start time of the clock
   */
  void Start() { mStart = chrono::steady_clock::now(); }

  /**
   *  \return Return ellapsed time since the start [seconds]
   */
  double TimeSinceStart() const {
    const chrono::duration<double> elapsed = chrono::steady_clock::now() - mStart;
    return elapsed.count();
  }

private:
  chrono::time_point<chrono::steady_clock> mStart;
};
}
