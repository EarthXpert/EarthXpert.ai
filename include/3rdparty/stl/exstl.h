/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"

#include <iostream>

namespace earthxpert {
// 
template<typename Ty>
std::ostream& operator<<(std::ostream& s, const std::vector<Ty>& v) {
  s.put('[');
  char comma[3] = { '\0', ' ', '\0' };
  for (const auto& e : v) {
    s << comma << e;
    comma[0] = ',';
  }
  return s << ']';
}
} // namespace earthxpert
