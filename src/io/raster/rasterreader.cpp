/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/raster/rasterreader.h"
#include "3rdparty/gettext/exgettext.h"
#include "core/datasource/cvraster.h"
#include "core/datasource/gdalraster.h"
#include "core/datasource/rasterutils.h"
#include "core/exceptions/exexcept.h"
#include "core/spatialref/geocoding.h"
#include "core/utils/typehelper.h"

#include <numeric>

using namespace earthxpert;

/**
 *  \brief Construct reader object.
 *
 *  \param [in] imageFile full path of the image file
 *  \param [in] bands an array of 1-based band indices
 */
RasterReader::RasterReader(const std::string& imageFile, const std::vector<int>& bands)
  : mBands(bands) {
  auto dataset = GDALOpen(imageFile.c_str(), GA_ReadOnly);
  if (!dataset) {
    throw IOError(_("GDAL failed to open image file for read access."));
  }

  Initialize(static_cast<GDALDataset*>(dataset));
}

/**
 *  \brief Construct reader object.
 *
 *  \param [in] dataset an opened dataset for reading
 *  \param [in] bands an array of 1-based band indices
 */
RasterReader::RasterReader(GDALDataset* dataset, const std::vector<int>& bands)
  : mBands(bands) {
  assert(dataset);
  Initialize(dataset);
}

/**
 *  \brief Initialize reader object.
 *
 *  \param [in] dataset an opened dataset for reading
 */
void RasterReader::Initialize(GDALDataset* dataset) {
  mDataset.reset(dataset);
  ObtainDatasetInfo();
}

/**
 *  \return Returns metadata tags
 */
std::unique_ptr<RasterMetadata> RasterReader::GetMetadata() const {
  return {};
}

/**
 *  \return Returns pointer to the spatial coordinate system
 */
std::unique_ptr<SpatialReferenceSystem> RasterReader::GetCoordinateSystem() const {
  auto spatialRef = std::make_unique<SpatialReferenceSystem>(mDataset->GetProjectionRef());
  if (!spatialRef) {
    throw IOError(_("This image file has no infomation on coordinate system "));
  }
  return spatialRef;
}

/**
 *  \return Returns pointer to file geocoding
 */
std::unique_ptr<Geocoding> RasterReader::GetGeocoding() const {
  double geoTransform[6];
  if (mDataset->GetGeoTransform(geoTransform) == CE_None) {
    return std::make_unique<Geocoding>(geoTransform[0], geoTransform[3],
      geoTransform[1], geoTransform[5]);
  }
  return nullptr; // no file geocoding;
}

/**
 *  \brief Calculate the memory size in bytes to hold a raster
 *  given the width and height.
 *
 *  \param [in] numCols horizontal width
 *  \param [in] numRows vertical height
 *  \return Returns number of bytes to accomadate the image data of raster object
 */
int64_t RasterReader::GetRasterBufferNumBytes(const int numCols,
  const int numRows) const noexcept {
  const auto bufNumCols = numCols + mMarginCols * 2;
  const auto bufNumRows = numRows + mMarginRows * 2;
  return GetRasterBufferNumBytes(int64_t(bufNumCols) * int64_t(bufNumRows));
}

/**
 *  \param [in] numPix number of pixels
 *
 *  \return Return number of bytes to accomadate the image data of raster object.
 */
int64_t RasterReader::GetRasterBufferNumBytes(const int64_t numPix) const noexcept {
  return (int64_t(CV_ELEM_SIZE(mCvDT)) * numPix);
}

/**
 *  \brief Reads the entire image.
 *
 *  \param [in] rasterType type of data structure
 *  \return Returns pointer to raster object holding the entire image
 */
std::unique_ptr<IRaster> RasterReader::ReadRaster(RasterType rasterType) {
  return ReadRaster(0, 0, mFileNumCols, mFileNumRows, rasterType);
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
 *  \param [in] rasterType type of data structure
 *  \return Returns pointer to raster object holding a pixel block
 */
std::unique_ptr<IRaster> RasterReader::ReadRaster(const int colOffset, const int rowOffset,
  const int numCols, const int numRows, const RasterType rasterType) {
  std::unique_ptr<IRaster> raster;

  if (RasterType::eCVMAT == rasterType) {
    auto cvRaster = new CvRaster(numCols, numRows, mCvDT,
      colOffset, rowOffset, mMarginCols, mMarginRows, mMarginKind);
    raster.reset(cvRaster);
    ReadRaster(cvRaster);
  }
  else if (RasterType::eGDAL == rasterType) {
    auto gdalRaster = new GdalRaster(numCols, numRows, mGdalDT, mReadNumBands,
      colOffset, rowOffset, mMarginCols, mMarginRows, mMarginKind);
    raster.reset(gdalRaster);
    ReadRaster(gdalRaster);
  }
  else {
    throw std::invalid_argument(_("Unsupported rater type"));
  }

  return raster;
}

/**
 *  \brief Reads a pixel block defined by the passed raster object, populates
 *  the associated buffer.
 *
 *  \param [in/out] raster the raster object to be populated with the pixel block.
 */
void RasterReader::ReadRaster(IRaster* const raster) {
  const auto rasterType = raster->GetRasterType();

  if (RasterType::eCVMAT == rasterType) {
    ReadRaster(static_cast<CvRaster* const>(raster));
  }
  else if (RasterType::eGDAL == rasterType) {
    ReadRaster(static_cast<GdalRaster* const>(raster));
  }
  else {
    throw std::invalid_argument(_("Unsupported rater type"));
  }
}

/**
 *  \brief Reads a pixel block defined by the passed OpenCV raster object, populates
 *  the associated buffer.
 *
 *  \param [in/out] raster the raster object to be populated with the pixel block.
 */
void RasterReader::ReadRaster(CvRaster* const raster) {
  assert(mDataset);
  std::lock_guard<std::mutex> lock(mMutex);
  const int64_t rowNumBytes = raster->GetRowNumBytes();
  const auto [readWin, readPixelOff] = ObtainReadWindow(raster);
  const auto [readColOff, readRowOff, readNumCols, readNumRows] = readWin;
  const int64_t bpp = raster->GetBytesPerPixel();
  const int64_t readBytesOff = bpp * readPixelOff;
  auto readBuffer = raster->GetBuffer() + readBytesOff;

  if (1 == raster->GetNumBands()) {
    auto band = mDataset->GetRasterBand(mBands[0]);

    auto readRow{ readBuffer };
    for (int r = 0; r < readNumRows; ++r) {
      const auto err = band->RasterIO(GF_Read, readColOff, readRowOff + r,
        readNumCols, 1, readRow, readNumCols, 1, mGdalDT, 0, 0, nullptr);
      if (CE_None != err) {
        throw IOError(_("Failaure when reading by RasterIO"));
      }

      readRow += rowNumBytes;
    }
  }
  else {
    const int64_t bpb = raster->GetBytesPerBand();
    const int64_t bandRowNumBytes = bpb * readNumCols;
    const int64_t pixelRowNumBytes = bpp * readNumCols;
    std::unique_ptr<uchar[]> gdalBuffer(new uchar[pixelRowNumBytes]);

    auto readRow{ readBuffer };
    for (int r = 0; r < readNumRows; ++r) {
      const auto err = mDataset->RasterIO(GF_Read, readColOff, readRowOff + r,
        readNumCols, 1, gdalBuffer.get(), readNumCols, 1, mGdalDT,
        mReadNumBands, mBands.data(), 0, 0, 0, nullptr);
      if (CE_None != err) {
        throw IOError(_("Failaure when reading by RasterIO"));
      }

      // convert RGB of GDAL into BGR of CVMAT
      uchar* pixel{ readRow };
      uchar* bandRow{ gdalBuffer.get() + (pixelRowNumBytes - bandRowNumBytes) };
      for (int c = 0; c < readNumCols; ++c) {
        uchar* band{ bandRow };
        for (int b = mReadNumBands-1; b >= 0; --b) {
          memcpy(pixel, band, bpb);
          pixel += bpb;
          band -= bandRowNumBytes;
        }

        bandRow += bpb;
      }

      readRow += rowNumBytes;
    }
  }

  const auto fillDim = ObtainMarginFillDim(raster);
  FillMargins(raster, fillDim);
}

/**
 *  \brief Reads a pixel block defined by the passed GDAL raster object, populates
 *  the associated buffer.
 *
 *  \param [in/out] raster the raster object to be populated with the pixel block.
 */
void RasterReader::ReadRaster(GdalRaster* const raster) {
  assert(mDataset);
  std::lock_guard<std::mutex> lock(mMutex);
  const int64_t rowNumBytes = raster->GetRowNumBytes();
  const int64_t bandNumBytes = raster->GetBandNumBytes();
  const auto [readWin, readPixelOff] = ObtainReadWindow(raster);
  const auto [readColOff, readRowOff, readNumCols, readNumRows] = readWin;
  const int64_t readBytesOff = raster->GetBytesPerBand() * readPixelOff;

  auto readBuffer = raster->GetBuffer() + readBytesOff;

  for (auto b : mBands) {
    auto band = mDataset->GetRasterBand(b);

    auto readRow{ readBuffer };
    for (int r = 0; r < readNumRows; ++r) {
      const auto err = band->RasterIO(GF_Read, readColOff, readRowOff + r,
        readNumCols, 1, readRow, readNumCols, 1, mGdalDT, 0, 0, nullptr);
      if (CE_None != err) {
        throw IOError(_("Failaure when reading by RasterIO"));
      }

      readRow += rowNumBytes;
    }

    readBuffer += bandNumBytes;
  }

  const auto fillDim = ObtainMarginFillDim(raster);
  FillMargins(raster, fillDim);
}

/**
 *  \brief Fill margins, if needed, for OpenCV raster object.
 *
 *  \param [in/out] raster the raster object already populated with the pixel block.
 *  \param [in] fillDim tells the dimensions of the four sides.
 */
void RasterReader::FillMargins(CvRaster* const raster, MarginFillDim fillDim) {
  earthxpert::FillMargins(raster, fillDim);
}

/**
 *  \brief Fill margins, if needed, for GDAL raster object.
 *
 *  \param [in/out] raster the raster object already populated with the pixel block.
 *  \param [in] fillDim tells the dimensions of the four sides.
 */
void RasterReader::FillMargins(GdalRaster* const raster, MarginFillDim fillDim) {
  auto cvRasters = ConvertRaster(raster, std::vector<int>());
  for (auto& cvRaster : cvRasters) {
    FillMargins(cvRaster.get(), fillDim);
  }
}

/**
 *  \brief obtain dataset information.
 */
void RasterReader::ObtainDatasetInfo() {
  mFileNumBands = mDataset->GetRasterCount();
  if (1 > mFileNumBands) {
    throw IOError(_("The image file contains no image bands."));
  }
  if (mFileNumBands < mBands.size()) {
    throw std::invalid_argument(_("Requested bands are larger than the available bands"));
  }
  if (mBands.empty()) {
    mBands.resize(mFileNumBands);
    std::iota(mBands.begin(), mBands.end(), 1);
  }
  else {
    auto [itMin, itMax] = std::minmax_element(mBands.begin(), mBands.end());
    if (1 > * itMin || mFileNumBands < *itMax) {
      throw std::out_of_range(_("Requested bands are out of the range of available bands"));
    }
  }
  mReadNumBands = static_cast<int>(mBands.size());
  mFileNumCols = mDataset->GetRasterXSize();
  mFileNumRows = mDataset->GetRasterYSize();
  mNodataValues.resize(mReadNumBands);

  auto band = mDataset->GetRasterBand(mBands[0]);
  mGdalDT = band->GetRasterDataType();
  mNodataValues[0] = band->GetNoDataValue();

  // check if the data type is same for all read bands
  for (int b = 1; b < mReadNumBands; ++b) {
    band = mDataset->GetRasterBand(mBands[b]);
    if (mGdalDT != band->GetRasterDataType()) {
      throw std::logic_error(_("Image bands should be of same data type"));
    }
    mNodataValues[b] = band->GetNoDataValue();
  }

  mCvDT = ConvertPixelType(mGdalDT, mReadNumBands);
}

/**
 *  \brief Obtain information about the read window.
 *
 *  \param [in] raster the raster object.
 *  \return Returns the read window offset, size, and the pixel offset.
 */
std::tuple<RasterWindow, int64_t>
RasterReader::ObtainReadWindow(IRaster* const raster) const {
  const auto [bufColOff, bufRowOff] = raster->GetBufferOffsets();
  const auto [bufNumCols, bufNumRows] = raster->GetBufferSize();
  const auto bufColMax = bufColOff + bufNumCols;
  const auto bufRowMax = bufRowOff + bufNumRows;

  const auto readColOff = std::max(0, bufColOff);
  const auto readRowOff = std::max(0, bufRowOff);
  const auto readColMax = std::min(bufColMax, mFileNumCols);
  const auto readRowMax = std::min(bufRowMax, mFileNumRows);
  const auto readNumCols = readColMax - readColOff;
  const auto readNumRows = readRowMax - readRowOff;

  const int64_t readPixelOff = int64_t(bufNumCols) * (int64_t(readRowOff) - int64_t(bufRowOff)) +
    int64_t(readColOff) - int64_t(bufColOff);
  return { {readColOff, readRowOff, readNumCols, readNumRows}, readPixelOff };
}

/**
 *  \brief Obtain flag indicating which margin sides needing to be filled.
 *
 *  \param [in] raster the raster object already populated with the pixel block.
 *  \return Returns flag indicating margin sides to be filled.
 */
MarginFlag RasterReader::ObtainMarginFlag(IRaster* const raster) const {
  const auto [bufColOff, bufRowOff] = raster->GetBufferOffsets();
  const auto [bufNumCols, bufNumRows] = raster->GetBufferSize();
  const auto bufColMax = bufColOff + bufNumCols;
  const auto bufRowMax = bufRowOff + bufNumRows;

  MarginFlag flag;
  if (0 > bufRowOff) flag.set(Enum(MarginSide::eTOP));
  if (bufRowMax > mFileNumRows) flag.set(Enum(MarginSide::eBOTTOM));
  if (0 > bufColOff) flag.set(Enum(MarginSide::eLEFT));
  if (bufColMax > mFileNumCols) flag.set(Enum(MarginSide::eRIGHT));

  return flag;
}

/**
 *  \brief Obtain dimensions of four margin sides needing to be filled.
 *
 *  \param [in] raster the raster object already populated with the pixel block.
 *  \return Returns margin dimensions needing to be filled.
 */
MarginFillDim RasterReader::ObtainMarginFillDim(IRaster* const raster) const {
  const auto [bufColOff, bufRowOff] = raster->GetBufferOffsets();
  const auto [bufNumCols, bufNumRows] = raster->GetBufferSize();
  const auto bufColMax = bufColOff + bufNumCols;
  const auto bufRowMax = bufRowOff + bufNumRows;

  MarginFillDim fillDim{};
  if (0 > bufRowOff) fillDim[0] = -bufRowOff;
  if (bufRowMax > mFileNumRows) fillDim[1] = bufRowMax - mFileNumRows;
  if (0 > bufColOff) fillDim[2] = -bufColOff;
  if (bufColMax > mFileNumCols) fillDim[3] = bufColMax - mFileNumCols;

  return fillDim;
}
