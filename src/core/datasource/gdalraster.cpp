/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/datasource/gdalraster.h"

#include "ogrsf_frmts.h"

using namespace earthxpert;

/**
 *  \brief Construct raster object by allocating & owing a memory buffer.
 *
 *  \param [in] numCols image width [unit: pixels]
 *  \param [in] numRows image height
 *  \param [in] dataType data type of elements in a band
 *  \param [in] numBands number of bands/channels
 *  \param [in] colOffset col offset of first pixel
 *  \param [in] rowOffset row offset of first pixel
 *  \param [in] marginCols number of columns on left or right side
 *  \param [in] marginRows number of rows on top or bottom side
 */
GdalRaster::GdalRaster(const int numCols, const int numRows, const GDALDataType dataType, 
  const int numBands, const int colOffset, const int rowOffset,
  const int marginCols, const int marginRows, const MarginKind marginKind)
  : mNumCols(numCols), mNumRows(numRows), mDataType(dataType), mNumBands(numBands),
  mColOffset(colOffset), mRowOffset(rowOffset),
  mMarginCols(marginCols), mMarginRows(marginRows), mMarginKind(marginKind) {
  ObtainBufferInfo();
  mBuffer = std::shared_ptr<uchar[]>(new uchar[mBufNumBytes]);
}

/**
 *  \brief Construct raster object by passing an existing memory buffer.
 * The caller is resposible for the management of the passed buffer.
 *
 *  \param [in] numCols image width [unit: pixels]
 *  \param [in] numRows image height
 *  \param [in] dataType data type of elements in a band
 *  \param [in] numBands number of bands/channels
 *  \param [in] colOffset col offset of first pixel
 *  \param [in] rowOffset row offset of first pixel
 *  \param [in] marginCols number of columns on left or right side
 *  \param [in] marginRows number of rows on top or bottom side
 *  \param [in] buffer pointer to pre-allocted buffer
 */
GdalRaster::GdalRaster(const int numCols, const int numRows, const GDALDataType dataType, 
  const int numBands, const int colOffset, const int rowOffset,
  const int marginCols, const int marginRows, const MarginKind marginKind, uchar* buffer)
  : mNumCols(numCols), mNumRows(numRows), mDataType(dataType), mNumBands(numBands),
  mColOffset(colOffset), mRowOffset(rowOffset),
  mMarginCols(marginCols), mMarginRows(marginRows), mMarginKind(marginKind) {
  ObtainBufferInfo();
  mBuffer = std::shared_ptr<uchar[]>(buffer, [](uchar*) {});
}

/**
 *  \brief Construct raster object by passing an existing memory buffer.
 * The caller is resposible for the management of the passed buffer.
 *
 *  \param [in] numCols image width [unit: pixels]
 *  \param [in] numRows image height
 *  \param [in] dataType data type of elements in a band
 *  \param [in] numBands number of bands/channels
 *  \param [in] buffer pointer to pre-allocted buffer
 */
GdalRaster::GdalRaster(const int numCols, const int numRows, 
  const GDALDataType dataType, const int numBands, uchar* buffer)
  : GdalRaster(numCols, numRows, dataType, numBands, 
    0, 0, 0, 0, MarginKind::eCONSTANT, buffer) {
}

/**
 * Equality operator
 *
 * \param [in] rhs Object to compare with
 * \return true if same object or equal, false otherwise.
 */
bool GdalRaster::operator==(const GdalRaster& rhs) const {
  if (this != &rhs) {
    // TODO
    return false;
  }
  return true;
}

/**
 * Test inequality.
 *
 * \param [in] rhs Object to compare with
 * \return true if not equal.
 */
bool GdalRaster::operator!=(const GdalRaster& rhs) const {
  return !(operator==(rhs));
}

void GdalRaster::ObtainBufferInfo() {  
  mBpb = GDALGetDataTypeSizeBytes(mDataType);
  mBpp = mBpb * mNumBands;
  mBufColOff = mColOffset - mMarginCols;
  mBufRowOff = mRowOffset - mMarginRows;
  mBufNumCols = mNumCols + mMarginCols * 2;
  mBufNumRows = mNumRows + mMarginRows * 2;
  mImgPixelOff = int64(mBufNumCols) * int64(mMarginRows) + int64(mMarginCols);
  mRowNumBytes = int64(mBpb) * int64(mBufNumCols);
  mBandNumBytes = mRowNumBytes * int64(mBufNumRows);
  mBufNumBytes = mBandNumBytes * int64(mNumBands);
}
