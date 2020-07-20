/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "rectification/epipolar/epipolargenerator.h"
#include "core/exceptions/exexcept.h"
#include "core/math/utils.h"
#include "core/utils/irasterresample.h"
#include "core/utils/layoututils.h"
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
  const size_t memSize, const int stride)
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
  omp_set_nested(2);

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
  const auto start = chrono::steady_clock::now();

  // estimate tile size
  const int bpp = 1 +  mTfmBpp + mEpiBpp * 2 + mImgBpp * mSample * mSample * 2;
  const auto [tileNumCols, tileNumRows] = EstimateTileSize(mMemSize,
    bpp, mEpiNumCols, mEpiNumRows, kTileDim);
  RasterLayout rasLayout(mEpiNumCols, mEpiNumRows, tileNumCols, tileNumRows);
  
  mEpiTiles = rasLayout.GetRasterWindows();
  mNumTiles = int(mEpiTiles.size());

  // calculate image bound for epipolar tiles
  int numImgTiles{};
  mImgTiles.clear();

  std::array<Point2d, kNCP> epiCorners;

  for (const auto& epiTile : mEpiTiles) {
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
      numImgTiles++;
    }
    mImgTiles.push_back(imgTile);
  }

  if (0 == numImgTiles) {
    throw EpipolarError("LayoutTiles: empty image tiles");
  }

  AllocateBuffers();

  const chrono::duration<double> diff = chrono::steady_clock::now() - start;
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
 *  \todo reduce mMaskBuf to label grid nodes
 */
void EpipolarGenerator::AllocateBuffers() {
  const auto imgBufNumPix = GetTilesLimit(mImgTiles);
  const int64_t imgBufNumBytes = mImgBpp * imgBufNumPix;
  if (mImgBufNumBytes < imgBufNumBytes) {
    mImgBufRead.reset();
    mImgBufWork.reset();
    mImgBufNumBytes = imgBufNumBytes;
    mImgBufRead.reset(new uchar[mImgBufNumBytes]);
    mImgBufWork.reset(new uchar[mImgBufNumBytes]);
  }

  const auto epiBufNumPix = GetTilesLimit(mEpiTiles);
  const int64_t epiBufNumBytes = mEpiBpp * epiBufNumPix;
  if (mEpiBufNumBytes < epiBufNumBytes) {
    mEpiBufWork.reset();
    mEpiBufWrit.reset();
    mMaskBuf.reset();
    mTfmBuf.reset();
    
    mEpiBufNumBytes = epiBufNumBytes;
    mEpiBufWork.reset(new uchar[mEpiBufNumBytes]);
    mEpiBufWrit.reset(new uchar[mEpiBufNumBytes]);
    mMaskBuf.reset(new uchar[epiBufNumPix]);
    mTfmBuf.reset(new uchar[mTfmBpp * epiBufNumPix]);
  }
}

/**
 *  \brief
 */
void EpipolarGenerator::ResetBuffers(const int tileId) {
  if (!TileId(tileId)) return;
  const auto start = chrono::steady_clock::now();

  memset(mEpiBufWork.get(), 0, mEpiBpp * mEpiTiles[tileId].GetNumPixels());

  const chrono::duration<double> diff = chrono::steady_clock::now() - start;
  mPerf.tmReset += diff.count();
}

/**
 *  \brief
 */
void EpipolarGenerator::SwapBuffers() {
  std::swap(mImgBufRead, mImgBufWork);
  std::swap(mEpiBufWork, mEpiBufWrit);
}

/**
 *  \brief
 */
void EpipolarGenerator::FreeBuffers() {
  mImgBufRead.reset();
  mImgBufWork.reset();
  mEpiBufWork.reset();
  mEpiBufWrit.reset();
  mTfmBuf.reset();
  mMaskBuf.reset();
}

/**
 *  \brief pipelined overlapping of read/write and computation tasks
 *  \todo investigate GDAL thread model: does it allow simuteneous read/write of different datasets? 
 */
void EpipolarGenerator::ProcessTiles() {
  for (int t = -1; t <= mNumTiles; ++t) {
    ResetBuffers(t);

#pragma omp parallel sections num_threads(2)
    {
#pragma omp section
      {
        ReadTile(t + 1);
        WriteTile(t - 1);
      }

#pragma omp section
      ProcessTile(t);
    }

    SwapBuffers();
  }
}

/**
 *  \brief pipelined overlapping of read, computation, write tasks
 *  \todo 4x slower than SQ!
 */
void EpipolarGenerator::ProcessTilesPL() {
  for (int t = -1; t <= mNumTiles; ++t) {
    ResetBuffers(t);

#pragma omp parallel sections num_threads(3)
    {
#pragma omp section
      ReadTile(t + 1);

#pragma omp section
      ProcessTile(t);

#pragma omp section
      WriteTile(t - 1);
    }

    SwapBuffers();
  }
}

/**
 *  \brief sequential non-overlapping of read, computation, write tasks
 */
void EpipolarGenerator::ProcessTilesSQ() {
  for (int t = -1; t <= mNumTiles; ++t) {
    ResetBuffers(t);

    ReadTile(t + 1);
    ProcessTile(t);
    WriteTile(t - 1);

    SwapBuffers();
  }
}

/**
 *  \brief
 */
void EpipolarGenerator::ReadTile(const int tileId) {
  if (!TileId(tileId)) return;
  const auto [colOffset, rowOffset, numCols, numRows] = mImgTiles[tileId];
  if (0 == numCols || 0 == numRows) return;
  const auto start = chrono::steady_clock::now();
  //memset(mImgBufRead.get(), 0, mImgBpp * numCols * numRows);

  CvRaster imgRaster(numCols, numRows, mImgCvDT, colOffset, rowOffset, mImgBufRead.get());
  mImgReader->ReadRaster(&imgRaster);

  const chrono::duration<double> diff = chrono::steady_clock::now() - start;
  mPerf.tmRead += diff.count();
}

/**
 *  \brief
 */
void EpipolarGenerator::ProcessTile(const int tileId) {
  if (!TileId(tileId)) return;
  const auto [imgColOffset, imgRowOffset, imgNumCols, imgNumRows] = mImgTiles[tileId];
  if (0 == imgNumCols || 0 == imgNumRows) return;
  const auto [colOffset, rowOffset, numCols, numRows] = mEpiTiles[tileId];
  const auto start = chrono::steady_clock::now();
  memset(mMaskBuf.get(), 0, numCols * numRows);

  cv::Mat imgMat(imgNumRows, imgNumCols, mImgCvDT, mImgBufWork.get());
  cv::Mat epiMat(numRows, numCols, mEpiCvDT, mEpiBufWork.get());

  cv::Mat tfmMat(numRows, numCols, mTfmCvDT, mTfmBuf.get());
  tfmMat.forEach<cv::Vec2f>([](cv::Vec2f& tfm, const int*) {
    tfm[0] = -1; tfm[1] = -1; });
  EpiToImg(tileId, tfmMat);

  mRasResample->Resample(imgMat, epiMat, tfmMat);

  const chrono::duration<double> diff = chrono::steady_clock::now() - start;
  mPerf.tmProcess += diff.count();
}

/**
 *  \brief
 */
void EpipolarGenerator::WriteTile(const int tileId) {
  if (!TileId(tileId)) return;
  const auto start = chrono::steady_clock::now();

  const auto [colOffset, rowOffset, numCols, numRows] = mEpiTiles[tileId];
  CvRaster epiRaster(numCols, numRows, mEpiCvDT, colOffset, rowOffset, mEpiBufWrit.get());
  mEpiWriter->WriteRaster(&epiRaster);

  const chrono::duration<double> diff = chrono::steady_clock::now() - start;
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
  const int numGridCols = FillGridNodes(numCols, mStride, gridCols);
  std::vector<int> gridRows;
  const int numGridRows = FillGridNodes(numRows, mStride, gridRows);
  assert(2 <= numGridCols && 2 <= numGridRows);

  const int64_t numGridNodes = numGridCols * numGridRows;

  // Do geometry at grid nodes
#pragma omp parallel for
  for (int64_t g = 0; g < numGridNodes; ++g) {
    const auto [gridCol, gridRow] = IndexToCR(g, numGridCols);

    const int tileCol = gridCols[gridCol];
    const int tileRow = gridRows[gridRow];
    EpiToImg(tileId, tfmMat, tileCol, tileRow);
  }

  if (1 == mStride) return;
  
  // Densify non-grid positions
  const int64_t numPix = numCols * numRows;

  // todo method 0: loop over grid cells
  //const int64_t numGridCells = (numGridCols - 1) * (numGridRows);

  // method 1: loop over pixels
#pragma omp parallel for
  for (int64_t k = 0; k < numPix; ++k) {
    if (1 == mMaskBuf[k]) continue;
    const auto [tileCol, tileRow] = IndexToCR(k, numCols);

    const bool isGridCol = (0 == tileCol % mStride || tileCol == numCols - 1);
    const bool isGridRow = (0 == tileRow % mStride || tileRow == numRows - 1);
    if (isGridCol && isGridRow) continue; // failed grid point

    const int gridCol = std::min(tileCol / mStride, numGridCols - 2);
    const int gridRow = std::min(tileRow / mStride, numGridRows - 2);

    const int tileColL = gridCols[gridCol];
    const int tileColR = gridCols[gridCol + 1];
    const int tileRowU = gridRows[gridRow];
    const int tileRowL = gridRows[gridRow + 1];

    const int64_t i1 = numCols * tileRowU + tileColL;
    const int64_t i2 = numCols * tileRowU + tileColR;
    const int64_t i3 = numCols * tileRowL + tileColL;
    const int64_t i4 = numCols * tileRowL + tileColR;

    if (MaskAllZero(mMaskBuf.get(), i1, i2, i3, i4)) {
      continue;
    }

    if (MaskAllOne(mMaskBuf.get(), i1, i2, i3, i4)) {
      // Bilinear interpolation if all four grid points are valid
      // \todo pre-calculate fraction lookup table at locations 0 ~ mStride -1
      const double xf = double(tileCol - tileColL) / double(tileColR - tileColL);
      const double yf = double(tileRow - tileRowU) / double(tileRowL - tileRowU);

      const auto& tfm1 = tfmMat.at<cv::Vec2f>(tileRowU, tileColL);
      const auto& tfm2 = tfmMat.at<cv::Vec2f>(tileRowU, tileColR);
      const auto& tfm3 = tfmMat.at<cv::Vec2f>(tileRowL, tileColL);
      const auto& tfm4 = tfmMat.at<cv::Vec2f>(tileRowL, tileColR);

      auto& tfm = tfmMat.at<cv::Vec2f>(tileRow, tileCol);
      tfm[0] = float(LinearInterp(tfm1[0], tfm2[0], tfm3[0], tfm4[0], xf, yf));
      tfm[1] = float(LinearInterp(tfm1[1], tfm2[1], tfm3[1], tfm4[1], xf, yf));
      //mMaskBuf[k] = 1;     // uncomment if needing mask
    }
    else {
      // Do actual geometry if any grid node is invalid
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
    mMaskBuf[k] = 1;
  }
}
