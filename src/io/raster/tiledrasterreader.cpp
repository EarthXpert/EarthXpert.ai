/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/raster/tiledrasterreader.h"
#include "3rdparty/gettext/exgettext.h"
#include "core/exceptions/exexcept.h"

using namespace earthxpert;

TiledRasterReader::TiledRasterReader(std::vector<RasterWindow>&& tiles,
  std::vector<std::string>&& imageFiles, const std::vector<int>& bands)
  : mTiles(std::move(tiles)), mImageFiles(std::move(imageFiles)), mBands(bands) {
  Initialize();
}


void TiledRasterReader::Initialize() {
  mNumTiles = int(mTiles.size());
  if (mNumTiles != int(mImageFiles.size()))
    throw std::invalid_argument(_("Incompatible layout"));
}
