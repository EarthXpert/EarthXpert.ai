/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "rectification/epipolar/epipolargenerator.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/irasterresample.h"
#include "core/utils/rasterlayout.h"
#include "epipolarmodel/iepipolarmodel.h"
#include "io/raster/irasterreader.h"
#include "io/raster/rasterwriter.h"

#include <chrono>
#include <omp.h>
#include <tuple>

using namespace earthxpert;
namespace chrono = std::chrono;

/**
 *  \brief
 * 
 *  \param memSize [in] memory available [unit: bytes]
 */
EpipolarGenerator::EpipolarGenerator(const IRasterResample* rasResample, 
  const size_t memSize, const unsigned short stride)
  : mRasResample(rasResample), mMemSize(memSize), mStride(stride) {
  mMarginDim = mRasResample->GetWinDim() / 2;
}

/**
 *  \brief
 *
 *  \return Returns true if operation succeeds
 */
bool EpipolarGenerator::Process(IRasterReader *const img, 
  IRasterWriter *const epi, IEpipolarModel const* epiModel) {
  mPerf.Clear();
  auto start = chrono::steady_clock::now();

  try {
    SetImageParams(img);
    SetEpipolarParams(epi, epiModel);

    LayoutTiles();
    ProcessTiles();
  }
  catch (...) {
    throw;
  }

  chrono::duration<double> diff = chrono::steady_clock::now() - start;
  mPerf.tmTotal = diff.count();
  return true;
}

/**
 *  \brief
 */
void EpipolarGenerator::SetImageParams(IRasterReader* const img) {
  assert(img);
  mImgReader = dynamic_cast<RasterReader* const>(img);
  std::tie(mImgNumCols, mImgNumRows) = mImgReader->GetSize();
  mImgCvDT = mImgReader->GetCvDataType();
  mImgBpp = CV_ELEM_SIZE(mImgCvDT);
}

/**
 *  \brief
 */
void EpipolarGenerator::SetEpipolarParams(
  IRasterWriter* const epi, IEpipolarModel const* epiModel) {
  assert(epi);
  mEpiWriter = dynamic_cast<RasterWriter* const>(epi);
  std::tie(mEpiNumCols, mEpiNumRows) = mEpiWriter->GetSize();
  mEpiCvDT = mEpiWriter->GetCvDataType();
  mEpiBpp = CV_ELEM_SIZE(mEpiCvDT);

  assert(epiModel);
  mEpiModel = epiModel;

  const Point2d ic{ mImgNumCols * 0.5, mImgNumRows * 0.5 };
  Point2d ec, ic2;
  mEpiModel->RawToEpi(ic, ec);
  mEpiModel->EpiToRaw(ec + Point2d(1.0, 1.0), ic2);

  const double sample = cv::norm(ic2 - ic) * M_SQRT1_2;
  mSample = std::max<int>(1, std::lround(sample));
}

/**
 *  \brief
 */
void EpipolarGenerator::LayoutTiles() {
  static constexpr int kNP = 8;
  auto start = chrono::steady_clock::now();

  // estimate tile size
  const double bpp = 1.0 +  mTfmBpp + mEpiBpp + mImgBpp * mSample * mSample;
  int tileDim = int(sqrt(mMemSize / bpp));
  tileDim = std::max(1, (tileDim / kTileDim)) * kTileDim;
  
  int tileNumCols = (tileDim < mEpiNumCols) ? tileDim : mEpiNumCols;
  int tileNumRows = (tileDim < mEpiNumRows) ? tileDim : mEpiNumRows;

  //  \param [in] num total number of pixels
  //  \param [in] tileNum number of tile pixels
  auto balNumTiles = [](const int num, const int tileNum) {
    const int numTiles = (num - 1) / tileNum + 1;
    return (tileNum * numTiles - num) / (kTileDim * numTiles);
  };
    
  const int balNumCols = balNumTiles(mEpiNumCols, tileNumCols) * kTileDim;
  const int balNumRows = balNumTiles(mEpiNumRows, tileNumRows) * kTileDim;

  tileNumCols -= balNumCols;
  tileNumRows -= balNumRows;
  RasterLayout rasLayout(mEpiNumCols, mEpiNumRows, tileNumCols, tileNumRows);
  
  mEpiTiles = rasLayout.GetRasterWindows();
  mNumTiles = int(mEpiTiles.size());

  // calculate image bound for epipolar tiles
  int numImgTiles{};
  mImgTiles.clear();

  std::array<Point2d, kNP> epiCorners;
  std::array<Point2d, kNP> imgCorners;

  for (const auto [colOffset, rowOffset, numCols, numRows] : mEpiTiles) {
    epiCorners[0] = Point2d(colOffset, rowOffset);
    epiCorners[1] = Point2d(colOffset + numCols / 2, rowOffset);
    epiCorners[2] = Point2d(colOffset + numCols, rowOffset);
    epiCorners[3] = Point2d(colOffset + numCols, rowOffset + numRows / 2);
    epiCorners[4] = Point2d(colOffset + numCols, rowOffset + numRows);
    epiCorners[5] = Point2d(colOffset + numCols / 2, rowOffset + numRows);
    epiCorners[6] = Point2d(colOffset, rowOffset + numRows);
    epiCorners[7] = Point2d(colOffset, rowOffset + numRows / 2);

    double minImgX = mImgNumCols;
    double minImgY = mImgNumRows;
    double maxImgX{};
    double maxImgY{};

    for (int i = 0; i < kNP; ++i) {
      mEpiModel->EpiToRaw(epiCorners[i], imgCorners[i]);
      imgCorners[i] = imgCorners[i] - Point2d(0.5, 0.5);

      if (minImgX > imgCorners[i].x) minImgX = imgCorners[i].x;
      if (minImgY > imgCorners[i].y) minImgY = imgCorners[i].y;
      if (maxImgX < imgCorners[i].x) maxImgX = imgCorners[i].x;
      if (maxImgY < imgCorners[i].y) maxImgY = imgCorners[i].y;
    }

    int minImgCol = int(floor(minImgX)) - mMarginDim;
    int minImgRow = int(floor(minImgY)) - mMarginDim;
    int maxImgCol = int(ceil(maxImgX)) + mMarginDim;
    int maxImgRow = int(ceil(maxImgY)) + mMarginDim;

    minImgCol = std::clamp(minImgCol, 0, mImgNumCols - 1);
    minImgRow = std::clamp(minImgRow, 0, mImgNumRows - 1);
    maxImgCol = std::clamp(maxImgCol, 0, mImgNumCols - 1);
    maxImgRow = std::clamp(maxImgRow, 0, mImgNumRows - 1);

    const int imgNumCols = maxImgCol - minImgCol + 1;
    const int imgNumRows = maxImgRow - minImgRow + 1;

    RasterWindow imgTile;
    if (1 < imgNumCols && 1 < imgNumRows) {
      imgTile.colOffset = minImgCol;
      imgTile.rowOffset = minImgRow;
      imgTile.numCols = imgNumCols;
      imgTile.numRows = imgNumRows;

      numImgTiles++;
    }
    mImgTiles.push_back(imgTile);
  }

  if (0 == numImgTiles) {
    throw EpipolarError("LayoutTiles: empty image tiles");
  }

  AllocateBuffers();

  chrono::duration<double> diff = chrono::steady_clock::now() - start;
  mPerf.tmLayout += diff.count();
}

/**
 *  \return Returns the max number of pixels to hold any tile
 */
int64_t EpipolarGenerator::GetTilesLimit(const std::vector<RasterWindow>& tiles) const {
  int maxTileNumPix{ -1 };

  for (auto& tile : tiles) {
    if (!tile.IsValid()) continue;

    const auto tileNumPix = tile.GetNumPixels();
    if (maxTileNumPix < tileNumPix)
      maxTileNumPix = tileNumPix;
  }

  return maxTileNumPix;
}

/**
 *  \brief allocate memory buffers
 */
void EpipolarGenerator::AllocateBuffers() {
  const auto imgBufNumPix = GetTilesLimit(mImgTiles);
  const int64_t imgBufNumBytes = mImgBpp * imgBufNumPix;
  if (mImgBufNumBytes < imgBufNumBytes) {
    mImgBuffer.reset();
    mImgBufNumBytes = imgBufNumBytes;
    mImgBuffer.reset(new uchar[mImgBufNumBytes]);
  }

  const auto epiBufNumPix = GetTilesLimit(mEpiTiles);
  const int64_t epiBufNumBytes = mEpiBpp * epiBufNumPix;
  if (mEpiBufNumBytes < epiBufNumBytes) {
    mEpiBuffer.reset();
    mMaskBuffer.reset();
    mTfmBuffer.reset();
    
    mEpiBufNumBytes = epiBufNumBytes;
    mEpiBuffer.reset(new uchar[mEpiBufNumBytes]);
    mMaskBuffer.reset(new uchar[epiBufNumPix]);
    mTfmBuffer.reset(new uchar[mTfmBpp * epiBufNumPix]);
  }
}

/**
 *  \brief
 */
void EpipolarGenerator::ResetBuffers(const int tileId) {
  auto start = chrono::steady_clock::now();

  const auto& epiTile = mEpiTiles[tileId];
  const int64_t epiNumPix = epiTile.GetNumPixels();
  memset(mEpiBuffer.get(), 0, mEpiBpp * epiNumPix);

  const auto& imgTile = mImgTiles[tileId];
  if (imgTile.IsValid()) {
    memset(mImgBuffer.get(), 0, mImgBpp * imgTile.GetNumPixels());
    memset(mMaskBuffer.get(), 0, epiNumPix);
    cv::Mat tfmMat(epiTile.numRows, epiTile.numCols, mTfmCvDT, mTfmBuffer.get());

    tfmMat.forEach<cv::Vec2f>([](cv::Vec2f& tfm, const int*) { 
      tfm[0] = -1; tfm[1] = -1; });
    
//#pragma omp parallel for
    //for (int64_t k = 0; k < epiNumPix; ++k) {
    //  const int col = k % epiTile.numCols;
    //  const int row = k / epiTile.numCols;
    //  auto& tfm = tfmMat.at<cv::Vec2f>(row, col);
    //  tfm[0] = -1;
    //  tfm[1] = -1;
    //}
  }

  chrono::duration<double> diff = chrono::steady_clock::now() - start;
  mPerf.tmReset += diff.count();
}

/**
 *  \brief
 */
void EpipolarGenerator::FreeBuffers() {
  mImgBuffer.reset();
  mEpiBuffer.reset();
  mTfmBuffer.reset();
  mMaskBuffer.reset();
}

/**
 *  \brief 
 */
void EpipolarGenerator::ProcessTiles() {
  for (int t = 0; t < mNumTiles; ++t) {
    ResetBuffers(t);

    ReadTile(t);
    ProcessTile(t);
    WriteTile(t);
  }
}

/**
 *  \brief
 */
void EpipolarGenerator::ReadTile(const int tileId) {
  const auto [colOffset, rowOffset, numCols, numRows] = mImgTiles[tileId];
  if (0 == numCols || 0 == numRows) return;
  auto start = chrono::steady_clock::now();

  CvRaster imgRaster(numCols, numRows, mImgCvDT, colOffset, rowOffset, mImgBuffer.get());
  mImgReader->ReadRaster(&imgRaster);

  chrono::duration<double> diff = chrono::steady_clock::now() - start;
  mPerf.tmRead += diff.count();
}

/**
 *  \brief
 */
void EpipolarGenerator::ProcessTile(const int tileId) {
  const auto [imgColOffset, imgRowOffset, imgNumCols, imgNumRows] = mImgTiles[tileId];
  if (0 == imgNumCols || 0 == imgNumRows) return;
  auto start = chrono::steady_clock::now();

  cv::Mat imgMat(imgNumRows, imgNumCols, mImgCvDT, mImgBuffer.get());

  const auto [colOffset, rowOffset, numCols, numRows] = mEpiTiles[tileId];
  cv::Mat epiMat(numRows, numCols, mEpiCvDT, mEpiBuffer.get());

  cv::Mat tfmMat(numRows, numCols, mTfmCvDT, mTfmBuffer.get());
  EpiToImg(tileId, tfmMat);

  mRasResample->Resample(imgMat, epiMat, tfmMat);

  chrono::duration<double> diff = chrono::steady_clock::now() - start;
  mPerf.tmProcess += diff.count();
}

/**
 *  \brief
 */
void EpipolarGenerator::WriteTile(const int tileId) {
  auto start = chrono::steady_clock::now();

  const auto [colOffset, rowOffset, numCols, numRows] = mEpiTiles[tileId];
  CvRaster epiRaster(numCols, numRows, mEpiCvDT, colOffset, rowOffset, mEpiBuffer.get());
  mEpiWriter->WriteRaster(&epiRaster);

  chrono::duration<double> diff = chrono::steady_clock::now() - start;
  mPerf.tmWrite += diff.count();
}

/**
 *  \brief Do geometric transformation to populate the map
 */
void EpipolarGenerator::EpiToImg(const int tileId, cv::Mat& tfmMat) {
  const auto [imgColOffset, imgRowOffset, imgNumCols, imgNumRows] = mImgTiles[tileId];
  const auto [colOffset, rowOffset, numCols, numRows] = mEpiTiles[tileId];

  // grid col/row positions for rigorous geometry
  std::vector<int> gridCols;
  std::vector<int> gridRows;

  for (int i = 0; i < numCols - 1; i += mStride) {
    gridCols.push_back(i);
  }
  gridCols.push_back(numCols - 1);

  for (int i = 0; i < numRows - 1; i += mStride) {
    gridRows.push_back(i);
  }
  gridRows.push_back(numRows - 1);

  const int numGridCols = int(gridCols.size());
  const int numGridRows = int(gridRows.size());
  assert(2 <= numGridCols);
  assert(2 <= numGridRows);

  const int64_t numGridPix = numGridCols * numGridRows;

  // Do geometry at grid points
#pragma omp parallel for
  for (int64_t g = 0; g < numGridPix; ++g) {
    const int gridCol = int(g % numGridCols);
    const int gridRow = int(g / numGridCols);

    const int tileCol = gridCols[gridCol];
    const int tileRow = gridRows[gridRow];
    EpiToImg(tileId, tfmMat, tileCol, tileRow);
  }

  if (1 == mSample) return;
  
  // Densify non-grid positions
  const int64_t numPix = numCols * numRows;

#pragma omp parallel for
  for (int64_t k = 0; k < numPix; ++k) {
    if (1 == mMaskBuffer[k]) continue;
    const int tileCol = int(k % numCols);
    const int tileRow = int(k / numCols);

    const bool isGridCol = (0 == tileCol % mSample || tileCol == numCols - 1);
    const bool isGridRow = (0 == tileRow % mSample || tileRow == numRows - 1);
    if (isGridCol && isGridRow) continue; // failed grid point

    const int gridCol = std::min(tileCol / mSample, numGridCols - 2);
    const int gridRow = std::min(tileRow / mSample, numGridRows - 2);

    const int tileColL = gridCols[gridCol];
    const int tileColR = gridCols[gridCol + 1];
    const int tileRowU = gridRows[gridRow];
    const int tileRowL = gridRows[gridRow + 1];

    const int64 i1 = tileRowU * numCols + tileColL;
    const int64 i2 = tileRowU * numCols + tileColR;
    const int64 i3 = tileRowL * numCols + tileColL;
    const int64 i4 = tileRowL * numCols + tileColR;

    if (0 == mMaskBuffer[i1] && 0 == mMaskBuffer[i2] &&
        0 == mMaskBuffer[i3] && 0 == mMaskBuffer[i4]) {
      continue;
    }

    auto& tfm = tfmMat.at<cv::Vec2f>(tileRow, tileCol);

    if (1 == mMaskBuffer[i1] && 1 == mMaskBuffer[i2] &&
        1 == mMaskBuffer[i3] && 1 == mMaskBuffer[i4]) {
      // Bilinear interpolation if all four grid points are valid
      const double xf = double(tileCol - tileColL) / double(tileColR - tileColL);
      const double yf = double(tileRow - tileRowU) / double(tileRowL - tileRowU);

      tfm[0] = float((1.0 - yf) * ((1.0 - xf) * gridCols[i1] + xf * gridCols[i2]) +
        yf * ((1.0 - xf) * gridCols[i3] + xf * gridCols[i4]));
      tfm[1] = float((1.0 - yf) * ((1.0 - xf) * gridRows[i1] + xf * gridRows[i2]) +
        yf * ((1.0 - xf) * gridRows[i3] + xf * gridRows[i4]));
      mMaskBuffer[k] = 1;
    }
    else {
      // Do actual geometry if any grid point is invalid
      EpiToImg(tileId, tfmMat, tileCol, tileRow);
    }
  }
}

/**
 *  \brief Do rigorous transformation at given epipolar pixel
 */
void EpipolarGenerator::EpiToImg(const int tileId, cv::Mat& tfmMat,
  const int tileCol, const int tileRow) {
  const auto [imgColOffset, imgRowOffset, imgNumCols, imgNumRows] = mImgTiles[tileId];
  const auto [colOffset, rowOffset, numCols, numRows] = mEpiTiles[tileId];

  Point2d epi(0.5 + colOffset + tileCol, 0.5 + rowOffset + tileRow), img;
  const auto ret = mEpiModel->EpiToRaw(epi, img);
  if (!ret) return;
  
  const float tfmCol = float(img.x - 0.5 - imgColOffset);
  const float tfmRow = float(img.y - 0.5 - imgRowOffset);

  if (0 <= tfmCol && tfmCol < imgNumCols - 1 &&
    0 <= tfmRow && tfmRow < imgNumRows - 1) {
    auto& tfm = tfmMat.at<cv::Vec2f>(tileRow, tileCol);
    tfm[0] = tfmCol;
    tfm[1] = tfmRow;

    const int64_t k = int64_t(numCols) * int64_t(tileRow) + int64_t(tileCol);
    mMaskBuffer[k] = 1;
  }
}
