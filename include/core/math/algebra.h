/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"

namespace earthxpert {
//
enum ReguMethod {
  eLevenberg = 0, // add ratio of diagonal to normal diagonal
  eMarquad = 1    // add value to normal diagonal
};
}
