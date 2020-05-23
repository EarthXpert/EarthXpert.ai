/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/wxcore.h"

namespace earthxpert {
/**
 *  The structure RasterWindow contains the offset and size information.
 */
struct WXCORE_EXPORTS RasterWindow {
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
};
}