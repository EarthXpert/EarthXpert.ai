/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include <array>
#include <bitset>

namespace earthxpert {
using MarginFlag = ::std::bitset<4>;
using MarginFillDim = ::std::array<int, 4>;

/**
 *  The margin kind defines the ways of filling the outof bound pixels in the margin
 *  when the margin is outsied of the image file.
 */
enum struct MarginKind {
  eMK_CONSTANT,    // fill with constant, e.g., 0
  eMK_REFLECTION,  // fill by reflecting using the edge of the image
  eMK_REPLICATION  // fill by duplicating the pixels along the edge of the image
};

/**
 *  The margin position.
 */
enum struct MarginSide : size_t {
  eMS_TOP = 0,
  eMS_BOTTOM = 1,
  eMS_LEFT = 2,
  eMS_RIGHT = 3
};

/**
 *  The pixel type defines the data type and band count hormanizing GDAL & OpenCV releated types.
 */
enum struct PixelType {
};
}