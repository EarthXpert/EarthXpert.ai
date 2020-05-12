/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include <array>
#include <bitset>

namespace watxtract {
using MarginFlag = ::std::bitset<4>;
using MarginFillDim = ::std::array<int, 4>;

/**
 *  The margin kind defines the ways of filling the outof bound pixels in the margin 
 *  when the margin is outsied of the image file.
 */
enum class MarginKind {
  eCONSTANT,    // fill with constant, e.g., 0
  eREFLECTION,  // fill by reflecting using the edge of the image
  eREPLICATION  // fill by duplicating the pixels along the edge of the image
};

/**
 *  The margin position.
 */
enum class MarginSide : size_t {
  eTOP = 0,
  eBOTTOM = 1,
  eLEFT = 2,   
  eRIGHT = 3
};

/**
 *  The pixel type defines the data type and band count hormanizing GDAL & OpenCV releated types.
 */
enum class PixelType {
};
}