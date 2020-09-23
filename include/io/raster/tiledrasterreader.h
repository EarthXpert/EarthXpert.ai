/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/raster/irasterreader.h"
#include "core/utils/fsutils.h"
#include "core/utils/rasterwindow.h"

#include <mutex>
#include <tuple>
#include <vector>

namespace earthxpert {
class CvRaster;
class GdalRaster;

class EXIO_EXPORTS TiledRasterReader /*final*/ : public IRasterReader {
public:
  TiledRasterReader(std::vector<RasterWindow>&& tiles, 
    std::vector<std::string>&& imageFiles, const std::vector<int>& bands = {});
  TiledRasterReader(const TiledRasterReader&) = delete;
  TiledRasterReader(TiledRasterReader&&) = delete;
  ~TiledRasterReader() = default;

  TiledRasterReader& operator=(const TiledRasterReader&) = delete;
  TiledRasterReader& operator=(TiledRasterReader&&) = delete;

private:
  std::vector<RasterWindow> mTiles;   // layout of the entire scene
  std::vector<std::string> mImageFiles; // one file per tile

  void Initialize();

  UPGDALDataset mDataset;  // GDALDataset object of current tile
  std::vector<int> mBands;  // list of bands to read from

  // cached information
  int mNumTiles{};
  int mFileNumBands{};  // total number of bands of image file
  int mReadNumBands{};  // number of bands to read from
  int mFileNumCols{};   // width of image file [unit: pixel]
  int mFileNumRows{};   // height of image file
  std::vector<double> mNodataValues;
  GDALDataType mGdalDT{};
  int mCvDT{};
};

using UPTiledRasterReader = std::unique_ptr<TiledRasterReader>;
using SPTiledRasterReader = std::shared_ptr<TiledRasterReader>;
}
