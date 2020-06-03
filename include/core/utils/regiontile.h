/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include <type_traits>

namespace earthxpert {
/**
 *  The structure RegionTile contains the offset and size information.
 *
 *  \param Ty value type of coordinates
 */
template <typename Ty> struct RegionTile {
  static_assert(std::is_arithmetic_v<Ty>, "Requires that template argument be integral or floating data type");

  Ty minX{};
  Ty minY{};
  Ty width{};
  Ty height{};

  void SetOffsets(const Ty minX, const Ty minY) {
    this->minX = minX;
    this->minY = minY;
  }

  void SetSize(const Ty width, const Ty height) {
    this->width  = width;
    this->height = height;
  }

  void SetMargins(const Ty marginWidth, const Ty marginHeight) {
    minX -= marginWidth;
    minY -= marginHeight;
    width  += marginWidth * 2;
    height += marginHeight * 2;
  }

  /**
   *  \param [in] rhs Object to test
   *  \return Return true if containing the other object improperly, false otherwise.
   */
  bool ImproperContain(const RegionTile<Ty>& rhs) const {
    if (minX > rhs.minX) return false;
    if (minY > rhs.minY) return false;
    if (minX + width < rhs.minX + rhs.width) return false;
    if (minY + height < rhs.minY + rhs.height) return false;
    return true;
  }
};

using RegionTileF = RegionTile<float>;
using RegionTileD = RegionTile<double>;
} // namespace earthxpert
