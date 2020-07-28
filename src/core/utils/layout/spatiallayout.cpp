/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/spatiallayout.h"

#include <algorithm>
#include <stdexcept>

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
  : SpatialLayout(0, 0, width, height, tileWidth, tileHeight, kind) {
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
SpatialLayout<Ty>::SpatialLayout(const Ty minX, const Ty minY, 
  const Ty width, const Ty height,
  const Ty tileWidth, const Ty tileHeight, const LayoutKind kind)
  : mMinX(minX), mMinY(minY), mWidth(width), mHeight(height), 
  mTileWidth(tileWidth), mTileHeight(tileHeight), mLayoutkind(kind) {
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
  // \todo
  return static_cast<int>(mTiles.size());
}

/**
 *  \brief Divides spatial extent into tiles.
 */
template <typename Ty>
void SpatialLayout<Ty>::LayoutTiles() {
  if (mWidth < mTileWidth || mHeight < mTileHeight) {
    throw std::invalid_argument("Thw tile size should be less than or equal to the region size");
  }
  if (0 == mTileWidth) mTileWidth = mWidth;
  if (0 == mTileHeight) mTileHeight = mHeight;

  std::vector<Ty> minXs;
  for (Ty x = 0; x < mWidth; x += mTileWidth) {
    minXs.push_back(x);
  }
  std::vector<Ty> minYs;
  for (Ty y = 0; y < mHeight; y += mTileHeight) {
    minYs.push_back(y);
  }

  const int numHoriTiles = int(minXs.size());
  const int numVertTiles = int(minYs.size());
  const int numTiles = numHoriTiles * numVertTiles;

  mTiles.resize(numTiles);
  for (int t = 0; t < numTiles; ++t) {
    int x{}, y{};
    if (LayoutKind::eHORIZONTAL == mLayoutkind) {
      x = t % numHoriTiles;
      y = t / numHoriTiles;
    }
    else {
      x = t / numVertTiles;
      y = t % numVertTiles;
    }
    mTiles[t].SetMinXY(mMinX + minXs[x], mMinY + minYs[y]);

    const Ty tileWidth = std::min<Ty>(mTileWidth, mWidth - minXs[x]);
    const Ty tileHeight = std::min<Ty>(mTileHeight, mHeight - minYs[y]);
    mTiles[t].SetSize(tileWidth, tileHeight);
  }
}

template class earthxpert::SpatialLayout<float>;
template class earthxpert::SpatialLayout<double>;
