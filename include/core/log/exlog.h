/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/glog/exglog.h"
#include "3rdparty/stl/exstl.h"

#include <string>

namespace earthxpert {
class EXCORE_EXPORTS ExLog {
public:
  ExLog();
  ~ExLog() = default;

  void Log(const schrono::time_point<schrono::steady_clock>& time, const std::string& msg);
  void LogSS(const double time, const std::string& msg);
};
}
