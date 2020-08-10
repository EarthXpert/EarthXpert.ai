/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/exsfm.h"

#include <algorithm>
#include <array>
#include <string>

namespace earthxpert {
//
struct FeatDetectPerf {
  union {  // images
    struct {
      int numCols;
      int numRows;
    };
    std::array<int, 2> images;
  };

  union {  // tiles
    struct {
      int numTiles;
      int numImgTiles;
    };
    std::array<int, 2> tiles;
  };

  union {  // timing
    struct {
      double tmTotal;   // total run time
      double tmInitial;
      double tmRead;
      double tmProcess;
      double tmWrite;
    };
    std::array<double, 5> timing;
  };

  FeatDetectPerf::FeatDetectPerf() { Clear(); }

  void Clear() noexcept {
    images.fill(0);
    tiles.fill(0);
    timing.fill(0.0);
  }
};

std::string EXSFM_EXPORTS ToString(const FeatDetectPerf* perf);
}
