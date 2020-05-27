/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rd_party/ex3rd_party.h"

#include "LASlib/laszip.hpp"
#include <string>

namespace earthxpert::thirdparty {
void EX3RDPARTY_EXPORTS CreateLAX(const std::string &lasFile);
} // namespace earthxpert::thirdparty
