/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/epipolar/exepipolar.h"
#include "epipolarmodel/iepipolarmodel.h"
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
    size_t memSize, unsigned short stride = 1);

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

private:
  static constexpr int kTileDim = 256;

  const IRasterResample* mRasResample{};
  size_t mMemSize{};         // memory size allowed for processing
  unsigned short mStride{1}; // stride for rigorous epipolar geometry

  void SetImageParams(IRasterReader* const img);
  void SetEpipolarParams(IRasterWriter* const epi, IEpipolarModel const* epiModel);

  std::vector<RasterWindow> mImgTiles;
  std::vector<RasterWindow> mEpiTiles;
  void LayoutTiles();  // layout helper
  int64_t GetTilesLimit(const std::vector<RasterWindow>& tiles) const;

  int64_t mImgBufNumBytes{};
  std::unique_ptr<uchar[]> mImgBuffer;    // raw image tile
  
  int64_t mEpiBufNumBytes{};
  std::unique_ptr<uchar[]> mEpiBuffer;    // epipolar tile
  std::unique_ptr<uchar[]> mTfmBuffer;    // transfomration map
  std::unique_ptr<uchar[]> mMaskBuffer;   // epipolar mask

  void AllocateBuffers();
  void ResetBuffers(int tileId);
  void FreeBuffers();

  void ProcessTiles();

  void ReadTile(int tileId);
  void ProcessTile(int tileId);
  void WriteTile(int tileId);

  void GeoTransformEpiToImg(int tileId, cv::Mat& tfmMat);

  // cached information
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
