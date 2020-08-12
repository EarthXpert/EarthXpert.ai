/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include <type_traits>

namespace earthxpert {
/**
 *  The structure SpatialTile contains the offset and size information
 *  of a contineous 2D range.
 *
 *  \param Ty value type of coordinates
 */
template <typename Ty> struct SpatialTile {
  static_assert(std::is_arithmetic_v<Ty>, "Requires that template argument be integral or floating data type");

  Ty minX{};    // x coordinate of lower-left corner
  Ty minY{};    // y coordinate of lower-left corner
  Ty width{};
  Ty height{};

  bool operator>=(const SpatialTile<Ty>& rhs) const { return ImproperContain(rhs); }

  void SetMinXY(const Ty minX, const Ty minY) {
    this->minX = minX;
    this->minY = minY;
  }

  void SetSize(const Ty width, const Ty height) {
    this->width  = width;
    this->height = height;
  }

  void SetBuffers(const Ty bufferWidth, const Ty bufferHeight) {
    minX -= bufferWidth;
    minY -= bufferHeight;
    width  += bufferWidth * 2;
    height += bufferHeight * 2;
  }

  inline bool IsValid() const noexcept { return (0 < width && 0 < height); }

  inline Ty GetArea() const noexcept { return width * height; }

  /**
   *  \param [in] rhs Object to test
   *  \return Return true if containing the other object improperly, false otherwise.
   */
  bool ImproperContain(const SpatialTile<Ty>& rhs) const {
    if (minX > rhs.minX) return false;
    if (minY > rhs.minY) return false;
    if (minX + width < rhs.minX + rhs.width) return false;
    if (minY + height < rhs.minY + rhs.height) return false;
    return true;
  }
};

using SpatialTileF = SpatialTile<float>;
using SpatialTileD = SpatialTile<double>;
} // namespace earthxpert
