/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
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
  static_assert(std::is_arithmetic_v<Ty>, "Requires that template argument be integral or floating data type");

public:
  using ValueType = Ty;

  RegionLayout(Ty width, Ty height, Ty tileWidth, Ty tileHeight, 
    LayoutKind kind = LayoutKind::eHORIZONTAL);
  RegionLayout(Ty minX, Ty minY, Ty width, Ty height,
    Ty tileWidth, Ty tileHeight, LayoutKind kind = LayoutKind::eHORIZONTAL);
  RegionLayout(const RegionLayout& rhs) = default;
  RegionLayout(RegionLayout&& rhs) noexcept = default;
  ~RegionLayout() = default;

  RegionLayout& operator=(const RegionLayout& rhs) = default;
  RegionLayout& operator=(RegionLayout&& rhs) noexcept = default;

  int SetTileSize(Ty tileWidth, Ty tileHeight);
  int SetNumTiles(int numTiles);

  int GetNumTiles() const {
    return static_cast<int>(mTiles.size());
  }

  /**
   *  \param [in] index 0-based position
   *  \return Returns const reference to the RegionTile object at given position.
   */
  const auto& GetRegionTile(const int index) const {
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
  Ty mMinX{};          // x coordinate at the lower-left corner of region
  Ty mMinY{};          // y coordinate at the lower-left corner of region
  Ty mWidth{};         // width of region
  Ty mHeight{};        // height of region
  Ty mTileWidth{};     // width of tile
  Ty mTileHeight{};    // height of tile
  LayoutKind mLayoutkind{LayoutKind::eHORIZONTAL};  // major direction
  std::vector<RegionTile<Ty>> mTiles;  // array of tiles formulating the layout

  void LayoutTiles();
};

using RegionLayoutF = RegionLayout<float>;
using RegionLayoutD = RegionLayout<double>;
} // namespace earthxpert
