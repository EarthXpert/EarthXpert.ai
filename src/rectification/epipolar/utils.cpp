/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "rectification/epipolar/utils.h"
#include "boost/format.hpp"
#include <sstream>

namespace earthxpert {
std::string ToString(const EpiGenPerf* perf) {
  std::stringstream ss;
  ss << boost::format("** EpipolarGenerator Performance **\n"
    "Total: %1%\nLayout: %2%\nReset: %3%\nRead %4%\nProcess: %5%\nWrite: %6%\n") % \
    perf->tmTotal % perf->tmLayout % perf->tmReset % \
    perf->tmRead % perf->tmProcess % perf->tmWrite;

  return ss.str();
}
}
