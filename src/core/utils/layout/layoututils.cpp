/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/layoututils.h"
#include <algorithm>

namespace earthxpert {
/**
 *  \param [in] memSize allowed memory size [unit: bytes]
 *  \param [in] bpp bytes per pixel
 *  \param [in] numCols max number of columns
 *  \param [in] numRows max number of rows
 *  \param [in] kTile minimal tile dimension
 */
std::pair<int, int> EstimateTileSize(const size_t memSize, const int bpp, 
  const int numCols, const int numRows, const int kTile) {
  int tileDim = int(sqrt(memSize / bpp));
  tileDim = std::max(1, (tileDim / kTile)) * kTile;

  int tileNumCols = (tileDim < numCols) ? tileDim : numCols;
  int tileNumRows = (tileDim < numRows) ? tileDim : numRows;

  //  \brief balance tile size
  //  \param [in] num total number of pixels
  //  \param [in] tileNum number of tile pixels
  auto balNumTiles = [kTile](const int num, const int tileNum) {
    const int numTiles = (num - 1) / tileNum + 1;
    return (tileNum * numTiles - num) / (kTile * numTiles);
  };

  const int balNumCols = balNumTiles(numCols, tileNumCols) * kTile;
  const int balNumRows = balNumTiles(numRows, tileNumRows) * kTile;

  return {tileNumCols - balNumCols, tileNumRows - balNumRows};
}
}
