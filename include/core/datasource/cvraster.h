/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/datasource/iraster.h"

#include "opencv2/core.hpp"

namespace earthxpert {
/**
 *  The class Raster stores a 2D pixel block that is pixel interleaved. For color image,
 *  the pixel bands are arranged in BGR sequence. 
 *  The pixel block may consist of the image itself and possibly the extra margin/border.
 */
class WXCORE_EXPORTS CvRaster final : public IRaster {
public:
  CvRaster(int numCols, int numRows, int dataType,
    int colOffset, int rowOffset, int marginCols, int marginRows, MarginKind marginKind);
  CvRaster(int numCols, int numRows, int dataType);
  CvRaster(int numCols, int numRows, int dataType,
    int colOffset, int rowOffset, int marginCols, int marginRows, MarginKind marginKind, uchar* buffer);
  CvRaster(int numCols, int numRows, int dataType, uchar* buffer);
  CvRaster(const CvRaster& rhs) = default;
  CvRaster(CvRaster&& rhs) noexcept = default;  // TODO
  ~CvRaster() = default;

  CvRaster& operator=(const CvRaster& rhs) = default;
  CvRaster& operator=(CvRaster&& rhs) noexcept = default;

  bool operator==(const CvRaster& rhs) const;
  bool operator!=(const CvRaster& rhs) const;

  std::unique_ptr<IRaster> Clone() const override {
    return std::make_unique<CvRaster>(*this);
  }

  /**
   * \return Returns type of raster data
   */
  RasterType GetRasterType() const noexcept override {
    return RasterType::eCVMAT;
  }

  /**
   *  Update a pixel value for the given pixel position.
   *
   *  \param [in] col horizontal column position
   *  \param [in] row vertical row position
   *  \param [in] pixel pointer to pixel
   */
  void SetPixel(const int col, const int row, const uchar* pixel) {
    SetPixel<uchar>(col, row, pixel);
  }

  /**
   *  Retrieve a pixel address for the given pixel position.
   *
   *  \param [in] col horizontal column position
   *  \param [in] row vertical row position
   *  \return Returns pointer to pixel
   */
  uchar* GetPixel(const int col, const int row) const override {
    return mMat.ptr(mMarginRows + row, mMarginCols + col);
  }

  uchar* operator()(const int col, const int row) const override {
    return GetPixel(col, row);
  }

  template <class Ty>
  void SetPixel(const int col, const int row, const Ty* pixel) {
    mMat.at<Ty>(row, col) = *pixel;
  }

  /**
   *  Retrieve a pixel value for the given pixel position.
   *
   *  \param _Ty data type of pixel
   *
   *  \param [in] col horizontal column position
   *  \param [in] row vertical row position
   *  \return Return pointer to the pixel.
   */
  template <class Ty>
  Ty* GetPixel(const int col, const int row) const {
    return mMat.ptr<Ty>(mMarginRows + row, mMarginCols + col);
  }

  /**
   * \return Returns image size.
   */
  std::pair<int, int> GetSize() const override {
    return { mNumCols, mNumRows };
  }

  /**
   * \return Return data type
   */
  int GetDataType() const noexcept { return mDataType; }

  /**
   * \return Return number of bands
   */
  int GetNumBands() const noexcept override { return mNumBands; }

  /**
   * \return Returns offsets of the first pixel of the image
   */
  std::pair<int, int> GetOffsets() const noexcept override {
    return { mColOffset, mRowOffset };
  }

  /**
   *  \return Returns margin size.
   */
  std::pair<int, int> GetMarginSize() const noexcept override {
    return { mMarginCols, mMarginRows };
  }

  /**
   *  \return Returns the margin kind.
   */
  MarginKind GetMarginKind() const noexcept override {
    return mMarginKind;
  }

  /**
   *  \return Returns a pointer to the array storing the pixels values
   */
  uchar* GetBuffer() const noexcept override { return mMat.data; }

  /**
   *  \return Returns number of bytes occupide by the entire raster buffer
   */
  int64 GetBufferNumBytes() const noexcept override { return mBufNumBytes; }

  /**
   *  \return Returns a reference to the internal cv::Mat member
   */
  cv::Mat& GetCvMat() const noexcept { return mMat; }

  /**
   *  \return Returns bytes per band
   */
  int GetBytesPerBand() const noexcept override { return mBpb; }

  /**
   *  \return Returns bytes per pixel
   */
  int GetBytesPerPixel() const noexcept override { return mBpp; }

  /**
   * \return Returns col, row offsets of the first pixel of the buffer
   */
  std::pair<int, int> GetBufferOffsets() const noexcept override {
    return { mBufColOff, mBufRowOff };
  }

  /**
   * \return Returns buffer size.
   */
  std::pair<int, int> GetBufferSize() const noexcept override {
    return { mBufNumCols, mBufNumRows };
  }

  /**
   *  \return Returns offset of the first pixel belonging to the image in the buffer.
   */
  int64 GetImagePixelOffset() const noexcept override {
    return mImgPixelOff;
  }

  /**
   *  \return Returns number of bytes occupide by a row of pixels
   */
  int64 GetRowNumBytes() const noexcept { return mRowNumBytes; }

private:
  int mNumCols{};               // number of image columns 
  int mNumRows{};               // number of image rows 
  int mDataType{ CV_8UC1 };     // data type of each band
  int mNumBands{};              // number of bands/channels
  int mColOffset{};             // col offset of image
  int mRowOffset{};             // row offset of image
  int mMarginCols{};            // margin cols
  int mMarginRows{};            // margin rows
  MarginKind mMarginKind{ MarginKind::eCONSTANT };
  mutable cv::Mat mMat;         // 2D matrix reprents the pixel block, each element is an pixel 

  // cached temporaries for performance
  int mBpb{};             // bytes occupied by one band
  int mBpp{};             // bytes occupied by one pixel
  int mBufColOff{};       // col offset of buffer
  int mBufRowOff{};       // row offset of buffer
  int mBufNumCols{};      // buffer cols including image cols, both left and right margins
  int mBufNumRows{};      // buffer rows including image rows, both top and bottom margins
  int64 mImgPixelOff{};   // offset of the first pixel belonging to the image in the buffer
  int64 mRowNumBytes{};   // number of bytes occupide by a row of pixels
  int64 mBufNumBytes{};   // number of bytes of mBuffer

  void ObtainMatInfo();
};
} // namespace watxtract
