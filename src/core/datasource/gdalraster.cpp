/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/datasource/gdalraster.h"

#include <ogrsf_frmts.h>

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
    if (mNumCols != rhs.mNumCols || mNumRows != rhs.mNumRows) 
      return false;
    if (mDataType != rhs.mDataType || mNumBands != rhs.mNumBands) 
      return false;
    if (mColOffset != rhs.mColOffset || mRowOffset != rhs.mRowOffset) 
      return false;
    if (mMarginCols != rhs.mMarginCols || 
      mMarginRows != rhs.mMarginRows || mMarginKind != rhs.mMarginKind) 
      return false;
    // mBuffer
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
  mImgPixelOff = int64_t(mBufNumCols) * int64_t(mMarginRows) + int64_t(mMarginCols);
  mRowNumBytes = int64_t(mBpb) * int64_t(mBufNumCols);
  mBandNumBytes = mRowNumBytes * int64_t(mBufNumRows);
  mBufNumBytes = mBandNumBytes * int64_t(mNumBands);
}
