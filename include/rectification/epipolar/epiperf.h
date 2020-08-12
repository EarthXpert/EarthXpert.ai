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
  union {  // images
    struct {
      int rawNumCols;
      int rawNumRows;
      int epiNumCols;
      int epiNumRows;
    };
    std::array<int, 4> images;
  };

  union {  // tiles
    struct {
      int numTiles;
      int numImgTiles;
    };
    std::array<int, 2> tiles;
  };

  double validRatio{};  // ratio of valid boundary area : epipolar area

  union {  // timing
    struct {
      double tmTotal;   // total run time
      double tmInitial;
      double tmRead;
      double tmProcess;
      double tmRigorous;
      double tmDensify;
      double tmResample;
      double tmWrite;
    };
    std::array<double, 8> timing;
  };

  union {  // cell statistics
    struct {  // number of grid cells
      size_t cellBkgd;   // four grid nodes are background
      size_t cellNorm;
      size_t cellFast;
    };
    std::array<size_t, 3> cells;
  };

  union { // number of pixels
    struct {
      size_t pixBkgd;   // background
      size_t pixNorm;
      size_t pixFast;
    };
    std::array<size_t, 3> pixels;
  };

  EpiGenPerf::EpiGenPerf() { Clear(); }

  void Clear() noexcept {
    images.fill(0);
    tiles.fill(0);
    timing.fill(0.0);
    cells.fill(0);
    pixels.fill(0);
  }
};

std::string EXEPIPOLAR_EXPORTS ToString(const EpiGenPerf* perf);
}
