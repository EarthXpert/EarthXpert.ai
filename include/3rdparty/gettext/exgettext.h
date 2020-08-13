/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"

#include <libintl.h>
#include <string>

namespace earthxpert {
//#define _(msgid) gettext(msgid)
inline char* _(const char* str) {
  return gettext(str);
}

inline char* _(const std::string& str) {
  return gettext(str.c_str());
}

inline char* _(const std::string_view& str) {
  return gettext(str.data());
}

namespace thirdparty {

}
} // namespace earthxpert::thirdparty
