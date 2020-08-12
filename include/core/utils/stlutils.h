/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"

#include<functional>

namespace earthxpert::utils {
template<typename Ty>
using UPDx = std::unique_ptr<Ty, std::function<void(Ty*)>>;
}
