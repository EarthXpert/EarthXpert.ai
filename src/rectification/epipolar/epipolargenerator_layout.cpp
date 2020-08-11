/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "rectification/epipolar/epipolargenerator.h"
#include "core/exceptions/exexcept.h"
#include "core/math/constants.h"
#include "core/math/mutils.h"
#include "core/utils/layoututils.h"
#include "core/utils/rasterlayout.h"

#include <omp.h>
#include <tuple>

using namespace earthxpert;

/**
 *  \brief setup the tiling strategy under the available memory.
 */
void EpipolarGenerator::LayoutTiles() {
  // estimate tile size
  const int bpp = 1 +  mTfmBpp + mEpiBpp * 2 + mImgBpp * mSample * mSample * 2;
  const auto [tileNumCols, tileNumRows] = EstimateTileSize(mMemSize,
    bpp, mEpiNumCols, mEpiNumRows, kTileDim);

  mPerf.numImgTiles = DefineTiles(tileNumCols, tileNumRows);
  
  if (0 == mPerf.numImgTiles)
    throw EpipolarError("LayoutTiles: empty tiles");

  AllocateBuffers();
}

/**
 *  \return Return number of valid tiles.
 *  \todo assign a GeotransformAlgo tag by doing boundary and epiTile intersection 
 */
int EpipolarGenerator::DefineTiles(const int tileNumCols, const int tileNumRows) {
  RasterLayout rasLayout(mEpiNumCols, mEpiNumRows, tileNumCols, tileNumRows);
  if (kMinNumTiles > rasLayout.GetNumWindows()) {
    rasLayout.SetNumWindows(kMinNumTiles);
  }
  mPerf.numTiles = mNumTiles = rasLayout.GetNumWindows();
  mTiles.resize(mNumTiles);

  // calculate image bound for epipolar tiles
  const auto& bound = mEpiModel->GetBoundary();
  int numImgTiles{};

//#pragma omp parallel for reduction(+ : numImgTiles)
  for (int i = 0; i < mNumTiles; ++i) {
    mTiles[i].epiTile = rasLayout.GetRasterWindow(i);
    //const auto epiPoly = utils::ConvertPolygon(mTiles[i].epiTile);
    //Polygon2d poly;
    //bool ret = bg::intersection(epiPoly, bound, poly);
    //if (ret) {
    //}

    mTiles[i].imgTile = DefineImgTile(mTiles[i].epiTile);
    mTiles[i].hasImgTile = mTiles[i].imgTile.IsValid();
    if (mTiles[i].hasImgTile) {
      numImgTiles++;
    }
  }

  return numImgTiles;
}

/**
 *  \brief obtain image tile information given an epipolar tile. 
 */
RasterWindow EpipolarGenerator::DefineImgTile(const RasterWindow& epiTile) {
  std::array<Point2d, kNCP> epiCorners;
  FillCorners(epiTile, epiCorners);

  Point2d minImgXY(mImgNumCols, mImgNumRows);
  Point2d maxImgXY(-1, -1);

  Point2d imgCorner;
  for (int i = 0; i < kNCP; ++i) {
    mEpiModel->EpiToRaw(epiCorners[i], imgCorner);
    imgCorner = imgCorner - Point2d(0.5, 0.5);

    MinMax(minImgXY, maxImgXY, imgCorner);
  }

  // add margin and clamp to image extent
  const Point2i minImgCR(std::clamp(int(floor(minImgXY.x)) - mMarginDim, 0, mImgNumCols - 1),
    std::clamp(int(floor(minImgXY.y)) - mMarginDim, 0, mImgNumRows - 1));
  const Point2i maxImgCR(std::clamp(int(ceil(maxImgXY.x)) + mMarginDim, 0, mImgNumCols - 1),
    std::clamp(int(ceil(maxImgXY.y)) + mMarginDim, 0, mImgNumRows - 1));
  const cv::Size2i imgSize(maxImgCR - minImgCR + Point2i(1, 1));

  RasterWindow imgTile;
  if (1 < imgSize.width && 1 < imgSize.height) {
    imgTile.SetWindow(minImgCR, imgSize);
  }
  return imgTile;
}

/**
 *  \return Returns the max number of pixels to hold any image tile
 *
 *  \param [in] tiles the tiling layout
 */
std::pair<int64_t, int64_t> 
EpipolarGenerator::GetTilesLimit(const std::vector<TileInfo>& tiles) const {
  int64_t maxImgTileNumPix{ -1 };
  int64_t maxEpiTileNumPix{ -1 };

  for (auto& tile : tiles) {
    if (tile.imgTile.IsValid()) {
      Larger(maxImgTileNumPix, tile.imgTile.GetNumPixels());
    }

    if (tile.epiTile.IsValid()) {
      Larger(maxEpiTileNumPix, tile.epiTile.GetNumPixels());
    }
  }

  return { maxImgTileNumPix, maxEpiTileNumPix };
}
