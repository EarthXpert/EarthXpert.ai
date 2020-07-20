/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/utils/rasterwindow.h"

#include <utility>
#include <vector>

namespace earthxpert {
constexpr int kNCP = 8; // number of corner points

/**
 *  \return Return the col/row for given linear index of a row-major 2d grid
 */
inline std::pair<int, int> IndexToCR(const int64_t idx, const int numCols) {
  return { int(idx % numCols), int(idx / numCols) };
}

/**
 *  \return Return true if all the four corners are invalid
 */
inline bool MaskAllZero(const uchar* mask, const int64_t i1,
  const int64_t i2, const int64_t i3, const int64_t i4) {
  return (0 == mask[i1] && 0 == mask[i2] && 0 == mask[i3] && 0 == mask[i4]);
}

/**
 *  \return Return true if all the four corners are valid
 */
inline bool MaskAllOne(const uchar* mask, const int64_t i1,
  const int64_t i2, const int64_t i3, const int64_t i4) {
  return (1 == mask[i1] && 1 == mask[i2] && 1 == mask[i3] && 1 == mask[i4]);
}

std::pair<int, int> EXCORE_EXPORTS EstimateTileSize(size_t memSize,
  int bpp, int numCols, int numRows, int kTile=256);

void EXCORE_EXPORTS FillCorners(const RasterWindow& win, 
  std::array<Point2d, kNCP>& corners);

int EXCORE_EXPORTS FillGridNodes(const int num,
  const int stride, std::vector<int>& gridNodes);
}
