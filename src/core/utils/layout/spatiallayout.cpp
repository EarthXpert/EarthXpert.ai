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

  if (LayoutKind::eHORIZONTAL == mLayoutkind) {
    for (Ty h = 0; h < mHeight; h += mTileHeight) {
      Ty tileHeight = std::min<Ty>(mTileHeight, mHeight - h);

      for (Ty w = 0; w < mWidth; w += mTileWidth) {
        Ty tileWidth = std::min<Ty>(mTileWidth, mWidth - w);

        mTiles.push_back(SpatialTile<Ty>{ mMinX + w, mMinY + h, tileWidth, tileHeight });
      }
    }
  }
  else {
    for (Ty w = 0; w < mWidth; w += mTileWidth) {
      Ty tileWidth = std::min<Ty>(mTileWidth, mWidth - w);

      for (Ty h = 0; h < mHeight; h += mTileHeight) {
        Ty tileHeight = std::min<Ty>(mTileHeight, mHeight - h);

        mTiles.push_back(SpatialTile<Ty>{ mMinX + w, mMinY + h, tileWidth, tileHeight });
      }
    }
  }
}

template class earthxpert::SpatialLayout<float>;
template class earthxpert::SpatialLayout<double>;
