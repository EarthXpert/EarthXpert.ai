/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/raster/irasterreader.h"
#include "core/utils/fsutils.h"

#include <mutex>
#include <tuple>
#include <vector>

namespace earthxpert {
class CvRaster;
class GdalRaster;

/**
 *  The class RasterReader provides format in-dependent functios to read from
 *  an image file with OpenCV as backend, and deserialize pixle block in
 *  an IRaster object having specific configurations, such as size, offset,
 *  margin, bands, etc.
 *
 *  Each RasterReader instance is connected and thus bound to specific bands.
 *  Therefore a new instance is needed to read from different subsets of
 *  available bands.
 */
class EXIO_EXPORTS CvRasterReader : public IRasterReader {
public:
  CvRasterReader(const std::string& imageFile, const std::vector<int>& bands = {});
  CvRasterReader(const fs::path& imageFile, const std::vector<int>& bands = {});
  CvRasterReader(const CvRasterReader&) = delete;
  CvRasterReader(CvRasterReader&&) = delete;
  ~CvRasterReader() = default;

  CvRasterReader& operator=(const CvRasterReader&) = delete;
  CvRasterReader& operator=(CvRasterReader&&) = delete;

private:
  std::vector<int> mBands;  // list of bands to read from
};

using UPExaiRasterReader = std::unique_ptr<CvRasterReader>;
using SPExaiRasterReader = std::shared_ptr<CvRasterReader>;
} // namespace earthxpert
