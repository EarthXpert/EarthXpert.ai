/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/raster/irasterwriter.h"
#include "io/raster/rasternullreader.h"

namespace earthxpert {
/**
 *  The class RasterNullWriter provides mock functios doing nothing.
 *
 *  \todo deal with mock objects of RasterMetadata, SpatialReferenceSystem, 
 *  Geocoding, CvRaster, GdalRaster, etc.
 */
class EXIO_EXPORTS RasterNullWriter final : public IRasterWriter, public IRasterReader {
public:
  RasterNullWriter(const std::string&, const std::vector<int> & = {}) {}
  RasterNullWriter(const std::string&, const std::string, int, int, GDALDataType, int) {}
  RasterNullWriter(const RasterNullWriter&) = delete;
  RasterNullWriter(RasterNullWriter&&) = delete;
  ~RasterNullWriter() = default;

  RasterNullWriter& operator=(const RasterNullWriter&) = delete;
  RasterNullWriter& operator=(RasterNullWriter&&) = delete;

  GDALDataset* GetDataset() const noexcept { return nullptr; }

  std::string GetFileFormat() const { return ""; }

  void SetMetadata(const RasterMetadata*) override {}

  void SetCoordinateSystem(const SpatialReferenceSystem*) override {}

  void SetGeocoding(const Geocoding*) override {}

  void WriteRaster(IRaster* const) override {}

  void WriteRaster(const CvRaster*) {}
  void WriteRaster(const GdalRaster*) {}

  void BuildPyramids() override {}

  std::unique_ptr<RasterMetadata> GetMetadata() const override { return nullptr; }

  std::pair<int, int> GetSize() const noexcept override { return {0, 0}; }

  GDALDataType GetDataType() const noexcept { return GDT_Unknown; }

  int GetNumBands() const noexcept override { return 0; }

  void SetMargins(int, int, MarginKind) override {}

  std::pair<int, int> GetMargins() const noexcept override { return {0, 0}; }

  MarginKind GetMarginKind() const noexcept override { return MarginKind::eCONSTANT; }

  std::unique_ptr<SpatialReferenceSystem> GetCoordinateSystem() const override { return nullptr; }

  std::unique_ptr<Geocoding> GetGeocoding() const override { return nullptr; }

  std::unique_ptr<IRaster> ReadRaster(RasterType) override { return nullptr; }

  std::unique_ptr<IRaster> ReadRaster(int, int, int, int, RasterType) override { return nullptr; }

  void ReadRaster(IRaster* const raster) override {}
};
} // namespace earthxpert
