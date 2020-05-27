/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/raster/irasterwriter.h"
#include "io/raster/rasterreader.h"
#include "core/spatialref/geocoding.h"

namespace earthxpert {
/**
 *  The class RasterWriter provides functios to write into an image file
 *  with GDAL as backend, and serialize pixle block in an IRaster object
 *  having specific configurations, such as size, offset, bands, etc.
 *  Each RasterWriter instance is connected and thus bound to specific bands.
 *  Therefore a new instance is needed to write into different subsets of bands.
 *
 *  \remark will revisit for a better design.
 */
class EXIO_EXPORTS RasterWriter final : public IRasterWriter, public IRasterReader {
public:
  RasterWriter(const std::string& imageFile);
  RasterWriter(const std::string& imageFile, const std::vector<int>& bands);
  RasterWriter(const std::string& imageFile, const std::string format,
    int numCols, int numRows, GDALDataType dataType, int numBands);
  RasterWriter(GDALDataset* dataset);
  RasterWriter(GDALDataset* dataset, const std::vector<int>& bands);
  RasterWriter(const RasterWriter&) = delete;
  RasterWriter(RasterWriter&&) = delete;
  ~RasterWriter() = default;

  RasterWriter& operator=(const RasterWriter&) = delete;
  RasterWriter& operator=(RasterWriter&&) = delete;

  /**
   *  \return Returns the backend GDALDataset object. RasterWriter
   *  exports partially the GDAL functions. The caller could exploit
   *  the full GDAL capabilities via the returned object pointer.
   */
  GDALDataset* GetDataset() const noexcept {
    return mDataset;
  }

  /**
   *  \return Returns name of file format.
   */
  std::string GetFileFormat() const {
    return mDataset->GetDriverName();
  }

  /**
   *  \brief Inject metadata tags
   *
   *  \param [in] metaData the metadata tags
   */
  void SetMetadata(const RasterMetadata* metaData) override;

  /**
   *  \brief Inject the ground coordinate system where the image file operates
   *
   *  \param [in] spatialref the coordinate system
   */
  void SetCoordinateSystem(const SpatialReferenceSystem* srs) override;

  /**
   *  \brief Inject the geocoding information into the image file.
   *
   *  \param [in] geocoding geocoding information
   */
  void SetGeocoding(const Geocoding* geocoding) override;

  /**
   *  \brief Write the pixel block contained in the raster object into image file.
   *
   *  \param [in] raster the raster object to be written into image file on disk
   */
  void WriteRaster(IRaster* const raster) override;

  void WriteRaster(const CvRaster* raster);
  void WriteRaster(const GdalRaster* raster);

  /**
   *  \brief Generate downsampled image overviews.
   */
  void BuildPyramids() override;

  /**
   *  \return Returns metadata tags
   */
  std::unique_ptr<RasterMetadata> GetMetadata() const override {
    return mReader->GetMetadata();
  }

  /**
   *  \return Returns width and height of image file [unit: pixels]
   */
  std::pair<int, int> GetSize() const noexcept override {
    return mReader->GetSize();
  }

  /**
   *  \return Returns GDAL data type.
   */
  GDALDataType GetDataType() const noexcept {
    return mGdalDT;
  }

  /**
   *  \return Return the number of channels contained in the image file
   */
  int GetNumBands() const noexcept override {
    return mReader->GetNumBands();
  }

  /**
   *  \brief Set the margin size and kind.
   *
   *  \param [in] marginCols the horizontal margin size
   *  \param [in] marginRows the vertical margin size
   *  \param [in] marginKind the margin kind.
   */
  void SetMargins(const int marginCols, const int marginRows,
    const MarginKind marginKind) override {
    return mReader->SetMargins(marginCols, marginRows, marginKind);
  }

  /**
   *  \return Returns margin size.
   */
  std::pair<int, int> GetMargins() const noexcept override {
    return mReader->GetMargins();
  }

  /**
   *  \return Returns the margin kind.
   */
  MarginKind GetMarginKind() const noexcept override {
    return mReader->GetMarginKind();
  }

  /**
   *  \brief Fetch the ground coordinate system where the image file operates.
   *
   *  \return Returns pointer to a coordinate system
   */
  std::unique_ptr<SpatialReferenceSystem> GetCoordinateSystem() const override {
    return mReader->GetCoordinateSystem();
  }

  /**
   *  \brief Fetch the geocoding information of the image file.
   */
  std::unique_ptr<Geocoding> GetGeocoding() const override {
    return mReader->GetGeocoding();
  }

  /**
   *  \brief Reads a pixel block defined by offsets, size, internal
   *  margin size, and kind. the offsets are relative to the upper-left corner
   *  pixel whose coordinates are (0, 0).
   *
   *  \param [in] colOffset starting column
   *  \param [in] rowOffset starting row
   *  \param [in] numCols horizontal width
   *  \param [in] numRows vertical height
   *  \return Returns pointer to raster object holding a pixel block
   */
  std::unique_ptr<IRaster> ReadRaster(const int colOffset, const int rowOffset,
    const int numCols, const int numRows, const RasterType rasterType) override {
    return mReader->ReadRaster(colOffset, rowOffset, numCols, numRows, rasterType);
  }

  /**
   *  \brief Reads a pixel block defined by the passed raster object, populates
   *  the associated buffer.
   *
   *  \param [in/out] raster the raster object to be populated with the pixel block.
   */
  void ReadRaster(IRaster* const raster) override {
    return mReader->ReadRaster(raster);
  }

private:
  std::unique_ptr<RasterReader> mReader;  // pointer to reader implementation

  // cached information
  GDALDataset* mDataset{ nullptr };  // non-owning pointer to GDALDataset object
  int mFileNumCols{};   // width of image file [unit: pixel]
  int mFileNumRows{};   // height of image file
  std::vector<int> mBands;    // list of bands to wrtie into
  GDALDataType mGdalDT{GDT_Byte};     // GDAL data type of image file
  int mWriteNumBands{};   // number of bands to write

  void ObtainDatasetInfo();
  std::tuple<RasterWindow, int64> ObtainWriteWindow(const IRaster* raster) const;
};
} // namespace earthxpert
