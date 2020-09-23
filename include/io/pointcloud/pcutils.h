/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/laslib/exlaslib.h"
#include "core/utils/spatialtile.h"

namespace earthxpert {
/**
 *  \param [in] tile the lower-left corner and the size of the tile
 *
 *  \return Return a LASfilter object for the given region.
 */
std::unique_ptr<LASfilter> CreateLasFilter(const SpatialTileD& tile) {
  return thirdparty::CreateLasFilter(tile.minX, tile.minY, tile.width, tile.height);
}
}
