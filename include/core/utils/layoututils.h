/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"

#include <utility>

namespace earthxpert {
std::pair<int, int> EXCORE_EXPORTS EstimateTileSize(size_t memSize, 
  int bpp, int numCols, int numRows, int kTile=256);
}