/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sfm/sparsematch/featuredetect.h"
#include "core/datasource/cvraster.h"
#include "core/exceptions/exexcept.h"
#include "core/math/constants.h"
#include "core/math/math.h"
#include "core/utils/layoututils.h"
#include "core/utils/rasterlayout.h"
#include "io/raster/rasterreader.h"

#include <omp.h>
#include <tuple>

using namespace earthxpert;

FeatureDetect::FeatureDetect(FeatureType featType, const size_t memSize) 
  : mFeatureType(featType), mMemSize(memSize) {
  Initialize();
}

void FeatureDetect::Initialize() {
  mDetector = x2d::SURF::create();

  mNumPT = omp_get_max_threads() - 1;   // spare read thread
  omp_set_nested(2);
}

bool FeatureDetect::Process(IRasterReader* const img, cv::FileStorage* fs) {
  mPerf.Clear();
  auto start = mClock.TimeSinceStart();

  try {
    SetImageParams(img);
    SetOutputParams(fs);

    LayoutTiles();
    ProcessTiles();
  }
  catch (...) {
    throw SfMError(_("Process failed"));
  }

  mPerf.tmTotal = mClock.TimeSinceStart() - start;
  return true;
}

/**
 *  \brief
 */
void FeatureDetect::SetImageParams(IRasterReader* const img) {
  assert(img);
  mImgReader = dynamic_cast<RasterReader* const>(img);
  std::tie(mImgNumCols, mImgNumRows) = mImgReader->GetSize();
  mImgCvDT = mImgReader->GetCvDataType();
  mImgBpp = CV_ELEM_SIZE(mImgCvDT);
  std::tie(mMarginDim, std::ignore) = mImgReader->GetMargins();
}

void FeatureDetect::SetOutputParams(cv::FileStorage* fs) {
  assert(fs);
  mFS = fs;

  *mFS << "signature" << "exai.featuredetect";
  *mFS << "version" << std::vector<int>{1, 0, 0};
  //*mFS << "imgSize";
  //*mFS << "imgFile";
  *mFS << "featureType" << mFeatureType;
}

/**
 *  \brief allocate memory buffers
 */
void FeatureDetect::AllocateBuffers() {
  const auto imgBufNumPix = GetTilesLimit(mTiles);

  const int64_t imgBufNumBytes = mImgBpp * imgBufNumPix;
  if (mImgBufNumBytes < imgBufNumBytes) {
    mImgBufRead.reset();
    mImgBufWork.reset();
    mImgBufNumBytes = imgBufNumBytes;
    mImgBufRead.reset(new uchar[mImgBufNumBytes]);
    mImgBufWork.reset(new uchar[mImgBufNumBytes]);
  }
}

/**
 *  \brief reset buffers to reduce conditional branches
 *
 *  \param [in] tileId the index of tile to reset
 */
void FeatureDetect::ResetBuffers(const int tileId) {
  if (!TileId(tileId)) return;

  mKpBufWork.clear();
  mDctBufWork.release(); 
}

/**
 *  \brief swap read/write and work buffers for overlapping
 */
void FeatureDetect::SwapBuffers() {
  std::swap(mImgBufRead, mImgBufWork);
  std::swap(mKpBufWork, mKpBufWrite);
  cv::swap(mDctBufWork, mDctBufWrite);
}

/**
 *  \brief free buffers in case not needed any more
 */
void FeatureDetect::FreeBuffers() {
  mImgBufRead.reset();
  mImgBufWork.reset();
  mKpBufWork.swap(std::vector<cv::KeyPoint>());
  mKpBufWrite.swap(std::vector<cv::KeyPoint>());
  mDctBufWork.release();
  mDctBufWrite.release();
}

/**
 *  \brief pipelined overlapping of read/write and computation tasks
 */
void FeatureDetect::ProcessTiles() {
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
 *  \brief read a tile from raw image
 *
 *  \param [in] tileId the index of tile to read
 */
void FeatureDetect::ReadTile(const int tileId) {
  if (!TileId(tileId)) return;
  const auto [colOffset, rowOffset, numCols, numRows] = ImgTile(tileId);
  if (0 == numCols || 0 == numRows) return;
  const auto start = mClock.TimeSinceStart();

  CvRaster raster(numCols, numRows, mImgCvDT, colOffset, rowOffset, mImgBufRead.get());
  mImgReader->ReadRaster(&raster);

  mPerf.tmRead += mClock.TimeSinceStart() - start;
}

/**
 *  \brief process a tile
 *
 *  \param [in] tileId the index of tile to process
 */
void FeatureDetect::ProcessTile(const int tileId) {
  if (!TileId(tileId)) return;
  const auto start = mClock.TimeSinceStart();

  // in-memory ProcessTile(imgMat, kpVec, dctMat)

  mPerf.tmProcess += mClock.TimeSinceStart() - start;
}

/**
 *  \brief write descriptors
 *
 *  \param [in] tileId the index of tile to write
 */
void FeatureDetect::WriteTile(const int tileId) {
  if (!TileId(tileId)) return;
  const auto start = mClock.TimeSinceStart();

  *mFS << mDctBufWrite;

  mPerf.tmWrite += mClock.TimeSinceStart() - start;
}
