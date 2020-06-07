/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/raster/irasterreader.h"

#include <vector>

namespace earthxpert {
class CvRaster;
class GdalRaster;

/**
 *  The class RasterNullReader provides mock behaviour that does nothing.
 *
 *  \todo deal with mock objects of RasterMetadata, SpatialReferenceSystem, 
 *  Geocoding, CvRaster, GdalRaster, etc.
 */
class EXIO_EXPORTS RasterNullReader final : public IRasterReader {
public:
  RasterNullReader(const std::string&, const std::vector<int>& = {}) {}
  RasterNullReader(GDALDataset*, const std::vector<int>& = {}) {}
  RasterNullReader(const RasterNullReader&) = delete;
  RasterNullReader(RasterNullReader&&) = delete;
  ~RasterNullReader() = default;

  RasterNullReader& operator=(const RasterNullReader&) = delete;
  RasterNullReader& operator=(RasterNullReader&&) = delete;

  GDALDataset* GetDataset() const noexcept { return nullptr; }

  std::string GetFileFormat() const { return ""; }

  std::unique_ptr<RasterMetadata> GetMetadata() const override { return nullptr; }

  std::pair<int, int> GetSize() const noexcept override { return { 0, 0 }; }

  GDALDataType GetDataType() const noexcept { return GDT_Unknown; }

  auto GetReadBands() const noexcept { return std::vector<int>{}; }

  int GetNumBands() const noexcept override { return 0; }

  void SetMargins(int, int, MarginKind) override {}

  std::pair<int, int> GetMargins() const noexcept override { return {0, 0}; }

  MarginKind GetMarginKind() const noexcept override { return MarginKind::eCONSTANT; }

  std::unique_ptr<SpatialReferenceSystem> GetCoordinateSystem() const override { return nullptr; }

  std::unique_ptr<Geocoding> GetGeocoding() const override { return nullptr; }

  int64 GetRasterBufferNumBytes(int, int) const noexcept { return 0; }

  std::unique_ptr<IRaster> ReadRaster(RasterType) override { return nullptr; }

  std::unique_ptr<IRaster> ReadRaster(int, int, int, int, RasterType) override { return nullptr; }

  void ReadRaster(IRaster* const) override {}

  void ReadRaster(CvRaster* const) {}
  void ReadRaster(GdalRaster* const) {}

  //std::unique_ptr<IPyramidReader> GetPyramidReader();
};
} // namespace earthxpert
