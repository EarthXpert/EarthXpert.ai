/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include <type_traits>

namespace earthxpert {
/**
 *  The structure SpatialCube contains the offset and size information
 *  of a contineous 3D range.
 *
 *  \param Ty value type of coordinates
 */
template <typename Ty> struct SpatialCube {
  static_assert(std::is_arithmetic_v<Ty>, "Requires that template argument be integral or floating data type");

  Ty minX{};    // x coordinate of lower-left corner
  Ty minY{};    // y coordinate of lower-left corner
  Ty minZ{};    // z coordinate of lower-left corner
  Ty length{};
  Ty width{};
  Ty height{};

  bool operator>=(const SpatialCube<Ty>& rhs) const { return ImproperContain(rhs); }

  void SetMinXYZ(const Ty minX, const Ty minY, const Ty minZ) {
    this->minX = minX;
    this->minY = minY;
    this->minZ = minZ;
  }

  void SetSize(const Ty length, const Ty width, const Ty height) {
    this->length = length;
    this->width = width;
    this->height = height;
  }

  void SetBuffers(const Ty bufferLength, const Ty bufferWidth, const Ty bufferHeight) {
    minX -= bufferLength;
    minY -= bufferWidth;
    minZ -= bufferHeight;
    length += bufferLength * 2;
    width += bufferWidth * 2;
    height += bufferHeight * 2;
  }

  inline bool IsValid() const noexcept { 
    return (0 < length && 0 < width && 0 < height); 
  }

  inline Ty GetVolume() const noexcept { return length * width * height; }

  /**
   *  \param [in] rhs Object to test
   *  \return Return true if containing the other object improperly, false otherwise.
   */
  bool ImproperContain(const SpatialCube<Ty>& rhs) const {
    if (minX > rhs.minX) return false;
    if (minY > rhs.minY) return false;
    if (minZ > rhs.minZ) return false;
    if (minX + length < rhs.minX + rhs.length) return false;
    if (minY + width < rhs.minY + rhs.width) return false;
    if (minZ + height < rhs.minZ + rhs.height) return false;
    return true;
  }
};

using SpatialCubeF = SpatialCube<float>;
using SpatialCubeD = SpatialCube<double>;
} // namespace earthxpert
