/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/layoututils.h"
#include "core/exceptions/exexcept.h"
#include "core/math/math.h"
#include <algorithm>

namespace earthxpert {
/**
 *  \param [in] memSize allowed memory size [unit: bytes]
 *  \param [in] bpp bytes per pixel
 *  \param [in] numCols max number of columns
 *  \param [in] numRows max number of rows
 *  \param [in] kTile minimal tile dimension
 *  \param [in] kind enforce tile shape
 *  \param [in] marginCols number of columns of left & right margins
 *  \param [in] marginRows number of rows of top & bottom margins
 * 
 *  \todo throw if memSize is too small to afford margins
 */
std::pair<int, int> EstimateTileSize(const size_t memSize, 
  const int bpp, const int numCols, const int numRows, 
  const int kTile, const LayoutKind kind,
  const int marginCols, const int marginRows) {
  const size_t marginMemSize = 
    size_t(marginCols) * size_t(marginRows) * size_t(bpp) * 4;
  if (marginMemSize >= memSize) {
    throw std::invalid_argument(_("memSize: increase to be larger than ") + 
      std::to_string(marginMemSize));
  }
  
  const size_t numPix = memSize / bpp;
  int tileNumCols{}, tileNumRows{};

  if (eLK_FULLHEIGHT == kind)
  {
    tileNumCols = int(numPix / (size_t(numRows) + size_t(marginRows) * 2)) - marginCols * 2;
    tileNumCols = std::max(1, (tileNumCols / kTile)) * kTile;
    tileNumRows = numRows;
  }
  else if (eLK_FULLWIDTH == kind)
  {
    tileNumCols = numCols;
    tileNumRows = int(numPix / (size_t(numCols) + size_t(marginCols) * 2)) - marginRows * 2;
    tileNumRows = std::max(1, (tileNumRows / kTile)) * kTile;
  }
  else if (eLK_SQUARE == kind)
  {
    //const int tileDim = int(sqrt(numPix));
    //tileNumCols = std::max(1, ((tileDim - marginCols * 2) / kTile)) * kTile;
    //tileNumRows = std::max(1, ((tileDim - marginRows * 2) / kTile)) * kTile;
    const int marginSub = marginCols - marginRows;
    const int tileDim = int(sqrt(double(numPix) + pow(marginSub, 2))) - 
      (marginCols + marginRows);
    tileNumCols = tileNumRows = std::max(1, (tileDim / kTile)) * kTile;
  }
  else
  {
    assert(false && "Unsupported LayoutKind");
  }
  assert(0 < tileNumCols && 0 < tileNumRows);

  Smaller(tileNumCols, numCols);
  Smaller(tileNumRows, numRows);

  //  \brief balance tile size to be even over the entire scene
  //  \param [in] num total number of pixels
  //  \param [in] tileNum number of tile pixels
  //  \return number of tile pixels to decrease by
  auto balTileNum = [kTile](const int num, const int tileNum) {
    const int numTiles = (num - 1) / tileNum + 1;
    return ((tileNum * numTiles - num) / (kTile * numTiles))* kTile;
  };

  const int balNumCols = balTileNum(numCols, tileNumCols);
  const int balNumRows = balTileNum(numRows, tileNumRows);

  return {tileNumCols - balNumCols, tileNumRows - balNumRows};
}
}
