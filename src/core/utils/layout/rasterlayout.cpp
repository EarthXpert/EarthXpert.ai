/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/rasterlayout.h"

#include <algorithm>
#include <stdexcept>

using earthxpert::RasterLayout;
using earthxpert::RasterWindow;

/**
 *  \param [in] numCols    width of image [unit: pixels]
 *  \param [in] numRows    height of image
 *  \param [in] winNumCols width of window
 *  \param [in] winNumRows height of window
 *  \param [in] kind the primary direction for division
 */
RasterLayout::RasterLayout(const int numCols, const int numRows,
  const int winNumCols, const int winNumRows, const LayoutKind kind)
  : RasterLayout(RasterWindow{ 0, 0, numCols, numRows }, winNumCols, winNumRows, kind) {
}

/**
 *  \param [in] range raster range to divide into smaller windows
 *  \param [in] winNumCols width of window
 *  \param [in] winNumRows height of window
 *  \param [in] kind the primary direction for division
 */
RasterLayout::RasterLayout(const RasterWindow& range,
  const int winNumCols, const int winNumRows, const LayoutKind kind)
  : mRange(range), mWinNumCols(winNumCols), mWinNumRows(winNumRows), mLayoutkind(kind) {
  LayoutWindows();
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
  LayoutWindows();
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
void RasterLayout::LayoutWindows() {
  if (mRange.numCols < mWinNumCols || mRange.numRows < mWinNumRows) {
    throw std::invalid_argument("Thw window size should be less than or equal to the image size");
  }
  if (0 == mWinNumCols) mWinNumCols = mRange.numCols;
  if (0 == mWinNumRows) mWinNumRows = mRange.numRows;

  std::vector<int> colOffsets;
  for (int c = 0; c < mRange.numCols; c += mWinNumCols) {
    colOffsets.push_back(c);
  }
  std::vector<int> rowOffsets;
  for (int r = 0; r < mRange.numRows; r += mWinNumRows) {
    rowOffsets.push_back(r);
  }

  const int numHoriWins = int(colOffsets.size());
  const int numVertWins = int(rowOffsets.size());
  const int numWins = numHoriWins * numVertWins;
  
  mWins.resize(numWins);
  for (int w = 0; w < numWins; ++w) {
    int c{}, r{};
    if (LayoutKind::eHORIZONTAL == mLayoutkind) {
      c = w % numHoriWins;
      r = w / numHoriWins;
    }
    else {
      c = w / numVertWins;
      r = w % numVertWins;
    }
    mWins[w].SetOffsets(colOffsets[c], rowOffsets[r]);

    const int winNumCols = std::min(mWinNumCols, mRange.numCols - colOffsets[c]);
    const int winNumRows = std::min(mWinNumRows, mRange.numRows - rowOffsets[r]);
    mWins[w].SetSize(winNumCols, winNumRows);
  }
}
