/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "core/coretypes.h"
#include "core/utils/rasterwindow.h"

#include <cassert>
#include <vector>

namespace earthxpert {
enum class LayoutKind;
/**
 *  The class RasterLayout formulate the non-overlapping tiling of image
 *  in raster coodinate system.
 *  \remark a specialization of template class SpatialLayout.
 */
class EXCORE_EXPORTS RasterLayout {
public:
  RasterLayout(int numCols, int numRows, int winNumCols, int winNumRows, 
    LayoutKind kind = LayoutKind::eLK_HORIZONTAL);
  RasterLayout(const RasterWindow& range, int winNumCols, int winNumRows, 
    LayoutKind kind = LayoutKind::eLK_HORIZONTAL);
  RasterLayout(const RasterLayout& rhs) = default;
  RasterLayout(RasterLayout&& rhs) noexcept = default;
  ~RasterLayout() = default;

  RasterLayout& operator=(const RasterLayout& rhs) = default;
  RasterLayout& operator=(RasterLayout&& rhs) noexcept = default;

  int SetWindowSize(int winNumCols, int winNumRows);
  int SetNumWindows(int numWins);

  int GetNumWindows() const { return static_cast<int>(mWins.size()); }

  /**
   *  \param [in] index 0-based position
   *  \return Returns const reference to the RasterWindow object at given position.
   */
  const auto& GetRasterWindow(const int index) const {
    assert(0 <= index && index < mWins.size());
    return mWins[index];
  }

  /**
   *  \return Returns const reference to the array of RasterWindow objects.
   */
  const auto& GetRasterWindows() const { return mWins; }

private:
  RasterWindow mRange;    // raster range to divide into smaller windows
  int mWinNumCols{};  // width of window
  int mWinNumRows{};  // height of window
  LayoutKind mLayoutkind{LayoutKind::eLK_HORIZONTAL};  // major direction
  std::vector<RasterWindow> mWins;  // array of raster windows formulating the layout

  void LayoutWindows();
};
}