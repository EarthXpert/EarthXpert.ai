/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "core/coretypes.h"
#include "core/utils/regiontile.h"

#include <cassert>
#include <vector>

namespace earthxpert {
enum class LayoutKind;

/**
 *  The class RegionLayout formulate the non-overlapping tiling of 2D
 *  rectangular area in Cartician coordinate system, including 
 *  the bottom & the left edges, excluding the top & the right edges.
 *
 *  \param Ty value type of coordinates
 */
template <typename Ty> class EXCORE_EXPORTS RegionLayout {
public:
  using ValueType = Ty;

  RegionLayout(Ty width, Ty height,
    Ty tileWidth, Ty tileHeight, LayoutKind kind = LayoutKind::eHORIZONTAL);
  RegionLayout(Ty xmin, Ty ymin, Ty width, Ty height,
    Ty tileWidth, Ty tileHeight, LayoutKind kind = LayoutKind::eHORIZONTAL);
  ~RegionLayout() {}

  int SetTileSize(Ty tileWidth, Ty tileHeight);
  int SetNumTiles(int numTiles);

  int GetNumTiles() const {
    return static_cast<int>(mTiles.size());
  }

  /**
   *  \param [in] index 0-based position
   *  \return Returns const reference to the RegionTile object at given position.
   */
  const auto& GetRegionTile(const int index) {
    assert(0 <= index && index < mTiles.size());
    return mTiles[index];
  }

  /**
   *  \return Returns const reference to the array of RegionTile objects.
   */
  const auto& GetRegionTiles() const {
    return mTiles;
  }

private:
  Ty mXMin{};          // x coordinate at the lower-left corner of region
  Ty mYmin{};          // y coordinate at the lower-left corner of region
  Ty mWidth{};         // width of region
  Ty mHeight{};        // height of region
  Ty mTileWidth{};     // width of tile
  Ty mTileHeight{};    // height of tile
  LayoutKind mLayoutkind{LayoutKind::eHORIZONTAL};  // major direction
  std::vector<RegionTile<Ty>> mTiles;  // array of tiles formulating the layout

  void GenerateLayout();
};

using RegionLayoutF = RegionLayout<float>;
using RegionLayoutD = RegionLayout<double>;
}
