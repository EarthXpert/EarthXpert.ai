/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sfm/sparsematch/featuredetect.h"
#include "core/exceptions/exexcept.h"
#include "core/math/constants.h"
#include "core/math/math.h"
#include "core/utils/layoututils.h"
#include "core/utils/rasterlayout.h"

using namespace earthxpert;

/**
 *  \brief setup the tiling strategy under the available memory.
 */
void FeatureDetect::LayoutTiles() {
  const int bpp = mImgBpp;
  const auto [tileNumCols, tileNumRows] = EstimateTileSize(mMemSize,
    bpp, mImgNumCols, mImgNumRows, kTileDim);

  DefineTiles(tileNumCols, tileNumRows);

  AllocateBuffers();
}

/**
 *  \return Return number of valid tiles.
 */
int FeatureDetect::DefineTiles(const int tileNumCols, const int tileNumRows) {
  RasterLayout rasLayout(mImgNumCols, mImgNumRows, tileNumCols, tileNumRows);
  if (kMinNumTiles > rasLayout.GetNumWindows()) {
    rasLayout.SetNumWindows(kMinNumTiles);
  }  
  mNumTiles = rasLayout.GetNumWindows();
  mTiles.resize(mNumTiles);

  for (int i = 0; i < mNumTiles; ++i) {
    mTiles[i].imgTile = rasLayout.GetRasterWindow(i);
  }

  return mNumTiles;
}

/**
 *  \return Returns the max number of pixels to hold any image tile
 *
 *  \param [in] tiles the tiling layout
 */
int64_t FeatureDetect::GetTilesLimit(const std::vector<TileInfo>& tiles) const {
  int64_t maxImgTileNumPix{ -1 };

  for (auto& tile : tiles) {
    if (tile.imgTile.IsValid()) {
      Larger(maxImgTileNumPix, tile.imgTile.GetNumPixels());
    }
  }

  return maxImgTileNumPix;
}
