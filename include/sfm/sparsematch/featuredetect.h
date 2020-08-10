/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/sparsematch/ifeaturedetect.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/utils/rasterwindow.h"

namespace earthxpert {
class IRasterReader;
class RasterReader;

/**
 *  The class FeatureDetect implements the template methods.
 */
class EXSFM_EXPORTS FeatureDetect : public IFeatureDetect {
public:
  FeatureDetect(FeatureType featType, size_t memSize);
  virtual ~FeatureDetect() = default;

  bool Process(IRasterReader* img, cv::FileStorage* fs);

private:
  static constexpr int kMinNumTiles = 4;  // min number of tiles for an overlapped pipeline

  struct TileInfo {
    RasterWindow imgTile;
  };

  void Initialize();

  FeatureType mFeatureType{eSURF};
  size_t mMemSize{};
  cv::Ptr<cv::FeatureDetector> mDetector;

  void SetImageParams(IRasterReader* img);
  void SetOutputParams(cv::FileStorage* fs);

  std::vector<TileInfo> mTiles;

  inline RasterWindow& ImgTile(const int tileId) {
    assert(0 <= tileId && tileId < mNumTiles);
    return mTiles[tileId].imgTile;
  }

  void LayoutTiles();  // layout helper
  int DefineTiles(int tileNumCols, int tileNumRows);
  int64_t GetTilesLimit(const std::vector<TileInfo>& tiles) const;

  int64_t mImgBufNumBytes{};
  std::unique_ptr<uchar[]> mImgBufRead;   // work buffer of tile
  std::unique_ptr<uchar[]> mImgBufWork;   // work buffer of tile

  std::vector<cv::KeyPoint> mKpBufWork;    // key points
  std::vector<cv::KeyPoint> mKpBufWrite; 
  cv::Mat mDctBufWork;                      // descriptors
  cv::Mat mDctBufWrite;

  void AllocateBuffers();
  void ResetBuffers(int tileId);
  void SwapBuffers();
  void FreeBuffers();

  void ProcessTiles();

  inline bool TileId(const int tileId) {
    return (0 <= tileId && tileId < mNumTiles);
  }

  void ReadTile(int tileId);
  void ProcessTile(int tileId);
  void WriteTile(int tileId);

  // cached information
  int mNumPT{ 1 };    // number of processing threads
  int mMarginDim{};
  int mNumTiles{};

  RasterReader* mImgReader{};
  int mImgNumCols{};
  int mImgNumRows{};
  int mImgCvDT{};
  int mImgBpp{};  // bytes per pixel

  cv::FileStorage* mFS{};
};
}
