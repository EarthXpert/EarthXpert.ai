/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "rectification/epipolar/utils.h"

#include <boost/format.hpp>
#include <numeric>
#include <sstream>

namespace earthxpert {
std::string ToString(const EpiGenPerf* perf) {
  std::stringstream ss;
  ss << boost::format("** Performance (%1% tiles) **\n"
    "Total time(s): %2%\n  Initial: %3%\n  Read: %4%\n  Process: %5%\n    "
    "Rigorous: %6%\n    Densify:  %7%\n    Resample: %8%\n  Write: %9%\n") % \
    perf->numTiles % perf->tmTotal % perf->tmInitial % perf->tmRead % perf->tmProcess %\
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
