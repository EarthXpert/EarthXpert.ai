/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/epipolar/exepipolar.h"

#include <algorithm>
#include <array>
#include <string>

namespace earthxpert {
//
struct EpiGenPerf {
  static constexpr size_t kIS = 2;
  static constexpr size_t kTM = 9;
  static constexpr size_t kNS = 3;

  union {  // tiles
    struct {
      int numTiles;
      int numImgTiles;
    };
    std::array<int, kIS> tiles;
  };

  union {  // timing
    struct {
      double tmTotal;   // total run time
      double tmLayout;
      double tmReset;
      double tmRead;
      double tmProcess;
      double tmRigorous;
      double tmDensify;
      double tmResample;
      double tmWrite;
    };
    std::array<double, kTM> timing;
  };

  union {  // cell statistics
    struct {  // number of grid cells
      size_t cellBkgd;   // four grid nodes are background
      size_t cellNorm;
      size_t cellFast;
    };
    std::array<size_t, kNS> cells;
  };

  union { // number of pixels
    struct {
      size_t pixBkgd;   // background
      size_t pixNorm;
      size_t pixFast;
    };
    std::array<size_t, kNS> pixels;
  };

  EpiGenPerf::EpiGenPerf() { Clear(); }

  void Clear() noexcept {
    tiles.fill(0);
    timing.fill(0.0);
    cells.fill(0);
    pixels.fill(0);
  }
};

std::string ToString(const EpiGenPerf* perf);
}
