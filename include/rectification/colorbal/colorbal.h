/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/colorbal/excolorbal.h"

namespace earthxpert {
struct ColorBalOptions {

};

void EXCOLORBAL_EXPORTS DoColorBalance(const ColorBalOptions& cbOpt);
}
