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
 *  an image file with GDAL as backend, and deserialize pixle block in 
 *  an IRaster object having specific configurations, such as size, offset, 
 *  margin, bands, etc. 
 *
 *  Each RasterReader instance is connected and thus bound to specific bands.
 *  Therefore a new instance is needed to read from different subsets of 
 *  available bands.
 */
class EXIO_EXPORTS RasterReader final : public IRasterReader {
public:
  RasterReader(const std::string& imageFile, const std::vector<int>& bands = {});
  RasterReader(const fs::path& imageFile, const std::vector<int>& bands = {});
  RasterReader(GDALDataset* dataset, const std::vector<int>& bands = {});
  RasterReader(const RasterReader&) = delete;
  RasterReader(RasterReader&&) = delete;
  ~RasterReader() = default;

  RasterReader& operator=(const RasterReader&) = delete;
  RasterReader& operator=(RasterReader&&) = delete;

  /**
   *  \return Returns the backend GDALDataset object. RasterReader
   *  exports partially the GDAL functions. The caller could exploit
   *  the full GDAL capabilities via the returned object pointer.
   */
  GDALDataset* GetDataset() const noexcept { return mDataset.get(); }

  /**
   *  \return Returns name of file format.
   */
  std::string GetFileFormat() const { return mDataset->GetDriverName(); }

  /**
   *  \return Returns metadata tags
   */
  std::unique_ptr<RasterMetadata> GetMetadata() const override;

  /**
   *  \return Returns width and height of image file [unit: pixels]
   */
  std::pair<int, int> GetSize() const noexcept override {
    return { mFileNumCols, mFileNumRows };
  }

  /**
   *  \return Returns GDAL data type.
   */
  GDALDataType GetDataType() const noexcept { return mGdalDT; }

  /**
   *  \return Returns OpenCV data type.
   */
  int GetCvDataType() const noexcept { return mCvDT; }

  /**
   *  \return Return the 1-based band indices to read from
   */
  auto GetReadBands() const noexcept { return mBands; }

  /**
   *  \return Return the number of channels contained in the image file
   */
  int GetNumBands() const noexcept override { return mFileNumBands; }

  /**
   *  \return Returns metadata NODATA_VALUES.
   */
  std::vector<double>& GetNodataValues() noexcept override { return mNodataValues; }

  /**
   *  \brief Set the margin size and kind.
   *
   *  \param [in] marginCols the horizontal margin size
   *  \param [in] marginRows the vertical margin size
   *  \param [in] marginKind the margin kind.
   */
  void SetMargins(const int marginCols, const int marginRows,
    const MarginKind marginKind) override {
    mMarginCols = marginCols;
    mMarginRows = marginRows;
    mMarginKind = marginKind;
  }

  /**
   *  \return Returns margin size.
   */
  std::pair<int, int> GetMargins() const noexcept override {
    return {mMarginCols, mMarginRows};
  }

  /**
   *  \return Returns the margin kind.
   */
  MarginKind GetMarginKind() const noexcept override { return mMarginKind; }

  /**
   *  \brief Fetch the ground coordinate system where the image file operates.
   *
   *  \return Returns pointer to a coordinate system
   */
  UPSpatialRefSys GetCoordinateSystem() const override;

  /**
   *  \brief Fetch the geocoding information of the image file.
   */
  std::unique_ptr<Geocoding> GetGeocoding() const override;

  /**
   *  \brief Calculate the memory size in bytes to hold a raster
   *  given the width and height.
   *
   *  \param [in] numCols horizontal width
   *  \param [in] numRows vertical height
   *  \return Returns number of bytes to accomadate the image data of raster object
   */
  int64 GetRasterBufferNumBytes(int numCols, int numRows) const noexcept {
    const auto bufNumCols = numCols + mMarginCols * 2;
    const auto bufNumRows = numRows + mMarginRows * 2;
    return GetRasterBufferNumBytes(int64_t(bufNumCols) * int64_t(bufNumRows));
  }

  int64 GetRasterBufferNumBytes(int64 numPix) const noexcept {
    return (int64_t(CV_ELEM_SIZE(mCvDT)) * numPix);
  }

  /**
   *  \brief Reads the entire image.
   *
   *  \param [in] rasType type of data structure
   *  \return Returns pointer to raster object holding the entire image
   */
  std::unique_ptr<IRaster> ReadRaster(RasterType rasType) override {
    return ReadRaster(0, 0, mFileNumCols, mFileNumRows, rasType);
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
   *  \param [in] rasType type of data structure
   *  \return Returns pointer to raster object holding a pixel block
   */
  std::unique_ptr<IRaster> ReadRaster(int colOffset, int rowOffset,
    int numCols, int numRows, RasterType rasType) override;

  /**
   *  \brief Reads a pixel block defined by a window.
   *
   *  \param [in] win Raster window
   *  \param [in] rasType type of data structure
   *  \return Returns pointer to raster object holding a pixel block
   */
  std::unique_ptr<IRaster> ReadRaster(const RasterWindow& win,
    RasterType rasType) override {
    return ReadRaster(win.colOffset, win.rowOffset, 
      win.numCols, win.numRows, rasType);
  }

  /**
   *  \brief Reads a pixel block defined by the passed raster object, populates
   *  the associated buffer.
   *
   *  \param [in/out] raster the raster object to be populated with the pixel block.
   */
  void ReadRaster(IRaster* const raster) override;

  void ReadRaster(CvRaster* const raster);
  void ReadRaster(GdalRaster* const raster);

  /**
   *  \return Returns an IPyramidReader object for accessing downsampled image overviews.
   */
   //std::unique_ptr<IPyramidReader> GetPyramidReader();

private:
  void Initialize(GDALDataset* dataset);

  UPGDALDataset mDataset;  // pointer to GDALDataset object
  std::vector<int> mBands;  // list of bands to read from

  void ReadBand(GDALRasterBand* band, const RasterWindow& readWin, 
    uchar* const readBuffer, int64_t rowNumBytes);

  int mMarginCols{};
  int mMarginRows{};
  MarginKind mMarginKind{MarginKind::eCONSTANT};

  void FillMargins(CvRaster* const raster, MarginFillDim fillDim);
  void FillMargins(GdalRaster* const raster, MarginFillDim fillDim);

  mutable std::mutex mMutex;  // GDAL RasterIO seems thread unsafe

  // cached information & metadata
  int mFileNumBands{};  // total number of bands of image file
  int mReadNumBands{};  // number of bands to read from
  int mFileNumCols{};   // width of image file [unit: pixel]
  int mFileNumRows{};   // height of image file
  std::vector<double> mNodataValues;
  GDALDataType mGdalDT{};
  int mCvDT{};

  void ObtainDatasetInfo();
  std::tuple<RasterWindow, int64> ObtainReadWindow(IRaster* const raster) const;
  MarginFlag ObtainMarginFlag(IRaster* const raster) const;
  MarginFillDim ObtainMarginFillDim(IRaster* const raster) const;
};

using UPRasterReader = std::unique_ptr<RasterReader>;
using SPRasterReader = std::shared_ptr<RasterReader>;
} // namespace earthxpert
