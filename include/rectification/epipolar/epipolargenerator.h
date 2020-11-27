/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/epipolar/exepipolar.h"
#include "rectification/epipolar/epiperf.h"
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
 *  The class EpipolarGenerator creates epipolar images from raw images
 *  given an epipolar model. In satellite remote sensing, these
 *  epipolar images are also named as OrthoReady stereo product.
 *  It is not thread-safe to process multiple images using the same
 *  generator object. Thus for parallel processing, create a generator
 *  object for each thread in the team.
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
  ~EpipolarGenerator() = default;

  EpipolarGenerator &operator=(const EpipolarGenerator &) = delete;
  EpipolarGenerator &operator=(EpipolarGenerator &&) = delete;

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
  static constexpr int kMinNumTiles = 4;  // min number of tiles for an overlapped pipeline
  static constexpr double kValidRatio = 0.85;  // by trials

  enum ExecPolicy {
    ePL2,   // two tasks pipelined
    ePL3,   // three tasks pipelined
    eSQ     // sequential
  };

  enum GeotransformAlgo {
    eBOUND,   // optimized algorithm using epipolar boundary
    eFULL     // general algorithm using full extent
  };

  struct TileInfo {
    bool hasImgTile{};
    GeotransformAlgo geoAlgo{ eFULL };
    RasterWindow imgTile;
    RasterWindow epiTile;
  };

  void Initialize();

  const IRasterResample* mRasResample{};
  size_t mMemSize{};         // memory size allowed for processing
  int mStride{1}; // stride for rigorous epipolar geometry
  GeotransformAlgo mGeoAlgo{eFULL};

  void SetImageParams(IRasterReader* img);
  void SetEpipolarParams(IRasterWriter* epi, IEpipolarModel const* epiModel);

  std::vector<TileInfo> mTiles;

  inline RasterWindow& ImgTile(const int tileId) {
    assert(0 <= tileId && tileId < mNumTiles);
    return mTiles[tileId].imgTile;
  }
  inline RasterWindow& EpiTile(const int tileId) {
    assert(0 <= tileId && tileId < mNumTiles);
    return mTiles[tileId].epiTile;
  }

  void LayoutTiles();  // layout helper
  int DefineTiles(int tileNumCols, int tileNumRows);
  RasterWindow DefineImgTile(const RasterWindow& epiTile);
  std::pair<int64_t, int64_t> GetTilesLimit(const std::vector<TileInfo>& tiles) const;

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

  void ProcessTilesPL2();
  void ProcessTilesPL3();
  void ProcessTilesSQ();

  template <int execPolicy>
  void ProcessTiles() {
    if constexpr(ePL2 == execPolicy)
      ProcessTilesPL2();
    else if constexpr(ePL3 == execPolicy)
      ProcessTilesPL3();
    else
      ProcessTilesSQ();
  }

  inline bool TileId(const int tileId) {
    return (0 <= tileId && tileId < mNumTiles);
  }

  void ReadTile(int tileId);
  void ProcessTile(int tileId);
  void WriteTile(int tileId);

  void GeotransformTile(int tileId, cv::Mat& tfmMat);

  void RigorousGridNodes(const int tileId, cv::Mat& tfmMat,
    const std::vector<int>& gridCols, const std::vector<int>& gridRows);
  void DensifyCells(const int tileId, cv::Mat& tfmMat,
    const std::vector<int>& gridCols, const std::vector<int>& gridRows);
  void DensifyPixels(const int tileId, cv::Mat& tfmMat,
    const std::vector<int>& gridCols, const std::vector<int>& gridRows);
  void EpiToImg(int tileId, cv::Mat& tfmMat,
    int tileCol, int tileRow, bool insideImg=true);

  // performance
  WallClock mClock;
  EpiGenPerf mPerf;

  void GatherStatTile(int tileId);

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

// typically for satellite remote sensing
using OrthoReadyGenerator = EpipolarGenerator;

using UPEpipolarGenerator = std::unique_ptr<EpipolarGenerator>;
using SPEpipolarGenerator = std::shared_ptr<EpipolarGenerator>;
} // namespace earthxpert
