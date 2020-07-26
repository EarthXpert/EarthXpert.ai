/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "rectification/epipolar/utils.h"
#include "boost/format.hpp"

#include <numeric>
#include <sstream>

namespace earthxpert {
std::string ToString(const EpiGenPerf* perf) {
  std::stringstream ss;
  ss << boost::format("** Performance (%1% tiles) **\n"
    "Total time(s): %2%\n  Read %3%\n  Process: %4%\n    Rigorous: %5%\n"
    "    Densify:  %6%\n    Resample: %7%\n  Write: %8%\n") % \
    perf->numTiles % perf->tmTotal % perf->tmRead % perf->tmProcess % \
    perf->tmRigorous % perf->tmDensify % perf->tmResample % perf->tmWrite;

  const auto totalPix = 
    std::accumulate(perf->pixels.begin(), perf->pixels.end(), 0ULL);
  if (0 < totalPix) {
    ss << boost::format("Total pixels: %1%\n  Bkgd: %2%\n  Norm %3%\n  Fast: %4%\n") % \
      totalPix % (double(perf->pixBkgd) / totalPix) % \
      (double(perf->pixNorm) / totalPix) % (double(perf->pixFast) / totalPix);
  }

  return ss.str();
}
}
