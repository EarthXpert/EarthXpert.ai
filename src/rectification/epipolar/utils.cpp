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
    "Total: %1%\nRead %2%\nProcess: %3%\nWrite: %4%\n") \
    % perf->tmTotal % perf->tmRead % perf->tmProcess % perf->tmWrite;

  return ss.str();
}
}
