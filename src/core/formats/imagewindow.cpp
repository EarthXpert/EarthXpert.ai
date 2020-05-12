/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/formats/imagewindow.h"
#include "core/datasource/utils.h"

using watxtract::ImageWindow;

/**
 *  \brief constructor for ImageWindow
 */
ImageWindow::ImageWindow(int32_t numCols, int32_t numRows, int32_t marginCols,
                         int32_t marginRows, int32_t offsetCols,
                         int32_t offsetRows, int numBands,
                         GDALDataType dataType,
                         std::unique_ptr<uint8_t[]> &&imgData)
    : mNumCols(numCols), mNumRows(numRows), mMarginCols(marginCols),
      mMarginRows(marginRows), mOffsetCols(offsetCols), mOffsetRows(offsetRows),
      mDataType(dataType), mNumBands(numBands) {
  mData = std::move(imgData);
}

size_t ImageWindow::GetChannelOffset(int32_t chanNumber) const {
  int32_t totalCols = mNumCols + 2 * mMarginCols;
  int32_t totalRows = mNumRows + 2 * mMarginRows;
  size_t pixelSize = GDALGetDataTypeSizeBytes(mDataType);
  return pixelSize * totalCols * totalRows * (chanNumber - 1);
}

/**
 *  \brief get data with in the channel
 *  \param [in] the channel data get back
 *
 *	\return cv::Mat containing channel data(pointer to the data. ImageWindow
 *   is still the owner of the data)
 */
cv::Mat ImageWindow::GetChannelData(int16_t chanNumber) const {
  if (chanNumber > mNumBands)
    throw std::runtime_error("ImageWindow ERROR: channel out of range");
  int32_t totalCol = mNumCols + 2 * mMarginCols;
  int32_t totalRow = mNumRows + 2 * mMarginRows;
  cv::Mat cv_data;
  return cv::Mat(totalRow, totalCol, ConvertPixelType(mDataType, 1),
                 mData.get() + GetChannelOffset(chanNumber));
}

/**
 *  \brief get data for all channel with the arrangement
 *	\ RGBRGBRGBRGBRGBRGB
 *
 *	\return cv::Mat with a copy of the data
 */
cv::Mat ImageWindow::GetCVData() {
  int32_t totalCol = mNumCols + 2 * mMarginCols;
  int32_t totalRow = mNumRows + 2 * mMarginRows;
  int64_t totalPixel = totalCol * totalRow;

  // calculate pixel size
  size_t pixelSize = GDALGetDataTypeSizeBytes(mDataType);

  // allocate buffer
  auto new_data_buffer = new uint8_t[pixelSize * totalCol * totalRow * mNumBands];

  // rearrange the buffer to fill the pixeldata to
  // RGBRGB
  // RGBRGB
  // RGBRGB
  // In the new buffer, the pixel offset is 'the total number of channels' *
  // 'current pixel number' + 'current band number' In the old buffer, the pixel
  // offset is 'the total number of pixels' * 'current band number' + 'current
  // pixel number'
  for (int i = 0; i < totalPixel; i++) {
    for (int bandNumber = 0; bandNumber < mNumBands; bandNumber++) {
      memcpy(new_data_buffer +
                 pixelSize * (mNumBands * i + bandNumber), // new buffer
             mData.get() +
                 pixelSize * (bandNumber * totalPixel + i), // old buffer
             pixelSize);
    }
  }
  return cv::Mat(totalRow, totalCol,
    ConvertPixelType(mDataType, mNumBands),
                 new_data_buffer);
}

/**
 *  \brief get offset of the raster in imagewindow
 *  \param [out] colOffset the offset in number columns
 *  \param [out] rowOffset the offset in number rows
 *
 *	\return
 */
void ImageWindow::GetOffset(int32_t &colOffset, int32_t &rowOffset) const {
  colOffset = mOffsetCols;
  rowOffset = mOffsetRows;
}

/**
 *  \brief get the size of raster store in imagewindow
 *  \param [out] numCols the number of columns in the raster
 *  \param [out] numRows the number of rows in the raster
 */
void ImageWindow::GetSize(int32_t &numCols, int32_t &numRows) const {
  numCols = mNumCols;
  numRows = mNumRows;
}

/**
 *  \brief get the margin size of the image in the raster
 *  \param [out] marginCols the number margins in column
 *  \param [out] marginRows the number margins in row
 */
void ImageWindow::GetMargin(int32_t &marginCols, int32_t &marginRows) const {
  marginCols = mMarginCols;
  marginRows = mMarginRows;
}

/**
 *  \brief get the number of bands in the raster image
 *
 *	\return the number of bands
 */
int ImageWindow::GetNumBands() const { return mNumBands; }

/**
 *  \brief return the total number of pixel store in the ImageWindow
 */
size_t ImageWindow::Size() const {
  return sizeof(uint8_t) * (mNumCols + 2 * mMarginCols) *
         (mNumRows + 2 * mMarginRows);
}

/**
 *  \return Return the pointer to the start of the data buffer
 */
const uint8_t *ImageWindow::GetData() const { return mData.get(); }

/**
 *  \brief set the data buffer in ImageWindow
 *	\param [in] imgData an unique pointer to the data buffer
 *	\param [in] dataType the data type of the buffer
 *	\param [in] numBands the number of bands of this data buffer
 */
void ImageWindow::SetData(std::unique_ptr<uint8_t[]> &&imgData,
                          const GDALDataType dataType, const int numBands) {
  mData = std::move(imgData);
  mNumBands = numBands;
  mDataType = dataType;
}

/**
 *  \return Return pixel data type
 */
GDALDataType ImageWindow::GetDataType() const { return mDataType; }
