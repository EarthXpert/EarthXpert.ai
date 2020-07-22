/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/utils/rasterwindow.h"
#include "core/math/math.h"

#include <utility>
#include <vector>

namespace earthxpert {
constexpr int kNCP = 8; // number of corner points

// mask the way to produce the results
enum MaskKind : uint8_t {
  eMK_BKGD = 0, // background
  eMK_NORM,     // normal, e.g. rigorous calculation
  eMK_FAST      // fast, e.g. interpolated from rigorous 
};

/**
 *  \return Return the col/row for given linear index of a row-major 2d grid
 */
inline std::pair<int, int> IndexToCR(const int64_t idx, const int step) {
  return { int(idx % step), int(idx / step) };
}

/**
 *  \return Return the linear index of a row-major 2d grid for given col/row
 */
inline int64_t CRToIndex(const int col, const int row, const int step) {
  return (int64_t(step) * row + col);
}

inline Tuple4<int> GridCorners(const std::vector<int>& gridCols, 
  const std::vector<int>& gridRows, const int gridCol, const int gridRow) {
  assert(gridCol + 1 < gridCols.size() && gridRow + 1 < gridRows.size());
  return { gridCols[gridCol], gridRows[gridRow], 
           gridCols[gridCol + 1], gridRows[gridRow + 1] };
}

template <typename Ty> inline Tuple4<Ty&> MatCorners(cv::Mat& mat, 
  const int colL, const int rowU, const int colR, const int rowL) {
  return { mat.at<Ty>(rowU, colL), mat.at<Ty>(rowU, colR),
           mat.at<Ty>(rowL, colL), mat.at<Ty>(rowL, colR) };
}

/**
 *  \param [in] colL column of upper-left corner
 *  \param [in] row  row of upper-left corner
 *  \param [in] colR column of lower-right corner
 *  \param [in] rowL row of lower-right corner
 *  \param [in] step width of the image
 */
inline Tuple4<size_t> CornersIndex(const int colL, const int rowU,
  const int colR, const int rowL, const int step) {
  const size_t i1 = size_t(step) * rowU + colL;
  const size_t i2 = size_t(step) * rowU + colR;
  const size_t i3 = size_t(step) * rowL + colL;
  const size_t i4 = size_t(step) * rowL + colR;
  return { i1, i2, i3, i4 };
}

/**
 *  \param [in] col column position of pixel
 *  \param [in] row row position of pixel
 *  \param [in] step the height of column-major, or the width of row-major
 *  \param [in] dim number of elements of each pixel
 */
template <int order>
inline Tuple4<size_t> CornersIndex(const int col, const int row,
  const int step, const int dim) {
  size_t i1{};
  if constexpr (eCOLMAHOR == order) {
    i1 = (size_t(step) * col + row) * dim;
  }
  else {
    i1 = (size_t(step) * row + col) * dim;
  }
  const size_t i2 = i1 + dim;
  const size_t i3 = i1 + size_t(step) * dim;
  const size_t i4 = i3 + dim;
  return { i1, i2, i3, i4 };
}

/**
 *  \return Return true if all the four corners are invalid
 */
inline bool MaskAllZero(const uchar* mask, const int64_t i1,
  const int64_t i2, const int64_t i3, const int64_t i4) {
  return (0 == mask[i1] + mask[i2] + mask[i3] + mask[i4]);
}

/**
 *  \return Return true if all the four corners are valid
 */
inline bool MaskAllOne(const uchar* mask, const int64_t i1,
  const int64_t i2, const int64_t i3, const int64_t i4) {
  return (4 == mask[i1] + mask[i2] + mask[i3] + mask[i4]);
}

std::pair<int, int> EXCORE_EXPORTS EstimateTileSize(size_t memSize,
  int bpp, int numCols, int numRows, int kTile=256);

void EXCORE_EXPORTS FillCorners(const RasterWindow& win, 
  std::array<Point2d, kNCP>& corners);

int EXCORE_EXPORTS FillGridCorners(const int num,
  const int stride, std::vector<int>& gridCorners);
int EXCORE_EXPORTS FillGridNodes(const int num,
  const int stride, std::vector<int>& gridNodes);
}
