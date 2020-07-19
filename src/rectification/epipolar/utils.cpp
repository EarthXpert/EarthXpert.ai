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
    "Total: %1%\n  Read %2%\n  Process: %3%\n  Write: %4%\n") % \
    perf->tmTotal % perf->tmRead % perf->tmProcess % perf->tmWrite;

  return ss.str();
}
}
