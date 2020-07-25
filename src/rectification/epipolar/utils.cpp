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
  ss << boost::format("** EpipolarGenerator Performance **\n"
    "Total time(s): %1%\n  Read %2%\n  Process: %3%\n"
    "    Rigorous: %4%\n    Densify:  %5%\n  Write: %6%\n") % \
    perf->tmTotal % perf->tmRead % perf->tmProcess % perf->tmRigorous % \
    (perf->tmProcess- perf->tmRigorous) % perf->tmWrite;

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
