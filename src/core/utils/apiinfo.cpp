/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/apiinfo.h"

#include <boost/format.hpp>
#include <numeric>
#include <sstream>

namespace earthxpert {
std::string GetApiBrand() { return "EarthXpert.ai"; }

void GetApiVersion(int& major, int& minor, int& patch, int& build) {
  major = 2;
  minor = 0;
  patch = 0;
  build = 20191102;
}

std::string GetApiVersion() {
  int major, minor, patch, build;
  GetApiVersion(major, minor, patch, build);

  std::stringstream ss;
  ss << boost::format("%1%.%2%.%3%.%4%") % major % minor % patch % build;

  return ss.str();
}
}
