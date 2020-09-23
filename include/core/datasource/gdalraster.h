/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/datasource/iraster.h"

#include "gdal_priv.h"

namespace earthxpert {
/**
 *  The class Raster stores a 2D pixel block that is band interleaved.
 *  The pixel block may consist of the image itself and possibly the extra margin/border.
 */
class EXCORE_EXPORTS GdalRaster final : public IRaster {
public:
  GdalRaster(int numCols, int numRows, GDALDataType dataType, int numBands,
    int colOffset = 0, int rowOffset = 0, int marginCols = 0, int marginRows = 0,
    MarginKind marginKind = MarginKind::eCONSTANT);
  GdalRaster(int numCols, int numRows, GDALDataType dataType, int numBands,
    int colOffset, int rowOffset, int marginCols, int marginRows, MarginKind marginKind, uchar* buffer);
  GdalRaster(int numCols, int numRows, GDALDataType dataType, int numBands, uchar* buffer);
  GdalRaster(const GdalRaster& rhs) = default;
  GdalRaster(GdalRaster&& rhs) noexcept = default;
  ~GdalRaster() = default;

  GdalRaster& operator=(const GdalRaster& rhs) = default;
  GdalRaster& operator=(GdalRaster&& rhs) noexcept = default;

  bool operator==(const GdalRaster& rhs) const;
  bool operator!=(const GdalRaster& rhs) const;

  std::unique_ptr<IRaster> Clone() const override {
    return std::make_unique<GdalRaster>(*this);
  }

  /**
   *  \return Returns type of raster data
   */
  RasterType GetRasterType() const noexcept override {
    return RasterType::eGDAL;
  }

  template <typename GdalRasterTy>
  struct PixelProxy {
    static_assert(std::is_same_v<GdalRaster, std::remove_cv_t<GdalRasterTy>>, "Must be GdalRaster");
    using GdalRaster_Pointer = std::add_pointer_t<GdalRasterTy>;

    PixelProxy(GdalRaster_Pointer gdalRaster, const int col, const int row) : raster(gdalRaster), c(col), r(row) {}

    operator uchar*() const {
      return nullptr;
    }

    GdalRaster_Pointer raster{};
    int c;
    int r;
  };

  /**
   *  Update a pixel value for the given pixel position.
   *
   *  \param [in] col horizontal column position
   *  \param [in] row vertical row position
   *  \param [in] pixel pointer to pixel
   */
  void SetPixel(const int col, const int row, const uchar* pixel) {
    const int64 offset = int64(mBpp) * (mImgPixelOff + int64(mBufNumCols) * int64(row) + int64(col));
    uchar* buffer = mBuffer.get() + offset;
    memcpy(buffer, pixel, mBpb);

    for (int b = 1; b < mNumBands; ++b) {
      buffer += mBandNumBytes;
      pixel += mBpb;
      memcpy(buffer, pixel, mBpb);
    }
  }

  /**
   *  Retrieve a pixel address for the given pixel position.
   *
   *  \param [in] col horizontal column position
   *  \param [in] row vertical row position
   *  \return Return pointer to the pixel.
   */
  uchar* GetPixel(const int col, const int row) const override {
    if (1 == mNumBands) {
      const int64 offset = int64(mBpp) * (mImgPixelOff + int64(mBufNumCols) * int64(row) + int64(col));
      return mBuffer.get() + offset;
    }
    else {
      return PixelProxy<const GdalRaster>(this, col, row);
    }
  }

  uchar* operator()(const int col, const int row) const override {
    return GetPixel(col, row);
  }

  template <class Ty>
  void SetPixel(const int col, const int row, const Ty* pixel) {
    static_assert(std::is_arithmetic_v<Ty>, "Fundamental data type");
    SetPixel(col, row, static_cast<const uchar*>(pixel));
  }

  /**
   *  Retrieve a pixel value for the given pixel position.
   *
   *  \param Ty data type of image channel
   *
   *  \param [in] col horizontal column position
   *  \param [in] row vertical row position
   *  \return Return pointer to the pixel.
   */
  template <class Ty>
  Ty* GetPixel(const int col, const int row) const {
    static_assert(std::is_arithmetic_v<Ty>, "Fundamental data type");
    return reinterpret_cast<Ty*>(GetPixel(col, row));
  }

  /**
   *  \return Returns image size.
   */
  std::pair<int, int> GetSize() const override {
    return { mNumCols, mNumRows };
  }

  /**
   *  \return Return data type
   */
  GDALDataType GetDataType() const noexcept { return mDataType; }

  /**
   *  \return Return number of bands
   */
  int GetNumBands() const noexcept override { return mNumBands; }

  /**
   *  \return Returns offsets of the first pixel of the image
   */
  std::pair<int, int> GetOffsets() const noexcept override {
    return { mColOffset, mRowOffset };
  }

  RasterWindow GetRasWin() const noexcept override {
    return { mColOffset, mRowOffset, mNumCols, mNumRows };
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
  uchar* GetBuffer() const noexcept override { return mBuffer.get(); }

  /**
   *  \return Returns number of bytes occupide by the entire raster buffer
   */
  int64 GetBufferNumBytes() const noexcept override { return mBufNumBytes; }

  /**
   *  \return Returns bytes per band
   */
  int GetBytesPerBand() const noexcept override { return mBpb; }

  /**
   *  \return Returns bytes per pixel
   */
  int GetBytesPerPixel() const noexcept override { return mBpp; }

  /**
   *  \return Returns col, row offsets of the first pixel of the buffer
   */
  std::pair<int, int> GetBufferOffsets() const noexcept override {
    return { mBufColOff, mBufRowOff };
  }

  /**
   *  \return Returns buffer size.
   */
  std::pair<int, int> GetBufferSize() const noexcept override {
    return { mBufNumCols, mBufNumRows };
  }

  RasterWindow GetBufWin() const noexcept override {
    return { mBufColOff, mBufRowOff, mBufNumCols, mBufNumRows };
  }

  /**
   *  \return Returns offset of the first pixel belonging to the image in the buffer.
   */
  int64 GetImagePixelOffset() const noexcept override {
    return mImgPixelOff;
  }

  /**
   *  \return Returns number of bytes occupide by a row of a band
   */
  int64 GetRowNumBytes() const noexcept { return mRowNumBytes; }

  /**
   *  \return Returns number of bytes occupied by one band
   */
  int64 GetBandNumBytes() const noexcept { return mBandNumBytes; }

private:
  //RasterWindow mRasWin;
  int mNumCols{};               // number of image columns
  int mNumRows{};               // number of image rows
  GDALDataType mDataType{ GDT_Unknown };     // data type of each band
  int mNumBands{};              // number of bands/channels
  int mColOffset{};             // col offset of image
  int mRowOffset{};             // row offset of image
  int mMarginCols{};            // margin cols
  int mMarginRows{};            // margin rows
  MarginKind mMarginKind{ MarginKind::eCONSTANT };
  std::shared_ptr<uchar[]> mBuffer; // pointer to data buffer, that is the pixel block including both image and margin

  // cached temporaries for performance
  int mBpb{};             // bytes occupied by one band
  int mBpp{};             // bytes occupied by one pixel
  //RasterWindow mBufWin;
  int mBufColOff{};       // col offset of buffer
  int mBufRowOff{};       // row offset of buffer
  int mBufNumCols{};      // buffer cols including image cols, both left and right margins
  int mBufNumRows{};      // buffer rows including image rows, both top and bottom margins
  int64 mImgPixelOff{};   // offset of the first pixel belonging to the image in the buffer
  int64 mRowNumBytes{};   // number of bytes occupide by a row of a band
  int64 mBandNumBytes{};  // number of bytes occupied by one band
  int64 mBufNumBytes{};   // number of bytes of mBuffer

  void ObtainBufferInfo();
};

using UPGdalRaster = std::unique_ptr<GdalRaster>;
using SPGdalRaster = std::shared_ptr<GdalRaster>;
} // namespace earthxpert
