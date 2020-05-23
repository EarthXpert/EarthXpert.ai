/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/wxcore.h"
#include "core/utils/rasterwindow.h"

#include <cassert>
#include <vector>

namespace earthxpert {
/**
 *  The class RasterLayout formulate the non-overlapping tiling of image.
 */
class WXCORE_EXPORTS RasterLayout {
public:
  /**
   *  \brief enum LayoutKind specifies the major direction of tile division.
   */
  enum class LayoutKind {
    eHORIZONTAL,
    eVERTICAL
  };

  RasterLayout(int numCols, int numRows, 
    int winNumCols, int winNumRows, LayoutKind kind=LayoutKind::eHORIZONTAL);
  ~RasterLayout() {}

  int SetWindowSize(int winNumCols, int winNumRows);
  int SetNumWindows(int numWins);

  int GetNumWindows() const {
    return static_cast<int>(mWins.size());
  }

  /**
   *  \param [in] index 0-based position
   *  \return Returns const reference to the RasterWindow object at given position.
   */
  const auto& GetRasterWindow(const int index) {
    assert(0 <= index && index < mWins.size());
    return mWins[index];
  }

  /**
   *  \return Returns const reference to the array of RasterWindow objects.
   */
  const auto& GetRasterWindows() const {
    return mWins;
  }

private:
  int mNumCols{};     // width of image [unit: pixels]
  int mNumRows{};     // height of image
  int mWinNumCols{};  // width of window
  int mWinNumRows{};  // height of window
  LayoutKind mLayoutkind{LayoutKind::eHORIZONTAL};  // major direction
  std::vector<RasterWindow> mWins;  // array of raster windows formulating the layout

  void GenerateLayout();
};
}