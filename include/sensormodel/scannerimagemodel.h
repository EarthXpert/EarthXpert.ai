/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/wxsensormodel.h"

#include <utility>

namespace watxtract {
/**
 *  The class defines image product vs focal plane relationship for pushbroom
 * line scanner.
 *
 *  \detail
 */
class WXSENSORMODEL_EXPORTS ScannerImageModel {
public:
  /**
   * Constructor
   */
  ScannerImageModel();

  ~ScannerImageModel() {}

  /**
   *  \return Return raster size of image (numCols, numRows) [unit: pixel]
   */
  std::pair<int, int> GetImageSize() const noexcept {
    return { mNumCols, mNumRows };
  }

private:
  int mNumCols{}; // number of columns of the image product
  int mNumRows{}; // number of rows of the image product
};
} // namespace watxtract
