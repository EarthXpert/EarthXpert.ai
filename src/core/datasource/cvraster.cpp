/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/datasource/cvraster.h"

using namespace earthxpert;

/**
 *  \brief Construct raster object by allocating & owing a memory buffer.
 *
 *  \param [in] numCols image width [unit: pixels]
 *  \param [in] numRows image height
 *  \param [in] dataType data type of elements in a band
 *  \param [in] colOffset col offset of first pixel
 *  \param [in] rowOffset row offset of first pixel
 *  \param [in] marginCols number of columns on left or right side
 *  \param [in] marginRows number of rows on top or bottom side
 */
CvRaster::CvRaster(const int numCols, const int numRows, const int dataType, 
  const int colOffset, const int rowOffset, 
  const int marginCols, const int marginRows, const MarginKind marginKind)
  : mNumCols(numCols), mNumRows(numRows), mDataType(dataType),
  mColOffset(colOffset), mRowOffset(rowOffset),
  mMarginCols(marginCols), mMarginRows(marginRows), mMarginKind(marginKind) {
  ObtainMatInfo();
  mMat.create(mBufNumRows, mBufNumCols, mDataType);
}

/**
 *  \brief Construct raster object by passing an existing memory buffer.
 * The caller is resposible for the management of the passed buffer.
 *
 *  \param [in] numCols image width [unit: pixels]
 *  \param [in] numRows image height
 *  \param [in] numBands number of bands/channels
 *  \param [in] dataType data type of elements in a band
 *  \param [in] colOffset col offset of first pixel
 *  \param [in] rowOffset row offset of first pixel
 *  \param [in] marginCols number of columns on left or right side
 *  \param [in] marginRows number of rows on top or bottom side
 *  \param [in] buffer pointer to pre-allocted buffer
 */
CvRaster::CvRaster(const int numCols, const int numRows, 
  const int dataType, const int colOffset, const int rowOffset,
  const int marginCols, const int marginRows, 
  const MarginKind marginKind, uchar* buffer)
  : mNumCols(numCols), mNumRows(numRows), mDataType(dataType),
  mColOffset(colOffset), mRowOffset(rowOffset),
  mMarginCols(marginCols), mMarginRows(marginRows), mMarginKind(marginKind) {
  ObtainMatInfo();
  mMat = cv::Mat(mBufNumRows, mBufNumCols, mDataType, buffer);
}

/**
 *  \brief Construct raster object by passing an existing memory buffer.
 * The caller is resposible for the management of the passed buffer.
 *
 *  \param [in] numCols image width [unit: pixels]
 *  \param [in] numRows image height
 *  \param [in] numBands number of bands/channels
 *  \param [in] dataType data type of elements in a band
 *  \param [in] colOffset col offset of first pixel
 *  \param [in] rowOffset row offset of first pixel
 *  \param [in] buffer pointer to pre-allocted buffer
 */
CvRaster::CvRaster(const int numCols, const int numRows,
  const int dataType, const int colOffset, const int rowOffset, uchar* buffer)
  : CvRaster(numCols, numRows, dataType, colOffset, rowOffset, 0, 0, MarginKind::eCONSTANT, buffer) {
}

/**
 *  \brief Construct raster object by passing an existing memory buffer.
 * The caller is resposible for the management of the passed buffer.
 *
 *  \param [in] numCols image width [unit: pixels]
 *  \param [in] numRows image height
 *  \param [in] numBands number of bands/channels
 *  \param [in] dataType data type of elements in a band
 *  \param [in] buffer pointer to pre-allocted buffer
 */
CvRaster::CvRaster(const int numCols, const int numRows, 
  const int dataType, uchar* buffer)
  : CvRaster(numCols, numRows, dataType, 0, 0, 0, 0, MarginKind::eCONSTANT, buffer) {
}

/**
 * Equality operator
 *
 * \param [in] rhs Object to compare with
 * \return true if same object or equal, false otherwise.
 * \todo
 */
bool CvRaster::operator==(const CvRaster& rhs) const {
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
    //if (mMat != rhs.mMat) return false;
  }
  return true;
}

/**
 * Test inequality.
 *
 * \param [in] rhs Object to compare with
 * \return true if not equal.
 */
bool CvRaster::operator!=(const CvRaster& rhs) const {
  return !(operator==(rhs));
}

void CvRaster::ObtainMatInfo() {
  mNumBands = CV_MAT_CN(mDataType);
  mBpb = CV_ELEM_SIZE1(mDataType);
  mBpp = CV_ELEM_SIZE(mDataType);
  mBufColOff = mColOffset - mMarginCols;
  mBufRowOff = mRowOffset - mMarginRows;
  mBufNumCols = mNumCols + mMarginCols * 2;
  mBufNumRows = mNumRows + mMarginRows * 2;
  mImgPixelOff = int64_t(mBufNumCols) * int64_t(mMarginRows) + int64_t(mMarginCols);
  mRowNumBytes = int64_t(mBpp) * int64_t(mBufNumCols);
  mBufNumBytes = mRowNumBytes * int64_t(mBufNumRows);
}
