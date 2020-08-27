/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "imageproc/eximageproc.h"
#include "core/datasource/iraster.h"

#include <memory>

namespace earthxpert {
bool EXIMAGEPROC_EXPORTS WallisFilter(const IRaster* src, IRaster* tgt);
}
