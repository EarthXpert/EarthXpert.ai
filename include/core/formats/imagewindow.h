/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/wxcore.h"
#include "cpl_conv.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"

#include <memory>

namespace watxtract {
/**
 *  Tyoe of margins
 */
enum MarginType {
  eZERO,       // zero padding  abc|000
  eREFLECTION, // reflection abc|cba
  eREPLICATION // replication abc|ccc
};

/**
 *  Class ImageWindow stores image data organized by band interleave.
 *  Inside a band, data is stored in row major order
 */
class WXCORE_EXPORTS ImageWindow {
public:
  ImageWindow(int32_t numCols, int32_t numRows, int32_t marginCols,
              int32_t marginRows, int32_t colOffset, int32_t rowOffset,
              int numBands, GDALDataType dataType,
              std::unique_ptr<uint8_t[]> &&imgData);
  ~ImageWindow() = default;

  /**
   *  \brief get data with in the channel
   *  \param [in] the channel data get back
   *
   *	\return cv::Mat containing channel data(pointer to the data. ImageWindow
   *   is still the owner of the data)
   */
  cv::Mat GetChannelData(int16_t numChannels) const;

  /**
   *  \brief get data for all channel with the arrangement
   *	\ RGBRGBRGBRGBRGBRGB
   *
   *	\return cv::Mat with a copy of the data
   */
  cv::Mat GetCVData();

  /**
   *  \brief get offset of the raster in imagewindow
   *  \param [out] colOffset the offset in number columns
   *  \param [out] rowOffset the offset in number rows
   */
  void GetOffset(int32_t &colOffset, int32_t &rowOffset) const;

  /**
   *  \brief get the size of raster store in imagewindow
   *  \param [out] numCols the number of columns in the raster
   *  \param [out] numRows the number of rows in the raster
   */
  void GetSize(int32_t &numCols, int32_t &numRows) const;

  /**
   *  \brief get the margin size of the image in the raster
   *  \param [out] marginCols the number margins in column
   *  \param [out] marginRows the number margins in row
   */
  void GetMargin(int32_t &marginCols, int32_t &marginRows) const;

  /**
   *  \brief get the number of bands in the raster image
   *
   *	\return the number of bands
   */
  int GetNumBands() const;

  /**
   *  \brief return the total number of pixel store in the ImageWindow
   *
   * \return the number of the pixels
   */
  size_t Size() const;

  /**
   *  \brief return the pointer to the start of the databuffer
   */
  const uint8_t *GetData() const;

  /**
   *  \brief set the data buffer in ImageWindow
   *	\param [in] rawData an unique pointer to the data buffer
   *	\param [in] dataType the data type of the buffer
   *	\param [in] numBands the number of bands of this data buffer
   */
  void SetData(std::unique_ptr<uint8_t[]> &&rawData, GDALDataType dataType,
               int numBands);

  /**
   *  \brief Data type stored in the file
   *
   *  \return Return Data type stored in the file
   */
  GDALDataType GetDataType() const;

private:
  std::unique_ptr<uint8_t[]> mData; // pointer to data
                                    // this stores band interleaved
  int32_t mNumCols{};               // number of columns in the window
  int32_t mNumRows{};               // number of rows in the data window
  int32_t mMarginCols{};            // x margin
  int32_t mMarginRows{};            // y margin
  int32_t mOffsetCols{};            // x offset
  int32_t mOffsetRows{};            // y offset
  int mNumBands{};                  // number of channels
  GDALDataType mDataType{};

  /**
   *  \brief Get the offset of the channel data within data buffer
   */
  size_t GetChannelOffset(int32_t chanNumber) const;
};
} // namespace watxtract
