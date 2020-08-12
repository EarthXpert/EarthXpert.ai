/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sfm/sparsematch/featdetectperf.h"

#include <boost/format.hpp>
#include <numeric>
#include <sstream>

namespace earthxpert {
std::string ToString(const FeatDetectPerf* perf) {
  std::stringstream ss;
  ss << boost::format("** Performance (%1% tiles) **\n"
    "Total time(s): %2%\n  Initial: %3%\n  Read: %4%\n  Process: %5%\n  Write: %6%\n") % \
    perf->numTiles % perf->tmTotal % perf->tmInitial % \
    perf->tmRead % perf->tmProcess % perf->tmWrite;

  return ss.str();
}
}
