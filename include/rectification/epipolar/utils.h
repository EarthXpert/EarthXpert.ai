/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/epipolar/exepipolar.h"
#include <string>

namespace earthxpert {
//
struct EpiGenPerf {
  double tmTotal{};
  double tmLayout{};
  double tmReset{};
  double tmRead{};
  double tmProcess{};
  double tmWrite{};

  void Clear() noexcept {
    tmTotal = 0.0;
    tmLayout = 0.0;
    tmReset = 0.0;
    tmRead = 0.0;
    tmProcess = 0.0;
    tmWrite = 0.0;
  }
};

std::string ToString(const EpiGenPerf* perf);
}
