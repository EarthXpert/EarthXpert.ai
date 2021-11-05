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
 *  The class ExaiRasterReader provides specific functios to read from
 *  an image file with the EXAI private format, and deserialize pixle block in
 *  an IRaster object having specific configurations, such as size, offset,
 *  margin, bands, etc.
 *
 *  Each ExaiRasterReader instance is connected and thus bound to specific bands.
 *  Therefore a new instance is needed to read from different subsets of
 *  available bands.
 */
class EXIO_EXPORTS ExaiRasterReader : public IRasterReader {
public:
  ExaiRasterReader(const std::string& imageFile, const std::vector<int>& bands = {});
  ExaiRasterReader(const fs::path& imageFile, const std::vector<int>& bands = {});
  ExaiRasterReader(const ExaiRasterReader&) = delete;
  ExaiRasterReader(ExaiRasterReader&&) = delete;
  ~ExaiRasterReader() = default;

  ExaiRasterReader& operator=(const ExaiRasterReader&) = delete;
  ExaiRasterReader& operator=(ExaiRasterReader&&) = delete;

private:
  std::vector<int> mBands;  // list of bands to read from
};

using UPExaiRasterReader = std::unique_ptr<ExaiRasterReader>;
using SPExaiRasterReader = std::shared_ptr<ExaiRasterReader>;
} // namespace earthxpert
