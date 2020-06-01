/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "core/datasource/rastertypes.h"

#include "opencv2/core/hal/interface.h"
#include <memory>
#include <utility>

namespace earthxpert {
/**
 *  The interface IRaster defines a 2D pixel block, that provides
 *  information about pixel value, size, data type, position and margin.
 */
class EXCORE_EXPORTS IRaster {
public:
  virtual ~IRaster() {}

  /**
   *  The RasterType defines the raster data structure
   */
  enum class RasterType {
    eUNDEFINED = -1,
    eGDAL,        ///< Indicates GDAL band interleave
    eCVMAT,       ///< Indicates OpenCV pixel interleave
    NumRasterTypes
  };
  using rt = RasterType;

  virtual std::unique_ptr<IRaster> Clone() const = 0;

  /**
   *  \return Returns type of raster data
   */
  virtual RasterType GetRasterType() const noexcept = 0;

  /**
   *  Update a pixel value for the given pixel position.
   *
   *  \param [in] col horizontal column position
   *  \param [in] row vertical row position
   *  \param [in] pixel pointer to pixel
   */
  virtual void SetPixel(int col, int row, const uchar* pixel) = 0;

  /**
   *  Retrieve a pixel value for the given pixel position.
   *
   *  \param [in] col horizontal column position
   *  \param [in] row vertical row position
   *  \return Returns pointer to pixel
   */
  virtual uchar* GetPixel(int col, int row) const = 0;

  virtual uchar* operator()(int col, int row) const = 0;

  /**
   *  \return Returns image size.
   */
  virtual std::pair<int, int> GetSize() const = 0;

  /**
   *  \return Return number of bands
   */
  virtual int GetNumBands() const noexcept = 0;

  /**
   *  \return Returns col, row offsets of the first pixel of the image
   */
  virtual std::pair<int, int> GetOffsets() const noexcept = 0;

  /**
   *  \return Returns col, row margins
   */
  virtual std::pair<int, int> GetMarginSize() const noexcept = 0;

  /**
   *  \return Returns margin kind
   */
  virtual MarginKind GetMarginKind() const noexcept = 0;

  /**
   *  \return Returns a pointer to the array storing pixels values
   */
  virtual uchar* GetBuffer() const noexcept = 0;

  /**
   *  \return Returns number of bytes occupide by the entire raster buffer
   */
  virtual int64 GetBufferNumBytes() const noexcept = 0;

  /**
   *  \return Returns bytes per band
   */
  virtual int GetBytesPerBand() const noexcept = 0;

  /**
   *  \return Returns bytes per pixel
   */
  virtual int GetBytesPerPixel() const noexcept = 0;

  /**
   *  \return Returns col, row offsets of the first pixel of the buffer
   */
  virtual std::pair<int, int> GetBufferOffsets() const noexcept = 0;

  /**
   *  \return Returns buffer size.
   */
  virtual std::pair<int, int> GetBufferSize() const noexcept = 0;

  /**
   *  \return Returns offset of the first pixel belonging to the image in the buffer.
   */
  virtual int64 GetImagePixelOffset() const noexcept = 0;
};
} // namespace earthxpert
