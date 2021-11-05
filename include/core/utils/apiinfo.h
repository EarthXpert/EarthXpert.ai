/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include <string>
#include <tuple>

namespace earthxpert {
std::string EXCORE_EXPORTS GetApiBrand();

void EXCORE_EXPORTS GetApiVersion(int& major, int& minor, int& patch, int& build);

std::string EXCORE_EXPORTS GetApiVersion();
}
