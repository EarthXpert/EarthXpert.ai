/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "rectification/epipolar/epipolargenerator.h"
#include "3rdparty/taskflow/extaskflow.h"
#include "core/exceptions/exexcept.h"
#include "core/math/interpgridvec.h"
#include "core/utils/irasterresample.h"
#include "core/utils/geomutils.h"
#include "core/utils/layoututils.h"
#include "epipolarmodel/iepipolarmodel.h"
#include "io/raster/irasterreader.h"
#include "io/raster/rasterwriter.h"

#include <iostream>
#include <omp.h>

using namespace earthxpert;

/**
 *  \brief
 * 
 *  \param memSize [in] memory available [unit: bytes]
 *  \todo hardware configuration: hyper-threading
 */
EpipolarGenerator::EpipolarGenerator(const IRasterResample* rasResample, 
  const size_t memSize, const int stride)
  : mRasResample(rasResample), mMemSize(memSize), mStride(stride) {
  Initialize();
}

void EpipolarGenerator::Initialize() {
  mMarginDim = mRasResample->GetWinDim() / 2;

  mNumPT = omp_get_max_threads() - 2;   // spare read/write threads
  omp_set_nested(2);
  //cv::setNumThreads(mNumPT);
}

/**
 *  \brief
 *
 *  \return Returns true if operation succeeds
 */
bool EpipolarGenerator::Process(IRasterReader *const img, 
  IRasterWriter *const epi, IEpipolarModel const* epiModel) {
  mPerf.Clear();
  auto start = mClock.TimeSinceStart();

  try {
    SetImageParams(img);
    SetEpipolarParams(epi, epiModel);
    
    LayoutTiles();
    mPerf.tmInitial = mClock.TimeSinceStart() - start;

    ProcessTiles<eTF>(); // ePL2 ePL3
  }
  catch (...) {
    throw EpipolarError(_("Process failed"));
  }

  mPerf.tmTotal = mClock.TimeSinceStart() - start;
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

  mPerf.rawNumCols = mImgNumCols;
  mPerf.rawNumRows = mImgNumRows;
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

  mPerf.epiNumCols = mEpiNumCols;
  mPerf.epiNumRows = mEpiNumRows;

  assert(epiModel);
  mEpiModel = epiModel;

  const Point2d ic{ mImgNumCols * 0.5, mImgNumRows * 0.5 };
  Point2d ec, ic2;
  mEpiModel->RawToEpi(ic, ec);
  mEpiModel->EpiToRaw(ec + Point2d(1.0, 1.0), ic2);

  const double sample = cv::norm(ic2 - ic) * M_SQRT1_2;
  mSample = std::max<int>(1, std::lround(sample));
  
  const auto& bound = mEpiModel->GetBoundary();
  mPerf.validRatio = bg::area(bound) / mEpiNumCols / mEpiNumRows;
  mGeoAlgo = (kValidRatio < mPerf.validRatio) ? eFULL : eBOUND;
}

/**
 *  \brief allocate memory buffers
 *  \todo reduce mEpiMaskBuf to label grid nodes
 */
void EpipolarGenerator::AllocateBuffers() {
  const auto [imgBufNumPix, epiBufNumPix] = GetTilesLimit(mTiles);

  const int64_t imgBufNumBytes = mImgBpp * imgBufNumPix;
  if (mImgBufNumBytes < imgBufNumBytes) {
    mImgBufRead.reset();
    mImgBufWork.reset();
    mImgBufNumBytes = imgBufNumBytes;
    mImgBufRead.reset(new uchar[mImgBufNumBytes]);
    mImgBufWork.reset(new uchar[mImgBufNumBytes]);
  }

  const int64_t epiBufNumBytes = mEpiBpp * epiBufNumPix;
  if (mEpiBufNumBytes < epiBufNumBytes) {
    mEpiBufWork.reset();
    mEpiBufWrit.reset();
    mEpiMaskBuf.reset();
    mTfmBuf.reset();
    
    mEpiBufNumBytes = epiBufNumBytes;
    mEpiBufWork.reset(new uchar[mEpiBufNumBytes]);
    mEpiBufWrit.reset(new uchar[mEpiBufNumBytes]);
    mEpiMaskBuf.reset(new uchar[epiBufNumPix]);
    mTfmBuf.reset(new uchar[mTfmBpp * epiBufNumPix]);
  }
}

/**
 *  \brief reset buffers to reduce conditional branches
 *
 *  \param [in] tileId the index of tile to reset
 */
void EpipolarGenerator::ResetBuffers(const int tileId) {
  if (!TileId(tileId)) return;
  memset(mEpiBufWork.get(), 0, mEpiBpp * EpiTile(tileId).GetNumPixels());
}

/**
 *  \brief swap read/write and work buffers for overlapping
 */
void EpipolarGenerator::SwapBuffers() {
  std::swap(mImgBufRead, mImgBufWork);
  std::swap(mEpiBufWork, mEpiBufWrit);
}

/**
 *  \brief free buffers in case not needed any more
 */
void EpipolarGenerator::FreeBuffers() {
  mImgBufRead.reset();
  mImgBufWork.reset();
  mEpiBufWork.reset();
  mEpiBufWrit.reset();
  mTfmBuf.reset();
  mEpiMaskBuf.reset();
}

/**
 *  \brief pipelined overlapping of read/write and computation tasks
 *  \todo investigate GDAL thread model: does it allow 
 *  simuteneous read/write of different datasets?
 */
void EpipolarGenerator::ProcessTilesPL2() {
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
  *  \todo Read/Write 4x slower than SQ!
  */
void EpipolarGenerator::ProcessTilesPL3() {
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
 *  \brief pipelined overlapping of read, computation, write tasks by TaskFlow
 */
void EpipolarGenerator::ProcessTilesTF() {
  tf::Executor executor;
  tf::Taskflow taskflow;

  int t{ -1 };

  auto [ii, cd, rb, rt, pt, wt, sb, lt, dn] = taskflow.emplace(
    [&t]() { t = -1; },
    [this, &t]() { return t <= mNumTiles ? 0 : 1; },
    [this, &t]() { ResetBuffers(t); },
    [this, &t]() { ReadTile(t + 1); },
    [this, &t]() { ProcessTile(t); },
    [this, &t]() { WriteTile(t - 1); },
    [this, &t]() { SwapBuffers(); },
    [&t]() { ++t; return 0; },
    []() {});

  ii.name("init");
  cd.name("t <= mNumTiles");
  rb.name("ResetBuffers(t)");
  rt.name("ReadTile(t + 1)");
  pt.name("ProcessTile(t)");
  wt.name("WriteTile(t - 1)");
  sb.name("SwapBuffers()");
  lt.name("loop over tiles");
  dn.name("done");

  ii.precede(cd);
  cd.precede(rb, dn);
  rb.precede(rt, pt, wt);
  sb.succeed(rt, pt, wt);
  lt.succeed(sb);
  cd.succeed(lt);

  //taskflow.dump(std::cout);
  //assert(8 == taskflow.num_tasks());
  executor.run(taskflow).wait();
}

/**
 *  \brief read a tile from raw image
 *
 *  \param [in] tileId the index of tile to read
 */
void EpipolarGenerator::ReadTile(const int tileId) {
  if (!TileId(tileId)) return;
  const auto [colOffset, rowOffset, numCols, numRows] = ImgTile(tileId);
  if (0 == numCols || 0 == numRows) return;
  const auto start = mClock.TimeSinceStart();
  //memset(mImgBufRead.get(), 0, size_t(mImgBpp) * numCols * numRows);

  CvRaster imgRaster(numCols, numRows, mImgCvDT, 
    colOffset, rowOffset, mImgBufRead.get());
  mImgReader->ReadRaster(&imgRaster);

  mPerf.tmRead += mClock.TimeSinceStart() - start;
}

/**
 *  \brief process a tile
 *
 *  \param [in] tileId the index of tile to process
 */
void EpipolarGenerator::ProcessTile(const int tileId) {
  if (!TileId(tileId)) return;
  const auto [imgNumCols, imgNumRows] = ImgTile(tileId).GetSize();
  if (0 == imgNumCols || 0 == imgNumRows) return;
  const auto start = mClock.TimeSinceStart();

  const auto [colOffset, rowOffset, numCols, numRows] = EpiTile(tileId);
  memset(mEpiMaskBuf.get(), eMK_BKGD, size_t(numCols) * numRows);

  cv::Mat imgMat(imgNumRows, imgNumCols, mImgCvDT, mImgBufWork.get());
  cv::Mat epiMat(numRows, numCols, mEpiCvDT, mEpiBufWork.get());
  cv::Mat epiMask(numRows, numCols, CV_8UC1, mEpiMaskBuf.get());
  cv::Mat tfmMat(numRows, numCols, mTfmCvDT, mTfmBuf.get());

  // in-memory ProcessTile(imgMat, epiMat, tfmMat)
  tfmMat.forEach<cv::Vec2f>([](cv::Vec2f& tfm, const int*) {
    tfm[0] = -1; tfm[1] = -1; });
  GeotransformTile(tileId, tfmMat);

  mRasResample->Resample(imgMat, nullptr, epiMat, &epiMask, tfmMat); // ResampleTile

  //GatherStatTile(tileId);
  mPerf.tmProcess += mClock.TimeSinceStart() - start;
}

/**
 *  \brief write a tile to epipolar image
 *
 *  \param [in] tileId the index of tile to write
 */
void EpipolarGenerator::WriteTile(const int tileId) {
  if (!TileId(tileId)) return;
  const auto start = mClock.TimeSinceStart();

  const auto [colOffset, rowOffset, numCols, numRows] = EpiTile(tileId);
  CvRaster epiRaster(numCols, numRows, mEpiCvDT, 
    colOffset, rowOffset, mEpiBufWrit.get());
  mEpiWriter->WriteRaster(&epiRaster);

  mPerf.tmWrite += mClock.TimeSinceStart() - start;
}

/**
 *  \brief Do geometric transformation to populate the map.
 *  first, do rigorous calculation of corresponding positions on
 *  raw images at grid nodes each giving a mark of
 *  either eNORM if inside raw tile otherwise eBKGD if outside;
 *  second, do linear interpolation for a full cell (i.e. four corners
 *  marked as eNORM), rigorous calculation for a partial cell (i.e.
 *  1~3 corners being eNORM, skip for empty cell (i.e. four eBKGD corners).
 *
 *  \param [in] tileId the index of tile to process
 *  \param [in] tfmMat transfomration map
 *
 *  \todo first, mark candidate eNORM nodes by identifying cells
 *  overlaping/intersecting the epipolar polygon that defines the
 *  boundary of valid region, then do rigorous calculation at candidate
 *  nodes, retaining the eNORM mark provided the EpiToRaw succeed;
 *  second, do linear interpolation for a full cell (i.e. four corners
 *  marked as eNORM), rarely rigorous calculation for a partial cell (i.e.
 *  1~3 corners being eNORM.
 *
 *  \hint (Brehensen) draw line segments of the epipolar polygon on a
 *  low-res image downsampled by a factor of mSample to identify cells
 *  on the boundary, then do sweep-line to scan internal cells.
 */
void EpipolarGenerator::GeotransformTile(const int tileId, cv::Mat& tfmMat) {
  const auto [numCols, numRows] = EpiTile(tileId).GetSize();

  // grid col/row positions for rigorous geometry
  std::vector<int> gridCols, gridRows;
  FillNodesByStride(numCols, mStride, gridCols);
  FillNodesByStride(numRows, mStride, gridRows);

  RigorousGridNodes(tileId, tfmMat, gridCols, gridRows);

  DensifyCells(tileId, tfmMat, gridCols, gridRows);
  //DensifyPixels(tileId, tfmMat, gridCols, gridRows);
}

/**
 *  \brief do rigorous geometric transformation on grid nodes
 *
 *  \param [in] tileId the index of tile to process
 *  \param [in] tfmMat transfomration map
 *  \param [in] gridCols grid col positions for rigorous geometry
 *  \param [in] gridRows grid row positions for rigorous geometry
 */
void EpipolarGenerator::RigorousGridNodes(const int tileId, cv::Mat& tfmMat,
  const std::vector<int>& gridCols, const std::vector<int>& gridRows) {
  auto start = mClock.TimeSinceStart();
  const int numGridCols = static_cast<int>(gridCols.size());
  const int numGridRows = static_cast<int>(gridRows.size());
  assert(2 <= numGridCols && 2 <= numGridRows);
  const int64_t numGridNodes = int64_t(numGridCols) * numGridRows;

#pragma omp parallel for num_threads(mNumPT)
  for (int64_t n = 0; n < numGridNodes; ++n) {
    const auto [gridCol, gridRow] = IndexToCR(n, numGridCols);

    const int tileCol = gridCols[gridCol];
    const int tileRow = gridRows[gridRow];
    EpiToImg(tileId, tfmMat, tileCol, tileRow);
  }

  mPerf.tmRigorous += mClock.TimeSinceStart() - start;
}

/**
 *  \brief densify the geometric transformation at 
 *  non-grid positions by looping over grid cells
 *
 *  \param [in] tileId the index of tile to process
 *  \param [in] tfmMat transfomration map
 *  \param [in] gridCols grid col positions for rigorous geometry
 *  \param [in] gridRows grid row positions for rigorous geometry
 *
 *  \remark 15% faster than PerPixels
 */
void EpipolarGenerator::DensifyCells(const int tileId, cv::Mat& tfmMat,
  const std::vector<int>& gridCols, const std::vector<int>& gridRows) {
  if (1 == mStride) return;  
  const auto start = mClock.TimeSinceStart();
  const auto [numCols, numRows] = EpiTile(tileId).GetSize();
  const int numGridCols = int(gridCols.size());
  const int numGridRows = int(gridRows.size());
  const int64_t numGridCells = 
    (int64_t(numGridCols) - 1) * (int64_t(numGridRows) - 1);

#pragma omp parallel for num_threads(mNumPT) \
    schedule(dynamic, (numGridCols - 1)*2)
  for (int64_t g = 0; g < numGridCells; ++g) {
    const auto [gridCol, gridRow] = IndexToCR(g, numGridCols-1);

    const auto [tileColL, tileRowU, tileColR, tileRowL] = 
      GridCorners(gridCols, gridRows, gridCol, gridRow);
    const auto [i1, i2, i3, i4] = CornersIndex(tileColL, tileRowU,
      tileColR, tileRowL, numCols);

    if (MaskAllZero(mEpiMaskBuf.get(), i1, i2, i3, i4)) 
      continue;
    
    const int numCellCols = std::min<int>(mStride, tileColR - tileColL + 1);
    const int numCellRows = std::min<int>(mStride, tileRowL - tileRowU + 1);

    if (MaskAllOne(mEpiMaskBuf.get(), i1, i2, i3, i4)) {
      const auto invTileCols = 1.0 / (tileColR - tileColL);
      const auto invTileRows = 1.0 / (tileRowL - tileRowU);
      const InterpGridVec<float, 2, std::false_type> interp(
        MatCorners<cv::Vec2f>(tfmMat, tileColL, tileRowU, tileColR, tileRowL));

      double xf{};
      for (int c = 0; c < numCellCols; ++c, xf+=invTileCols) {
        const int tileCol = tileColL + c;

        double yf{};
        for (int r = 0; r < numCellRows; ++r, yf+=invTileRows) {
          const int tileRow = tileRowU + r;
          const int64_t k = CRToIndex(tileCol, tileRow, numCols);
          if (eMK_NORM == mEpiMaskBuf[k]) continue;

          tfmMat.at<cv::Vec2f>(tileRow, tileCol) = interp.GetVal<float>(xf, yf);
          mEpiMaskBuf[k] = eMK_FAST; 
        }
      }
    }
    else {
      for (int c = 0; c < numCellCols; ++c) {
        const int tileCol = tileColL + c;

        for (int r = 0; r < numCellRows; ++r) {
          const int tileRow = tileRowU + r;
          const int64_t k = CRToIndex(tileCol, tileRow, numCols);
          if (eMK_NORM == mEpiMaskBuf[k]) continue;

          EpiToImg(tileId, tfmMat, tileCol, tileRow);
        }
      }
    }
  }
  mPerf.tmDensify += mClock.TimeSinceStart() - start;
}

/** 
 *  \brief densify the geometric transformation by looping over pixels
 *
 *  \param [in] tileId the index of tile to process
 *  \param [in] tfmMat transfomration map
 *  \param [in] gridCols grid col positions for rigorous geometry
 *  \param [in] gridRows grid row positions for rigorous geometry
 */
void EpipolarGenerator::DensifyPixels(const int tileId, cv::Mat& tfmMat,
  const std::vector<int>& gridCols, const std::vector<int>& gridRows) {
  if (1 == mStride) return;  // Densify non-grid positions
  const auto start = mClock.TimeSinceStart();
  const auto [numCols, numRows] = EpiTile(tileId).GetSize();
  const int64_t numPix = int64_t(numCols) * numRows;
  const int numGridCols = int(gridCols.size());
  const int numGridRows = int(gridRows.size());

#pragma omp parallel for num_threads(mNumPT) schedule(dynamic, numCols*16)
  for (int64_t k = 0; k < numPix; ++k) {
    if (eMK_NORM == mEpiMaskBuf[k]) continue;
    const auto [tileCol, tileRow] = IndexToCR(k, numCols);

    //const bool isGridCol = (0 == tileCol % mStride || tileCol == numCols - 1);
    //const bool isGridRow = (0 == tileRow % mStride || tileRow == numRows - 1);
    //if (isGridCol && isGridRow) continue; // failed grid point

    const int gridCol = std::min(tileCol / mStride, numGridCols - 2);
    const int gridRow = std::min(tileRow / mStride, numGridRows - 2);

    const auto [tileColL, tileRowU, tileColR, tileRowL] =
      GridCorners(gridCols, gridRows, gridCol, gridRow);
    const auto [i1, i2, i3, i4] = CornersIndex(tileColL, tileRowU,
      tileColR, tileRowL, numCols);

    if (MaskAllZero(mEpiMaskBuf.get(), i1, i2, i3, i4)) 
      continue;

    if (MaskAllOne(mEpiMaskBuf.get(), i1, i2, i3, i4)) {
      // Bilinear interpolation if all four grid points are valid
      // \todo pre-calculate fraction lookup table at locations 0 ~ mStride -1
      const auto [tfm1, tfm2, tfm3, tfm4] = 
        MatCorners<cv::Vec2f>(tfmMat, tileColL, tileRowU, tileColR, tileRowL);
      const auto xf = double(tileCol - tileColL) / double(tileColR - tileColL);
      const auto yf = double(tileRow - tileRowU) / double(tileRowL - tileRowU);

      tfmMat.at<cv::Vec2f>(tileRow, tileCol) = 
        LinearInterp<float, 2, double>(tfm1, tfm2, tfm3, tfm4, xf, yf);
      mEpiMaskBuf[k] = eMK_FAST;   
    }
    else {
      // Do actual geometry if any grid node is invalid
      EpiToImg(tileId, tfmMat, tileCol, tileRow);
    }
  }
  mPerf.tmDensify += mClock.TimeSinceStart() - start;
}

/**
 *  \brief Do rigorous transformation at given epipolar pixel
 *
 *  \param [in] tileId the index of tile to process
 *  \param [in] tfmMat transfomration map
 *  \param [in] tileCol column position of an epipolar pixel
 *  \param [in] tileRow row position of an epipolar pixel
 *  \param [in] insideImg check if the point inside the image tile
 */
void EpipolarGenerator::EpiToImg(const int tileId, cv::Mat& tfmMat,
  const int tileCol, const int tileRow, const bool insideImg) {
  const auto [imgColOff, imgRowOff, imgNumCols, imgNumRows] = ImgTile(tileId);
  const auto [colOff, rowOff, numCols, numRows] = EpiTile(tileId);

  Point2d epi(0.5 + colOff + tileCol, 0.5 + rowOff + tileRow), img;
  const auto ret = mEpiModel->EpiToRaw(epi, img);
  if (!ret) return;
  
  const float tfmCol = float(img.x - 0.5 - imgColOff);
  const float tfmRow = float(img.y - 0.5 - imgRowOff);

  if (!insideImg || (0 <= tfmCol && tfmCol <= imgNumCols - 1 &&
                     0 <= tfmRow && tfmRow <= imgNumRows - 1)) {
    auto& tfm = tfmMat.at<cv::Vec2f>(tileRow, tileCol);
    tfm[0] = tfmCol;
    tfm[1] = tfmRow;

    const int64_t k = CRToIndex(tileCol, tileRow, numCols);
    mEpiMaskBuf[k] = eMK_NORM; 
  }
}

/**
 *  \brief count number of pixels in each category
 *
 *  \param [in] tileId the index of tile to analyze
 */
void EpipolarGenerator::GatherStatTile(const int tileId) {
  const auto [colOffset, rowOffset, numCols, numRows] = EpiTile(tileId);
  const int64_t numPix = int64_t(numCols) * numRows;

//#pragma omp parallel for reduction(+ : )
  for (int64_t k = 0; k < numPix; ++k) {
    mPerf.pixels[mEpiMaskBuf[k]] ++;
  }
}
