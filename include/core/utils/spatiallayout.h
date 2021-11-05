/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "core/coretypes.h"
#include "core/utils/spatialtile.h"

#include <cassert>
#include <vector>

namespace earthxpert {
enum class LayoutDir;

/**
 *  The class SpatialLayout formulate the non-overlapping tiling of 2D
 *  rectangular area in Cartician coordinate system, including 
 *  the bottom & the left edges, excluding the top & the right edges.
 *
 *  \param Ty value type of coordinates
 */
template <typename Ty> class EXCORE_EXPORTS SpatialLayout {
  static_assert(std::is_arithmetic_v<Ty>, "Requires that template argument be integral or floating data type");

public:
  using ValueType = Ty;

  SpatialLayout(Ty width, Ty height, Ty tileWidth, Ty tileHeight, 
    LayoutDir dir = LayoutDir::eLD_HORIZONTAL);
  SpatialLayout(const SpatialTile<Ty>& range, Ty tileWidth, Ty tileHeight, 
    LayoutDir dir = LayoutDir::eLD_HORIZONTAL);
  SpatialLayout(const SpatialLayout& rhs) = default;
  SpatialLayout(SpatialLayout&& rhs) noexcept = default;
  ~SpatialLayout() = default;

  SpatialLayout& operator=(const SpatialLayout& rhs) = default;
  SpatialLayout& operator=(SpatialLayout&& rhs) noexcept = default;

  int SetTileSize(Ty tileWidth, Ty tileHeight);
  int SetNumTiles(int numTiles);

  int GetNumTiles() const { return static_cast<int>(mTiles.size()); }

  /**
   *  \param [in] index 0-based position
   *  \return Returns const reference to the SpatialTile object at given position.
   */
  const auto& GetSpatialTile(const int index) const {
    assert(0 <= index && index < mTiles.size());
    return mTiles[index];
  }

  /**
   *  \return Returns const reference to the array of SpatialTile objects.
   */
  const auto& GetSpatialTiles() const { return mTiles; }

private:
  SpatialTile<Ty> mRange; // spatial range to divide into smaller tiles
  Ty mTileWidth{};     // width of tile
  Ty mTileHeight{};    // height of tile
  LayoutDir mLayoutDir{LayoutDir::eLD_HORIZONTAL};  // major direction
  std::vector<SpatialTile<Ty>> mTiles;  // array of tiles formulating the layout

  void LayoutTiles();
};

using RegionLayoutF = SpatialLayout<float>;
using RegionLayoutD = SpatialLayout<double>;
} // namespace earthxpert
