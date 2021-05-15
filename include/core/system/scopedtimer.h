/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include <chrono>

namespace schrono = std::chrono;

namespace earthxpert {
/**
 *  
 */  
class EXCORE_EXPORTS ScopedTimer {
public:
  using ClockType = schrono::steady_clock;

  ScopedTimer(double& tm) : mStart(ClockType::now()), mTm(tm) {}
  ScopedTimer(const ScopedTimer&) = delete;
  ScopedTimer(ScopedTimer&&) = delete;

  ~ScopedTimer() {
    const schrono::duration<double> elapsed = ClockType::now() - mStart;
    mTm += elapsed.count();
  }

  auto operator=(const ScopedTimer&) -> ScopedTimer& = delete;
  auto operator=(ScopedTimer&&) -> ScopedTimer& = delete;

private:
  const ClockType::time_point mStart{};
  double& mTm;  // ref to elapsed time in seconds
};
}
