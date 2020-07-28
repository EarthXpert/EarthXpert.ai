/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/epipolar/exepipolar.h"
#include "rectification/epipolar/utils.h"
#include "epipolarmodel/iepipolarmodel.h"
#include "core/system/clock.h"
#include "core/utils/rasterwindow.h"

#include <memory>
#include <cstdint>
#include <vector>

namespace earthxpert {
class IRasterReader;
class IRasterResample;
class IRasterWriter;
class IEpipolarModel;
class RasterReader;
class RasterWriter;

/**
 *  The class EpipolarGenerator creates epipolar images from raw images.
 */
class EXEPIPOLAR_EXPORTS EpipolarGenerator {
public:
  /**
   *  \brief
   */
  EpipolarGenerator(const IRasterResample* rasResample, 
    size_t memSize, int stride = 1);

  EpipolarGenerator(const EpipolarGenerator &) = delete;
  EpipolarGenerator(EpipolarGenerator &&) = delete;
  EpipolarGenerator &operator=(const EpipolarGenerator &) = delete;
  EpipolarGenerator &operator=(EpipolarGenerator &&) = delete;
  ~EpipolarGenerator() = default;

  /**
   *  \brief
   *
   *  \return Returns true if operation succeeds
   */
  bool Process(IRasterReader* img, IRasterWriter* epi, IEpipolarModel const* epiModel);

  /**
   *  \return Returns performance info
   */
  const EpiGenPerf* GetPerfMettrics() noexcept { 
    mPerf.tmResample = mPerf.tmProcess - mPerf.tmRigorous - mPerf.tmDensify;
    return &mPerf; 
  }

private:
  enum ExecPolicy {
    ePL2,   // two tasks pipelined 
    ePL3,   // three tasks pipelined 
    eSQ     // sequential
  };
  static constexpr int kTileDim = 256;
  static constexpr int kMinNumTiles = 4;  // min number of tiles for an overlapped pipeline

  const IRasterResample* mRasResample{};
  size_t mMemSize{};         // memory size allowed for processing
  int mStride{1}; // stride for rigorous epipolar geometry

  void SetImageParams(IRasterReader* const img);
  void SetEpipolarParams(IRasterWriter* const epi, IEpipolarModel const* epiModel);

  std::vector<RasterWindow> mImgTiles;
  std::vector<RasterWindow> mEpiTiles;
  void LayoutTiles();  // layout helper
  int DefineTiles(int tileNumCols, int tileNumRows);
  RasterWindow DefineImgTile(const RasterWindow& epiTile);
  int64_t GetTilesLimit(const std::vector<RasterWindow>& tiles) const;

  int64_t mImgBufNumBytes{};
  std::unique_ptr<uchar[]> mImgBufRead;   // read buffer of raw image tile
  std::unique_ptr<uchar[]> mImgBufWork;   // work buffer

  int64_t mEpiBufNumBytes{};
  std::unique_ptr<uchar[]> mEpiBufWork;   // work buffer of epipolar tile
  std::unique_ptr<uchar[]> mEpiBufWrit;   // write buffer
  std::unique_ptr<uchar[]> mMaskBuf;      // epipolar mask
  std::unique_ptr<uchar[]> mTfmBuf;       // transfomration map

  void AllocateBuffers();
  void ResetBuffers(int tileId);
  void SwapBuffers();
  void FreeBuffers();

  template <int execPolicy>
  void ProcessTiles() {
    if constexpr (ePL2 == execPolicy)
      ProcessTilesPL2();
    else if constexpr (ePL3 == execPolicy)
      ProcessTilesPL3();
    else
      ProcessTilesSQ();
  }

  void ProcessTilesPL2();
  void ProcessTilesPL3();
  void ProcessTilesSQ();

  inline bool TileId(const int tileId) {
    return (0 <= tileId && tileId < mNumTiles);
  }

  void ReadTile(int tileId);
  void ProcessTile(int tileId);
  void WriteTile(int tileId);

  void Geotransform(int tileId, cv::Mat& tfmMat);
  void DensifyPerCells(const int tileId, cv::Mat& tfmMat,
    const std::vector<int>& gridCols, const std::vector<int>& gridRows);
  void DensifyPerPixels(const int tileId, cv::Mat& tfmMat,
    const std::vector<int>& gridCols, const std::vector<int>& gridRows);
  void EpiToImg(int tileId, cv::Mat& tfmMat, int tileCol, int tileRow);

  // performance
  WallClock mClock;
  EpiGenPerf mPerf;

  void GatherStatistics(int tileId);

  // cached information
  int mNumPT{ 1 };    // number of processing threads
  int mMarginDim{};
  int mNumTiles{};

  RasterReader* mImgReader{};
  int mImgNumCols{};
  int mImgNumRows{};
  int mImgCvDT{};
  int mImgBpp{};  // bytes per pixel

  RasterWriter* mEpiWriter{};
  int mEpiNumCols{};
  int mEpiNumRows{};
  int mEpiCvDT{};
  int mEpiBpp{};

  const IEpipolarModel* mEpiModel{};
  int mSample{ 1 };
  int mTfmCvDT{ CV_32FC2 };
  int mTfmBpp{ CV_ELEM_SIZE(CV_32FC2) };
};
} // namespace earthxpert
