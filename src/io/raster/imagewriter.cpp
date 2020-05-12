/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/raster/imagewriter.h"
#include "3rd_party/gdal/wxgdal.h"
#include "core/spatialref/geocoding.h"

#include <cstdlib>

using watxtract::Geocoding;
using watxtract::ImageWindow;
using watxtract::ImageWriter;

/**
 *  \brief Open an existing file to write image data
 *
 *  \param [in] imageFile the file name to open
 */
ImageWriter::ImageWriter(const std::string &imageFile)
    : ImageReader((GDALDataset *)GDALOpen(imageFile.c_str(), GA_Update)) {}

/**
 *  \brief Create a new file to writer image data
 *
 *  \param [in] imageFile path and name of image file to create
 *  \param [in] numCols number of columns in the new image 
 *  \param [in] numRows number of rows in the new image 
 *  \param [in] numBands number of bands in the new image 
 *  \param [in] dataType datatype for the new image
 */
ImageWriter::ImageWriter(const std::string &imageFile, int32_t numCols,
  int32_t numRows, int16_t numBands, GDALDataType datatype)
    : ImageReader(
      CreateRasterDataset(imageFile, "GTiff", numCols, numRows, datatype, numBands)) {}

/**
 *  \brief set the geocoding information to the open raster file
 *
 *  \param [in] geocoding geocoding information
 */
void ImageWriter::SetGeocoding(const std::unique_ptr<Geocoding> &geocoding) {
  double geoTransform[6] = { geocoding->mOriginX, geocoding->mPixelSizeX, 0,
                             geocoding->mOriginY, 0, geocoding->mPixelSizeY };
  mDataset->SetGeoTransform(geoTransform);
}

/**
 *  \brief set the coordinate system
 *
 *  \param [in] spatialref the coordinate system
 *
 *  \details https://www.gdal.org/classOGRSpatialReference.html
 */
void ImageWriter::SetCoordinateSystem(const OGRSpatialReference &spatialref) {
  auto spatialWKT = std::make_unique<char *>();
  spatialref.exportToWkt(spatialWKT.get());
  mDataset->SetProjection(*spatialWKT.get());
}

/**
 *  \brief Write the raster block buffer to the file. The owenership for
 *
 *  \param [in] imageWindow the image data
 */
void ImageWriter::WriteRaster(const std::unique_ptr<ImageWindow> &imagewindow) {
  // Get block information
  int NumBands = imagewindow->GetNumBands();
  int32_t marginCols, marginRows, colOffset, rowOffset, numCols, numRows;
  imagewindow->GetMargin(marginCols, marginRows);
  imagewindow->GetOffset(colOffset, rowOffset);
  imagewindow->GetSize(numCols, numRows);

  size_t pixelSize = GDALGetDataTypeSizeBytes(imagewindow->GetDataType());

  // create buffer to copy the buffer to write
  std::unique_ptr<uint8_t[]> bufferToWritePtr(
      new uint8_t[pixelSize * numCols * numRows]);

  // get the pointer to save runtime
  auto bufferToWrite = bufferToWritePtr.get();

  // get the databuffer from the ImageWindow
  auto databuffer = imagewindow->GetData();
  int32_t totalCol = numCols + 2 * marginCols;
  int32_t totalRow = numRows + 2 * marginRows;

  /**
   *  \ For each band, copy the buffer from the ImageWindow to the buffer to
   *    write and withdraw the margin aera from the ImageWindow The copy is
   *    done by row after row. In the loop it calculated the offset for each
   *    row and do the memcpy
   */
  GDALRasterBand *rasterBand;
  for (int band = 1; band <= NumBands; band++) {

    // calculate the offset of the source buffer with margin in this band
    auto bufferWithMargin =
        databuffer + pixelSize * totalCol * totalRow * (band - 1);

    int writeBufferRow = 0;  // row number in the buffertowrite
    int sourceBufferRow = 0; // row number in the sourcebuffer

    // for each row, do the memcpy for the buffer within the row
    for (; sourceBufferRow < totalRow; sourceBufferRow++) {
      if (!(sourceBufferRow - marginRows < 0 ||
            sourceBufferRow - marginRows >= numRows)) {
        memcpy(
            // offset of the row in buffertowrite
            bufferToWrite + pixelSize * numCols * writeBufferRow,
            // offset of row in sourcebuffer
            bufferWithMargin +
                pixelSize * (sourceBufferRow * totalCol + marginCols),
            // memory size of one row
            pixelSize * numCols);
        writeBufferRow++;
      }
    }
    rasterBand = mDataset->GetRasterBand(band);
    CPLErr res = rasterBand->RasterIO(GF_Write, colOffset, rowOffset, numCols,
                                      numRows, bufferToWrite, numCols, numRows,
                                      imagewindow->GetDataType(), 0, 0);
  }
}
