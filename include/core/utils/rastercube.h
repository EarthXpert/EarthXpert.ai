/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/opencv/exopencv.h"

#include <stdint.h>

namespace earthxpert {
/**
 *  The structure RasterCube contains the offset and size information
 *  of a discrete 3D range such as raster depth tensor.
 *  \todo 
 */
struct EXCORE_EXPORTS RasterCube {
  int colOffset{};
  int rowOffset{};
  //?
  int numCols{};
  int numRows{};
  //?

  /**
   *  Equality operator
   *
   *  \param [in] rhs Object to compare with
   *  \return true if same object or equal, false otherwise.
   */
  bool operator==(const RasterCube& rhs) const {
    if (colOffset != rhs.colOffset || rowOffset != rhs.rowOffset) return false;
    if (numCols != rhs.numCols || numRows != rhs.numRows) return false;
    return true;
  }

  /**
   *  Test inequality.
   *
   *  \param [in] rhs Object to compare with
   *  \return true if not equal.
   */
  bool operator!=(const RasterCube& rhs) const {
    return !(operator==(rhs));
  }

  inline void SetOffsets(const Point2i offset) {
    SetOffsets(offset.x, offset.y);
  }
  inline void SetOffsets(const int colOffset, const int rowOffset) {
    this->colOffset = colOffset;
    this->rowOffset = rowOffset;
  }

  inline void SetSize(const cv::Size2i size) {
    SetSize(size.width, size.height);
  }
  inline void SetSize(const int numCols, const int numRows) {
    this->numCols = numCols;
    this->numRows = numRows;
  }

  inline void SetWindow(const Point2i offset, cv::Size2i size) {
    SetOffsets(offset);
    SetSize(size);
  }
  inline void SetWindow(const int colOffset, const int rowOffset,
    const int numCols, const int numRows) {
    SetOffsets(colOffset, rowOffset);
    SetSize(numCols, numRows);
  }

  inline void SetMargins(const int marginCols, const int marginRows) {
    colOffset -= marginCols;
    rowOffset -= marginRows;
    numCols += marginCols * 2;
    numRows += marginRows * 2;
  }

  inline bool IsValid() const noexcept { return (0 < numCols && 0 < numRows); }

  inline int64_t GetNumPixels() const noexcept { return int64_t(numCols) * numRows; }

  std::pair<int, int> GetOffset() const noexcept { return { colOffset, rowOffset }; }
  std::pair<int, int> GetSize() const noexcept { return { numCols, numRows }; }

  /**
   *  \param [in] rhs Object to test
   *  \return Return true if containing the other object improperly, false otherwise.
   */
  bool ImproperContain(const RasterCube& rhs) const noexcept {
    if (colOffset > rhs.colOffset) return false;
    if (rowOffset > rhs.rowOffset) return false;
    if (colOffset + numCols < rhs.colOffset + rhs.numCols) return false;
    if (rowOffset + numRows < rhs.rowOffset + rhs.numRows) return false;
    return true;
  }
};
}
