/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "packages/utils/exutils.h"

#include <string>

namespace earthxpert {
//
struct EXPFUTILS_EXPORTS CommandInfo {
  std::string name;
  std::string description;
  std::string link;
  //functor

  CommandInfo();
};
}
