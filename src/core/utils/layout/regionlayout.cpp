/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/regionlayout.h"

#include <algorithm>
#include <stdexcept>

using namespace earthxpert;

/**
 *  \param [in] width    width of region
 *  \param [in] height    height of region
 *  \param [in] tileWidth width of tile
 *  \param [in] tileHeight height of tile
 */
template <typename Ty>
RegionLayout<Ty>::RegionLayout(const Ty width, const Ty height,
  const Ty tileWidth, const Ty tileHeight, const LayoutKind kind)
  : RegionLayout(0, 0, width, height, tileWidth, tileHeight, kind) {
}

/**
 *  \param [in] xmin      x coordinate at the lower-left corner of region
 *  \param [in] ymin      y coordinate at the lower-left corner of region
 *  \param [in] width     width of region
 *  \param [in] height    height of region
 *  \param [in] tileWidth width of tile
 *  \param [in] tileHeight height of tile
 */
template <typename Ty>
RegionLayout<Ty>::RegionLayout(const Ty xmin, const Ty ymin, 
  const Ty width, const Ty height,
  const Ty tileWidth, const Ty tileHeight, const LayoutKind kind)
  : mXMin(xmin), mYMin(ymin), mWidth(width), mHeight(height), 
  mTileWidth(tileWidth), mTileHeight(tileHeight), mLayoutkind(kind) {
  GenerateLayout();
}

/**
 *  \brief Divides region extent into tiles using specified tiling size.
 *
 *  \param [in] tileWidth width of tile
 *  \param [in] tileHeight height of tile
 *  \return Returns number of tiles.
 */
template <typename Ty>
int RegionLayout<Ty>::SetTileSize(const Ty tileWidth, const Ty tileHeight) {
  mTileWidth = tileWidth;
  mTileHeight = tileHeight;
  GenerateLayout();
  return static_cast<int>(mTiles.size());
}

/**
 *  \brief Divides region extent into tiles according to the expected number of tiles.
 *  The tile's width and height are balanced to be close to square.
 *
 *  \param [in] numTiles expected number of tiles
 *  \return Returns actual number of tiles.
 */
template <typename Ty>
int RegionLayout<Ty>::SetNumTiles(const int numTiles) {
  // \todo
  return static_cast<int>(mTiles.size());
}

/**
 *  \brief Divides region extent into tiles.
 */
template <typename Ty>
void RegionLayout<Ty>::GenerateLayout() {
  if (mWidth < mTileWidth || mHeight < mTileHeight) {
    throw std::invalid_argument("Thw tile size should be less than or equal to the region size");
  }
  if (0 == mTileWidth) mTileWidth = mWidth;
  if (0 == mTileHeight) mTileHeight = mHeight;

  if (LayoutKind::eHORIZONTAL == mLayoutkind) {
    for (double h = 0; h < mHeight; h += mTileHeight)
    {
      double tileHeight = std::min(mTileHeight, mHeight - h);

      for (double w = 0; w < mWidth; w += mTileWidth)
      {
        double tileWidth = std::min(mTileWidth, mWidth - w);

        mTiles.push_back(RegionTile{ mXMin + w, mYMin + h, tileWidth, tileHeight });
      }
    }
  }
  else {
    for (double w = 0; w < mWidth; w += mTileWidth)
    {
      double tileWidth = std::min(mTileWidth, mWidth - w);

      for (double h = 0; h < mHeight; h += mTileHeight)
      {
        double tileHeight = std::min(mTileHeight, mHeight - h);

        mTiles.push_back(RegionTile{ mXMin + w, mYMin + h, tileWidth, tileHeight });
      }
    }
  }
}

template<> class earthxpert::RegionLayout<float>;
template<> class earthxpert::RegionLayout<double>;
