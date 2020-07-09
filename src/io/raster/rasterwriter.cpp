/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */

#include "io/raster/rasterwriter.h"
#include "3rdparty/gettext/exgettext.h"
#include "core/datasource/cvraster.h"
#include "core/datasource/gdalraster.h"
#include "core/datasource/rasterutils.h"
#include "core/exceptions/exexcept.h"

using namespace earthxpert;

/**
 *  \brief Construct writer object for existing image file.
 *
 *  \param [in] imageFile full path of the image file
 *  \param [in] bands an array of 1-based band indices
 */
RasterWriter::RasterWriter(const std::string& imageFile, const std::vector<int>& bands) {
  mDataset = static_cast<GDALDataset*>(GDALOpen(imageFile.c_str(), GA_Update));
  if (!mDataset) {
    throw IOError(_("GDAL failed to open image file for write access."));
  }

  Initialize(mDataset, bands);
}

/**
 *  \brief Construct writer object for newly created image file.
 *
 *  \param [in] imageFile path and name of image file to create
 *  \param [in] format format of image file
 *  \param [in] numCols number of columns in the image file
 *  \param [in] numRows number of rows in the image file
 *  \param [in] dataType datatype for the image
 *  \param [in] numBands number of bands in the image
 */
RasterWriter::RasterWriter(const std::string& imageFile, const std::string format,
  int numCols, int numRows, GDALDataType dataType, int numBands) {
  mDataset = thirdparty::CreateRasterDataset(imageFile, 
    format, numCols, numRows, dataType, numBands);

  Initialize(mDataset, std::vector<int>{});
}

/**
 *  \brief Initialize both reader and writer components for existing image file.
 *
 *  \param [in] dataset an opened dataset for writing
 *  \param [in] bands an array of 1-based band indices
 */
void RasterWriter::Initialize(GDALDataset* dataset, const std::vector<int>& bands) {
  mReader = std::make_unique<RasterReader>(dataset, bands);
  ObtainDatasetInfo();
}

/**
 *  \brief Inject metadata tags
 *
 *  \param [in] metaData the metadata tags
 */
void RasterWriter::SetMetadata(const RasterMetadata* metaData) {
  std::unique_lock<std::shared_mutex> lock(mMutex);
}

/**
 *  \brief Set metadata NODATA_VALUES.
 */
void RasterWriter::SetNodataValues(const double nodataValue) {
  std::vector<double> nodataValues(mWriteNumBands, nodataValue);
  SetNodataValues(nodataValues);
}

void RasterWriter::SetNodataValues(const std::vector<double>& nodataValues) {
  assert(mWriteNumBands == nodataValues.size());
  std::unique_lock<std::shared_mutex> lock(mMutex);
  auto& ndValues = mReader->GetNodataValues();
  ndValues = nodataValues;

  for (int b = 0; b < mWriteNumBands; ++b) {
    mDataset->GetRasterBand(mBands[b])->SetNoDataValue(ndValues[b]);
  }
}

/**
 *  \brief Inject the ground coordinate system where the image file operates
 *
 *  \param [in] spatialref the coordinate system
 */
void RasterWriter::SetCoordinateSystem(const SpatialReferenceSystem* srs) {
  std::unique_lock<std::shared_mutex> lock(mMutex);
  char* wkt{nullptr};
  srs->exportToWkt(&wkt);
  mDataset->SetProjection(wkt);
  CPLFree(wkt);
}

/**
 *  \brief Inject the geocoding information into the image file.
 *
 *  \param [in] geocoding geocoding information
 */
void RasterWriter::SetGeocoding(const Geocoding* geocoding) {
  if (!geocoding) return;
  std::unique_lock<std::shared_mutex> lock(mMutex);

  double geoTransform[6] = { geocoding->mOriginX, geocoding->mPixelSizeX, 0,
                             geocoding->mOriginY, 0, geocoding->mPixelSizeY };
  mDataset->SetGeoTransform(geoTransform);
}

/**
 *  \brief Write the pixel block contained in the raster object into image file.
 *
 *  \param [in] raster the raster object to be written into image file on disk
 */
void RasterWriter::WriteRaster(IRaster* const raster) {
  const auto rasterType = raster->GetRasterType();
  if (RasterType::eCVMAT == rasterType) {
    WriteRaster(static_cast<CvRaster* const>(raster));
  }
  else if (RasterType::eGDAL == rasterType) {
    WriteRaster(static_cast<GdalRaster* const>(raster));
  }
  else {
    throw std::invalid_argument(_("Unsupported rater type"));
  }
}

/**
 *  \brief Write the pixel block contained in the OpenCV raster object into image file.
 *
 *  \param [in] raster the OpenCV raster object to be written into image file on disk
 */
void RasterWriter::WriteRaster(const CvRaster* raster) {
  std::unique_lock<std::shared_mutex> lock(mMutex);
  const int64 rowNumBytes = raster->GetRowNumBytes();
  const auto [writeWin, writePixelOff] = ObtainWriteWindow(raster);
  const auto [writeColOff, writeRowOff, writeNumCols, writeNumRows] = writeWin;
  const int64 bpp = raster->GetBytesPerPixel();
  const int64 writeBytesOff = bpp * writePixelOff;
  auto writeBuffer = raster->GetBuffer() + writeBytesOff;

  if (1 == raster->GetNumBands()) {
    auto band = mDataset->GetRasterBand(mBands[0]);

    auto writeRow{ writeBuffer };
    for (int r = 0; r < writeNumRows; ++r) {
      const auto err = band->RasterIO(GF_Write, writeColOff, writeRowOff + r,
        writeNumCols, 1, writeRow, writeNumCols, 1, mGdalDT, 0, 0, nullptr);
      if (CE_None != err) {
        throw IOError(_("Failure when reading by RasterIO"));
      }

      writeRow += rowNumBytes;
    }
  }
  else {
    const int64 bpb = raster->GetBytesPerBand();
    const int64 bandRowNumBytes = bpb * writeNumCols;
    const int64 pixelRowNumBytes = bpp * writeNumCols;
    std::unique_ptr<uchar[]> gdalBuffer(new uchar[pixelRowNumBytes]);

    auto writeRow{ writeBuffer };
    for (int r = 0; r < writeNumRows; ++r) {
      // convert BGR of CVMAT into RGB of GDAL
      uchar* pixel{ writeRow };
      uchar* bandRow{ gdalBuffer.get() + (pixelRowNumBytes - bandRowNumBytes) };
      for (int c = 0; c < writeNumCols; ++c) {
        uchar* band{ bandRow };
        for (int b = mWriteNumBands - 1; b >= 0; --b) {
          memcpy(band, pixel, bpb);
          pixel += bpb;
          band -= bandRowNumBytes;
        }

        bandRow += bpb;
      }

      const auto err = mDataset->RasterIO(GF_Write, writeColOff, writeRowOff + r,
        writeNumCols, 1, gdalBuffer.get(), writeNumCols, 1, mGdalDT,
        mWriteNumBands, mBands.data(), 0, 0, 0, nullptr);
      if (CE_None != err) {
        throw IOError(_("Failure when writing by RasterIO"));
      }

      writeRow += rowNumBytes;
    }
  }
}

/**
 *  \brief Write the pixel block contained in the GDAL raster object into image file.
 *
 *  \param [in] raster the GDAL raster object to be written into image file on disk
 */
void RasterWriter::WriteRaster(const GdalRaster* raster) {
  std::unique_lock<std::shared_mutex> lock(mMutex);
  const auto gdalDT = raster->GetDataType();
  if (gdalDT != mGdalDT) {
    throw std::invalid_argument(_("The data type of raster object is not compatible with image file"));
  }

  const int64 rowNumBytes = raster->GetRowNumBytes();
  const int64 bandNumBytes = raster->GetBandNumBytes();
  const auto [writeWin, writePixelOff] = ObtainWriteWindow(raster);
  const auto [writeColOff, writeRowOff, writeNumCols, writeNumRows] = writeWin;
  const int64 writeBytesOff = raster->GetBytesPerBand() * writePixelOff;

  auto writeBuffer = raster->GetBuffer() + writeBytesOff;

  for (auto b : mBands) {
    auto band = mDataset->GetRasterBand(b);

    auto writeRow{ writeBuffer };
    for (int r = 0; r < writeNumRows; ++r) {
      const auto err = band->RasterIO(GF_Write, writeColOff, writeRowOff + r,
        writeNumCols, 1, writeRow, writeNumCols, 1, mGdalDT, 0, 0, nullptr);
      if (CE_None != err) {
        throw IOError(_("Failure when reading by RasterIO"));
      }

      writeRow += rowNumBytes;
    }

    writeBuffer += bandNumBytes;
  }
}

/**
 *  \brief Generate downsampled image overviews.
 */
void RasterWriter::BuildPyramids() {

}

/**
 *  \brief obtain dataset information.
 */
void RasterWriter::ObtainDatasetInfo() {
  assert(mDataset);
  assert(mReader);

  std::tie(mFileNumCols, mFileNumRows) = mReader->GetSize();
  mBands = mReader->GetReadBands();
  mGdalDT = mDataset->GetRasterBand(mBands[0])->GetRasterDataType();
  mWriteNumBands = static_cast<int>(mBands.size());
}

/**
 *  \brief Obtain information about the read window.
 *
 *  \param [in] raster the raster object.
 *  \return Returns the write window offset, size, and the pixel offset.
 */
std::tuple<RasterWindow, int64>
RasterWriter::ObtainWriteWindow(const IRaster* raster) const {
  const int numBands = raster->GetNumBands();
  if (numBands != mWriteNumBands) {
    throw std::invalid_argument(_("Number of bands is different"));
  }

  const auto [colOffset, rowOffset] = raster->GetOffsets();
  const auto [numCols, numRows] = raster->GetSize();
  if (0 > colOffset || mFileNumCols < colOffset+numCols ||
      0 > rowOffset || mFileNumRows < rowOffset+numRows) {
    throw std::out_of_range(_("ObtainWriteWindow: raster object context is outside of image file"));
  }

  const int64 writePixelOff = raster->GetImagePixelOffset();
  return { {colOffset, rowOffset, numCols, numRows}, writePixelOff };
}
