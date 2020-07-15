/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/epipolar/exepipolar.h"
#include "epipolarmodel/iepipolarmodel.h"
#include "core/utils/rasterwindow.h"

#include <memory>
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
  bool Process(IRasterReader* img, IEpipolarModel const* epiModel, IRasterWriter* epi);

private:
  static constexpr int kTileDim = 256;

  const IRasterResample* mRasResample{};
  size_t mMemSize{};         // memory size allowed for processing
  unsigned short mStride{1}; // stride for rigorous epipolar geometry

  void SetImageParams(IRasterReader* const img);
  void SetEpipolarParams(IEpipolarModel const* epiModel, IRasterWriter* const epi);

  std::vector<RasterWindow> mImgTiles;
  std::vector<RasterWindow> mEpiTiles;
  void LayoutTiles();  // layout helper
  int64 GetTilesLimit(const std::vector<RasterWindow>& tiles,
    const RasterReader* reader) const;

  int64 mImgBufNumBytes{};
  std::unique_ptr<uchar[]> mImgBuffer;
  int64 mEpiBufNumBytes{};
  std::unique_ptr<uchar[]> mEpiBuffer;
  std::unique_ptr<uchar[]> mPosBuffer;
  void AllocateBuffers();
  void ResetBuffers();
  void FreeBuffers();

  void ProcessTiles();

  void ReadTile(int tileId);
  void ProcessTile(int tileId);
  void WriteTile(int tileId);

  // cached information
  int mMarginDim{};
  int mNumTiles{};

  RasterReader* mImgReader{};
  int mImgNumCols{};
  int mImgNumRows{};
  int mImgCvDT{};

  const IEpipolarModel* mEpiModel{};
  RasterWriter* mEpiWriter{};
  int mEpiNumCols{};
  int mEpiNumRows{};
  int mEpiCvDT{};
};
} // namespace earthxpert
