/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/rasterlayout.h"

#include <algorithm>
#include <stdexcept>

using namespace earthxpert;

/**
 *  \param [in] numCols    width of image [unit: pixels]
 *  \param [in] numRows    height of image
 *  \param [in] winNumCols width of window
 *  \param [in] winNumRows height of window
 */
RasterLayout::RasterLayout(const int numCols, const int numRows, 
  const int winNumCols, const int winNumRows, const LayoutKind kind) 
  : mNumCols(numCols), mNumRows(numRows), mWinNumCols(winNumCols),
  mWinNumRows(winNumRows), mLayoutkind(kind) {
  GenerateLayout();
}

/**
 *  \brief Divides image extent into tiles using specified window size.
 *
 *  \param [in] winNumCols width of window
 *  \param [in] winNumRows height of window
 *  \return Returns number of tiles.
 */
int RasterLayout::SetWindowSize(const int winNumCols, const int winNumRows) {
  mWinNumCols = winNumCols;
  mWinNumRows = winNumRows;
  GenerateLayout();
  return static_cast<int>(mWins.size());
}

/**
 *  \brief Divides image extent into tiles according to the expected number of windows.
 *  The window's width and height are balanced to be close to square.
 *
 *  \param [in] numWins expected number of windows
 *  \return Returns actual number of tiles.
 */
int RasterLayout::SetNumWindows(const int numWins) {
  // \todo
  return static_cast<int>(mWins.size());
}

/**
 *  \brief Divides image extent into tiles.
 */
void RasterLayout::GenerateLayout() {
  if (mNumCols < mWinNumCols || mNumRows < mWinNumRows) {
    throw std::invalid_argument("Thw window size should be less than or equal to the image size");
  }
  if (0 == mWinNumCols) mWinNumCols = mNumCols;
  if (0 == mWinNumRows) mWinNumRows = mNumRows;

  if (LayoutKind::eHORIZONTAL == mLayoutkind) {
    for (int r = 0; r < mNumRows; r += mWinNumRows)
    {
      int winNumRows = std::min(mWinNumRows, mNumRows - r);

      for (int c = 0; c < mNumCols; c += mWinNumCols)
      {
        int winNumCols = std::min(mWinNumCols, mNumCols - c);

        mWins.push_back(RasterWindow{ c, r, winNumCols, winNumRows });
      }
    }
  }
  else {
    for (int c = 0; c < mNumCols; c += mWinNumCols)
    {
      int winNumCols = std::min(mWinNumCols, mNumCols - c);

      for (int r = 0; r < mNumRows; r += mWinNumRows)
      {
        int winNumRows = std::min(mWinNumRows, mNumRows - r);

        mWins.push_back(RasterWindow{ c, r, winNumCols, winNumRows });
      }
    }
  }
}
