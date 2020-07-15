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

#include <omp.h>
#include <tuple>

using namespace earthxpert;

/**
 *  \brief
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
  IEpipolarModel const *const epiModel, IRasterWriter *const epi) {
  try {
    SetImageParams(img);
    SetEpipolarParams(epiModel, epi);

    LayoutTiles();
    ProcessTiles();
  }
  catch (...) {
    throw;
  }

  return true;
}

void EpipolarGenerator::SetImageParams(IRasterReader* const img) {
  assert(img);
  mImgReader = dynamic_cast<RasterReader* const>(img);
  std::tie(mImgNumCols, mImgNumRows) = mImgReader->GetSize();
}

void EpipolarGenerator::SetEpipolarParams(
  IEpipolarModel const* const epiModel, IRasterWriter* const epi) {
  assert(epiModel);
  mEpiModel = epiModel;

  assert(epi);
  mEpiWriter = dynamic_cast<RasterWriter* const>(epi);
  std::tie(mEpiNumCols, mEpiNumRows) = mEpiWriter->GetSize();
}

void EpipolarGenerator::LayoutTiles() {
  static constexpr int kNP = 8;
  RasterLayout rasLayout(mEpiNumCols, mEpiNumRows, kTileDim*10, kTileDim*10);
  mEpiTiles = rasLayout.GetRasterWindows();
  mNumTiles = int(mEpiTiles.size());

  std::array<Point2d, kNP> epiCorners;
  std::array<Point2d, kNP> imgCorners;

  int numImgTiles{};
  mImgTiles.clear();

  for (const auto [colOffset, rowOffset, numCols, numRows] : mEpiTiles) {
    epiCorners[0] = Point2d(colOffset, rowOffset);
    epiCorners[1] = Point2d(colOffset + numCols / 2, rowOffset);
    epiCorners[2] = Point2d(colOffset + numCols, rowOffset);
    epiCorners[3] = Point2d(colOffset + numCols, rowOffset + numRows / 2);
    epiCorners[4] = Point2d(colOffset + numCols, rowOffset + numRows);
    epiCorners[5] = Point2d(colOffset + numCols / 2, rowOffset + numRows;
    epiCorners[6] = Point2d(colOffset, rowOffset + numRows);
    epiCorners[7] = Point2d(colOffset, rowOffset + numRows / 2);

    double minImgX{ mImgNumCols };
    double minImgY{ mImgNumRows };
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
}

int64 EpipolarGenerator::GetTilesLimit(
  const std::vector<RasterWindow>& tiles, const RasterReader* reader) const {
  int maxTileNumPix{ -1 };
  for (auto tile : tiles) {
    const auto tileNumPix = tile.GetNumPixels();
    if (maxTileNumPix < tileNumPix)
      maxTileNumPix = tileNumPix;
  }

  return reader->GetRasterBufferNumBytes(maxTileNumPix);
}

void EpipolarGenerator::AllocateBuffers() {
  const auto imgBufNumBytes = GetTilesLimit(mImgTiles, mImgReader);
  if (mImgBufNumBytes < imgBufNumBytes) {
    mImgBuffer.reset();
    mImgBufNumBytes = imgBufNumBytes;
    mImgBuffer.reset(new uchar[mImgBufNumBytes]);
  }

  const auto epiBufNumBytes = GetTilesLimit(mEpiTiles, mEpiWriter->GetReader());
  if (mEpiBufNumBytes < epiBufNumBytes) {
    mEpiBuffer.reset();
    mEpiBufNumBytes = epiBufNumBytes;
    mEpiBuffer.reset(new uchar[mEpiBufNumBytes]);
  }
}

void EpipolarGenerator::ResetBuffers() {
  memset(mImgBuffer.get(), 0, mImgBufNumBytes);
  memset(mEpiBuffer.get(), 0, mEpiBufNumBytes);
}

void EpipolarGenerator::FreeBuffers() {
  mImgBuffer.reset();
  mEpiBuffer.reset();
}

void EpipolarGenerator::ProcessTiles() {
  for (int t = 0; t < mNumTiles; ++t) {
    ResetBuffers();

    ReadTile(t);
    ProcessTile(t);
    WriteTile(t);
  }
}

void EpipolarGenerator::ReadTile(const int tileId) {

  RasterWindow imgRw;
  if (!GetImageRasterWindow(epiRw, epiModel, pos, imgFileCols, imgFileRows, imgRw))
  {
    //this epi window has no overlap with the original image
//      epi->WriteRaster(&epiRaster);
    continue;
  }

  const auto imgbufNumBytes = imgReader->GetRasterBufferNumBytes(imgRw.numCols, imgRw.numRows);
  std::unique_ptr<uchar[]> imgBuffer{ new uchar[imgbufNumBytes] };
  CvRaster imgRaster(imgRw.numCols, imgRw.numRows, cvDT, imgRw.colOffset, imgRw.rowOffset,
    marginCols, marginRows, marginKind, imgBuffer.get());
  imgReader->ReadRaster(&imgRaster);

  cv::Mat map;
  map.create(epiRw.numRows, epiRw.numCols, CV_32FC2);
  GetEpiPixelMap(epiModel, pos, epiRw, imgRw, imgFileCols, imgFileRows, map);

  epi->WriteRaster(&epiRaster);
}

void EpipolarGenerator::ProcessTile(const int tileId) {
  CvRaster imgRaster;
  CvRaster epiRaster;
  CvRaster posRaster;

  mRasResample->Resample(imgRaster, epiRaster, posRaster);
}

void EpipolarGenerator::WriteTile(const int tileId) {
  const auto [colOffset, rowOffset, numCols, numRows] = mEpiTiles[tileId];

  CvRaster epiRaster(numCols, numRows, mEpiCvDT, colOffset, rowOffset, mEpiBuffer.get());
  mEpiWriter->WriteRaster(&epiRaster);
}
