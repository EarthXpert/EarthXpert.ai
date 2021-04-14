/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/raster/irasterwriter.h"
#include "io/raster/nullrasterreader.h"

namespace earthxpert {
/**
 *  The class NullRasterWriter provides mock functios doing nothing.
 *
 *  \todo deal with mock objects of RasterMetadata, OGRSpatialRef, 
 *  Geocoding, CvRaster, GdalRaster, etc.
 */
class EXIO_EXPORTS NullRasterWriter final : public IRasterWriter, public IRasterReader {
public:
  NullRasterWriter(const std::string&, const std::vector<int> & = {}) {}
  NullRasterWriter(const std::string&, const std::string, int, int, GDALDataType, int) {}
  NullRasterWriter(const NullRasterWriter&) = delete;
  NullRasterWriter(NullRasterWriter&&) = delete;
  ~NullRasterWriter() = default;

  NullRasterWriter& operator=(const NullRasterWriter&) = delete;
  NullRasterWriter& operator=(NullRasterWriter&&) = delete;

  GDALDataset* GetDataset() const noexcept { return nullptr; }

  std::string GetFileFormat() const { return ""; }

  void SetMetadata(const RasterMetadata*) override {}

  void SetNodataValues(double) {}
  void SetNodataValues(const std::vector<double>&) {}

  void SetCoordinateSystem(const OGRSpatialRef*) override {}

  void SetGeocoding(const Geocoding*) override {}

  void WriteRaster(IRaster* const) override {}

  void WriteRaster(const CvRaster*) {}
  void WriteRaster(const GdalRaster*) {}

  void BuildPyramids() override {}

  std::unique_ptr<RasterMetadata> GetMetadata() const override { return nullptr; }

  std::pair<int, int> GetSize() const noexcept override { return {0, 0}; }

  GDALDataType GetDataType() const noexcept { return GDT_Unknown; }

  int GetNumBands() const noexcept override { return 0; }

  std::vector<double>& GetNodataValues() noexcept {
    static std::vector<double> nodataValues;
    return nodataValues;
  }

  void SetMargins(int, int, MarginKind) override {}

  std::pair<int, int> GetMargins() const noexcept override { return {0, 0}; }

  MarginKind GetMarginKind() const noexcept override { return MarginKind::eMK_CONSTANT; }

  UPSpatialRefSys GetCoordinateSystem() const override { return nullptr; }

  std::unique_ptr<Geocoding> GetGeocoding() const override { return nullptr; }

  std::unique_ptr<IRaster> ReadRaster(RasterType) override { return nullptr; }

  std::unique_ptr<IRaster> ReadRaster(int, int, int, int, RasterType) override { return nullptr; }
  std::unique_ptr<IRaster> ReadRaster(const RasterWindow&, RasterType) override { return nullptr; }

  void ReadRaster(IRaster* const raster) override {}
};

using UPNullRasterWriter = std::unique_ptr<NullRasterWriter>;
using SPNullRasterWriter = std::shared_ptr<NullRasterWriter>;
} // namespace earthxpert
