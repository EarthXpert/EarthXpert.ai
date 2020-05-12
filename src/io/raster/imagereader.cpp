/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/raster/imagereader.h"
#include "core/exceptions/wxexcept.h"
#include "core/formats/imagewindow.h"
#include "core/spatialref/geocoding.h"

using watxtract::Geocoding;
using watxtract::ImageReader;
using watxtract::ImageWindow;
using watxtract::MarginType;

/**
 *  \brief Open the raster file to read
 *  \param [in] imageFile name of the file to open
 */
ImageReader::ImageReader(const std::string &imageFile) {
  mDataset.reset((GDALDataset *)GDALOpen(imageFile.c_str(), GA_ReadOnly));
  if (!mDataset) {
    throw IOError("GDAL failed to open image file.");
  }

  // get mDataType from band 1
  mDataType = mDataset->GetRasterBand(1)->GetRasterDataType();

  // check if all band have same mDataType
  int NumBands = mDataset->GetRasterCount();
  for (int i = 2; i <= NumBands; i++) {
    if (mDataType != mDataset->GetRasterBand(i)->GetRasterDataType()) {
      throw std::logic_error("Image bands should be of same data type");
    }
  }
}

/**
 *  \brief Create a ImageReader by an existing gdal dataset
 *
 *  \param [in] gdalDataSet the dataset open by gdal
 */
ImageReader::ImageReader(GDALDataset *dataset) {
  mDataset.reset(dataset);
  if (!mDataset)
    throw std::invalid_argument("Empty GDAL dataset");

  mDataType = mDataset->GetRasterBand(1)->GetRasterDataType();

  // check if all band have same mDataType
  int NumBands = mDataset->GetRasterCount();
  for (int i = 2; i <= NumBands; i++) {
    if (mDataType != mDataset->GetRasterBand(i)->GetRasterDataType()) {
      throw std::logic_error("Image bands should be of same data type");
    }
  }
}

/**
 *  \brief get the Coordinate system
 *
 *  \return Returns the coordinate system
 *
 *  \details to be replaced with OGRSpatialReference see
 *  \https://www.gdal.org/classOGRSpatialReference.html
 */
OGRSpatialReference ImageReader::GetCoordinateSystem() const {
  OGRSpatialReference spatialref;
  if (spatialref.importFromWkt(mDataset->GetProjectionRef())) {
    throw;
  }
  return spatialref;
}

/**
 *  \brief Set the margin value and type for read
 *  \param [in] marginCols margin value in X axis
 *  \param [in] marginRows margin value in Y axis
 *  \param [in] marginType the type of margin applying to read
 */
void ImageReader::SetMargin(int16_t marginCols, int16_t marginRows,
                            MarginType marginType) {
  mMarginCols = marginCols;
  mMarginRows = marginRows;
  mMarginType = marginType;
}

/**
 *  \brief Data type stored in the file
 *
 *  \return Return Data type stored in the file
 */
GDALDataType ImageReader::GetDataType() const { return this->mDataType; }

/**
 *  \brief Reads a image block defined by offset and size
 *
 *  \param [in] colOffset starting column (0,0 is upper left corner)
 *  \param [in] rowOffset starting row (0,0 is upper left corner)
 *  \param [in] numCols width  to read
 *  \param [in] numRows height to reade
 *  \return Returns pointer to image window
 *
 *  \details Function will throw, exception hierarchy to be defined
 */
std::unique_ptr<ImageWindow> ImageReader::ReadRaster(const int32_t colOffset,
                                                     const int32_t rowOffset,
                                                     const int32_t numCols,
                                                     const int32_t numRows) {
  int NumBands = this->GetNumBands();
  return std::make_unique<ImageWindow>(
      numCols, numRows, mMarginCols, mMarginRows, colOffset, rowOffset,
      NumBands, mDataType,
      Read(colOffset, rowOffset, numCols, numRows, mMarginCols, mMarginRows));
}

/**
 *  \brief Reads and populates a given image window
 *
 *  \param [in] imageWindow Pre-allocated image window whose content has to be
 *  populated with data
 */
void ImageReader::ReadRaster(std::unique_ptr<ImageWindow> &imageWindow) {
  int numberBands = mDataset->GetRasterCount();
  int32_t marginCols, marginRows, nColOffset, nRowOffset, nNumCols, nNumRows;
  imageWindow->GetMargin(marginCols, marginRows);
  imageWindow->GetOffset(nColOffset, nRowOffset);
  imageWindow->GetSize(nNumCols, nNumRows);
  imageWindow->SetData(Read(nColOffset, nRowOffset, nNumCols,
                                      nNumRows, marginCols, marginRows),
                       mDataType, numberBands);
}

/**
 *  \brief internal reads a image block defined by offset and size
 *
 *  \param [in] colOffset starting column (0,0 is upper left corner)
 *  \param [in] rowOffset starting row (0,0 is upper left corner)
 *  \param [in] numCols width  to read
 *  \param [in] numRows height to reade
 *  \param [in] marginCols margin value in X axis
 *  \param [in] marginRows margin value in Y axis
 *  \return Returns pointer to image window
 */
std::unique_ptr<uint8_t[]>
ImageReader::Read(const int32_t colOffset, const int32_t rowOffset,
                  const int32_t numCols, const int32_t numRows,
                  const int16_t marginCols, const int16_t marginRows) {
  int numBands = mDataset->GetRasterCount();

  // get the total size of the image
  int32_t imageNumCols = mDataset->GetRasterXSize();
  int32_t imageNumRows = mDataset->GetRasterYSize();

  if (colOffset < 0 || rowOffset < 0 || colOffset + numCols > imageNumCols ||
      rowOffset + numRows > imageNumRows) {
    throw std::out_of_range("");
  }

  // check margin
  if (marginCols > numCols || marginRows > numRows) {
    throw std::out_of_range("");
  }

  // calculate total size after applying margin
  int32_t resOffsetCols = colOffset - marginCols;
  int32_t resOffsetRows = rowOffset - marginRows;
  int32_t resNumCols = numCols + 2 * marginCols;
  int32_t resNumRows = numRows + 2 * marginRows;

  // calculate total the area to read
  int32_t readOffsetCols = std::max(colOffset - marginCols, (int32_t)0);
  int32_t readOffsetRows = std::max(rowOffset - marginRows, (int32_t)0);
  int32_t readNumCols =
      std::min(colOffset + numCols + marginCols, imageNumCols) - readOffsetCols;
  int32_t readNumRows =
      std::min(rowOffset + numRows + marginRows, imageNumRows) - readOffsetRows;

  // calculate pixel size
  size_t pixelSize = GDALGetDataTypeSizeBytes(mDataType);

  // allocate total Buffer to read
  std::unique_ptr<uint8_t[]> imageTotalBuffer(
      new uint8_t[pixelSize * resNumCols * resNumRows * numBands]);

  // allocate buffer for each band read, will auto get clean when out of scope
  std::unique_ptr<uint8_t[]> imageBufferPtr(
      new uint8_t[pixelSize * readNumCols * readNumRows]);

  // get the pointer to the buffer block to save runtime
  auto imageBuffer = imageBufferPtr.get();

  // read data for each band
  GDALRasterBand *rasterBand;
  for (int i = 1; i <= numBands; i++) {
    rasterBand = mDataset->GetRasterBand(i);

    CPLErr res = rasterBand->RasterIO(
        GF_Read, readOffsetCols, readOffsetRows, readNumCols, readNumRows,
        imageBuffer, readNumCols, readNumRows, mDataType, 0, 0);

    // map the image read with margin
    auto bandBuffer =
        imageTotalBuffer.get() + pixelSize * resNumCols * resNumRows * (i - 1);
    for (int32_t col = 0; col < resNumCols; col++) {
      for (int32_t row = 0; row < resNumRows; row++) {

        // calculate the coordinate related to read buffer
        int32_t readCol = col + resOffsetCols - readOffsetCols;
        int32_t readRow = row + resOffsetRows - readOffsetRows;
        if (mMarginType == eZERO) {
          if (readCol < 0 || readRow < 0 || readCol >= readNumCols ||
              readRow >= readNumRows) {
            *(bandBuffer + pixelSize * (row * resNumCols + col)) = 0;
          } else {
            std::memcpy(bandBuffer + pixelSize * (row * resNumCols + col),
                        imageBuffer +
                            pixelSize * (readRow * readNumCols + readCol),
                        pixelSize);
          }
          continue;
        }
        int32_t mapCol = (int32_t)0;
        int32_t mapRow = (int32_t)0;
        if (readCol <= 0) {
          if (mMarginType == eREPLICATION) {
            mapRow = 0;
          }
          if (mMarginType == eREFLECTION) {
            mapCol = -readCol;
          }
        } else if (readCol >= readNumCols) {
          if (mMarginType == eREPLICATION) {
            mapCol = readNumCols;
          }
          if (mMarginType == eREFLECTION) {
            mapCol = 2 * readNumCols - readCol;
          }
        } else {
          mapCol = readCol;
        }
        if (readRow <= 0) {
          if (mMarginType == eREPLICATION) {
            mapRow = 0;
          }
          if (mMarginType == eREFLECTION) {
            mapRow = -readRow;
          }
        } else if (readRow >= readNumRows) {
          if (mMarginType == eREPLICATION) {
            mapCol = readNumCols;
          }
          if (mMarginType == eREFLECTION) {
            mapCol = 2 * readNumCols - readCol;
          }
        } else {
          mapRow = readRow;
        }
        memcpy(bandBuffer + pixelSize * (row * resNumCols + col),
               imageBuffer + pixelSize * (mapRow * readNumCols + mapCol),
               pixelSize);
      }
    }
  }
  return imageTotalBuffer;
}

/**
 *  \brief get geocoding info from raster file
 *
 *  \return Returns pointer to file geocoding
 */
std::unique_ptr<Geocoding> ImageReader::GetGeocoding() const {
  double geoTransform[6];
  if (mDataset->GetGeoTransform(geoTransform) != CE_None) {
    throw;
  }
  return std::make_unique<Geocoding>(geoTransform[0], geoTransform[3],
                                     geoTransform[1], geoTransform[5]);
}

/**
 *  \brief get size of the raster image
 *
 *  \param [out] numCols number of columns
 *  \param [out] numRows number of rows
 *  \return Return image size
 */
void ImageReader::GetSize(int32_t &numCols, int32_t &numRows) const {
  numCols = mDataset->GetRasterXSize();
  numRows = mDataset->GetRasterYSize();
}
