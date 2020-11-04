/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/raster/irasterwriter.h"
#include "io/raster/tiledrasterreader.h"

#include <shared_mutex>

namespace earthxpert {
/**
 *
 */
class EXIO_EXPORTS TiledRasterWriter /*final*/ : public IRasterWriter, public IRasterReader{
public:
  TiledRasterWriter(std::vector<RasterWindow>&& tiles,
    std::vector<std::string>&& imageFiles, const std::vector<int> & bands = {});
  TiledRasterWriter(std::vector<RasterWindow>&& tiles,
    std::vector<std::string>&& imageFiles, const std::string format,
    GDALDataType dataType, int numBands);
  TiledRasterWriter(const TiledRasterWriter&) = delete;
  TiledRasterWriter(TiledRasterWriter&&) = delete;
  ~TiledRasterWriter() = default;

  TiledRasterWriter& operator=(const TiledRasterWriter&) = delete;
  TiledRasterWriter& operator=(TiledRasterWriter&&) = delete;

private:
  void Initialize();

  std::unique_ptr<TiledRasterReader> mReader;  // pointer to reader implementation
  mutable std::shared_mutex mMutex;

  // cached information
  int mNumTiles{};
  int mFileNumCols{};   // width of image file [unit: pixel]
  int mFileNumRows{};   // height of image file
  std::vector<int> mBands;    // list of bands to write into
  GDALDataType mGdalDT{ GDT_Byte };     // GDAL data type of image file
  int mWriteNumBands{};   // number of bands to write
};

using UPTiledRasterWriter = std::unique_ptr<TiledRasterWriter>;
using SPTiledRasterWriter = std::shared_ptr<TiledRasterWriter>;
}
