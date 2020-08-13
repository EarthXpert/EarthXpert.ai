/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/spatiallayout.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/layoututils.h"

#include <algorithm>

using earthxpert::SpatialLayout;
using earthxpert::SpatialTile;

/**
 *  \param [in] width    width of region
 *  \param [in] height    height of region
 *  \param [in] tileWidth width of tile
 *  \param [in] tileHeight height of tile
 */
template <typename Ty>
SpatialLayout<Ty>::SpatialLayout(const Ty width, const Ty height,
  const Ty tileWidth, const Ty tileHeight, const LayoutKind kind)
  : SpatialLayout(SpatialTile<Ty>{Ty(0), Ty(0), width, height}, tileWidth, tileHeight, kind) {
}

/**
 *  \param [in] minX      x coordinate at the lower-left corner of region
 *  \param [in] minY      y coordinate at the lower-left corner of region
 *  \param [in] width     width of region
 *  \param [in] height    height of region
 *  \param [in] tileWidth width of tile
 *  \param [in] tileHeight height of tile
 */
template <typename Ty>
SpatialLayout<Ty>::SpatialLayout(const SpatialTile<Ty>& range,
  const Ty tileWidth, const Ty tileHeight, const LayoutKind kind)
  : mRange(range), mTileWidth(tileWidth), mTileHeight(tileHeight), mLayoutkind(kind) {
  LayoutTiles();
}

/**
 *  \brief Divides spatial extent into tiles using specified tiling size.
 *
 *  \param [in] tileWidth width of tile
 *  \param [in] tileHeight height of tile
 *  \return Returns number of tiles.
 */
template <typename Ty>
int SpatialLayout<Ty>::SetTileSize(const Ty tileWidth, const Ty tileHeight) {
  mTileWidth = tileWidth;
  mTileHeight = tileHeight;
  LayoutTiles();
  return static_cast<int>(mTiles.size());
}

/**
 *  \brief Divides spatial extent into tiles according to the expected number of tiles.
 *  The tile's width and height are balanced to be close to square.
 *
 *  \param [in] numTiles expected number of tiles
 *  \return Returns actual number of tiles.
 */
template <typename Ty>
int SpatialLayout<Ty>::SetNumTiles(const int numTiles) {
  const auto ratio = double(mRange.width) / mRange.height;
  const auto numSqTiles = sqrt(double(numTiles) / ratio);
  Ty tileWidth = mRange.width / (numSqTiles * ratio);
  Ty tileHeight = mRange.height / numSqTiles;

  //tileWidth = ceil(tileWidth / kTileDim) * kTileDim;
  //tileHeight = ceil(tileHeight / kTileDim) * kTileDim;
  return SetTileSize(tileWidth, tileHeight);
}

/**
 *  \brief Divides spatial extent into tiles.
 */
template <typename Ty> void SpatialLayout<Ty>::LayoutTiles() {
  if (mRange.width < mTileWidth || mRange.height < mTileHeight) {
    throw std::invalid_argument(_("The tile size should be less than or equal to the region size"));
  }
  if (0 == mTileWidth) mTileWidth = mRange.width;
  if (0 == mTileHeight) mTileHeight = mRange.height;

  std::vector<Ty> minXs;
  const int numHoriTiles = FillCornersByStride(mRange.width, mTileWidth, minXs);
  std::vector<Ty> minYs;
  const int numVertTiles = FillCornersByStride(mRange.height, mTileHeight, minYs);

  const int numTiles = numHoriTiles * numVertTiles;

  mTiles.resize(numTiles);
  for (int t = 0; t < numTiles; ++t) {
    int x{}, y{};
    if (LayoutKind::eHORIZONTAL == mLayoutkind) {
      std::tie(x, y) = IndexToCR(t, numHoriTiles);
    }
    else {
      std::tie(y, x) = IndexToCR(t, numVertTiles);
    }
    mTiles[t].SetMinXY(mRange.minX + minXs[x], mRange.minY + minYs[y]);

    const Ty tileWidth = std::min<Ty>(mTileWidth, mRange.width - minXs[x]);
    const Ty tileHeight = std::min<Ty>(mTileHeight, mRange.height - minYs[y]);
    mTiles[t].SetSize(tileWidth, tileHeight);
  }
}

template class earthxpert::SpatialLayout<float>;
template class earthxpert::SpatialLayout<double>;
