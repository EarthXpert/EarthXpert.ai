/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"

namespace earthxpert {
/**
 *  The structure RasterWindow contains the offset and size information.
 */
struct EXCORE_EXPORTS RasterWindow {
  int colOffset{};
  int rowOffset{};
  int numCols{};
  int numRows{};

  void SetOffsets(const int colOffset, const int rowOffset) {
    this->colOffset = colOffset;
    this->rowOffset = rowOffset;
  }

  void SetSize(const int numCols, const int numRows) {
    this->numCols = numCols;
    this->numRows = numRows;
  }

  void SetMargins(const int marginCols, const int marginRows) {
    colOffset -= marginCols;
    rowOffset -= marginRows;
    numCols += marginCols * 2;
    numRows += marginRows * 2;
  }

  bool IsValid() const noexcept {
    return (0 < numCols && 0 < numRows);
  }

  int64 GetNumPixels() const noexcept { return int64(numCols) * int64(numRows); }

  /**
   *  \param [in] rhs Object to test
   *  \return Return true if containing the other object improperly, false otherwise.
   */
  bool ImproperContain(const RasterWindow& rhs) const noexcept {
    if (colOffset > rhs.colOffset) return false;
    if (rowOffset > rhs.rowOffset) return false;
    if (colOffset + numCols < rhs.colOffset + rhs.numCols) return false;
    if (rowOffset + numRows < rhs.rowOffset + rhs.numRows) return false;
    return true;
  }
};
}