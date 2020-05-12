/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/wxio.h"
#include "core/formats/imagewindow.h"

#include <iostream>
#include <utility>

namespace watxtract {
class CoordinateSystem;
class Geocoding;
class ImageWindow;

/**
 *  Raster reader interface. Read the raster file into ImageWindow containing
 * image data.
 */
class WXIO_EXPORTS IImageReader {
public:
  virtual ~IImageReader() {}

  /**
   *  \brief Set the margin value and type for read
   *  \param [in] marginCols margin value in X axis
   *  \param [in] marginRows margin value in Y axis
   *  \param [in] marginType the type of margin applying to read
   */
  virtual void SetMargin(int16_t marginCols, int16_t marginRows,
                         MarginType marginType) = 0;

  /**
   *  \return Returns number of bands
   *
   *  \details More details
   */
  virtual int GetNumBands() const = 0;

  /**
   *  \brief Data type stored in the file
   *
   *  \return Return Data type stored in the file
   *
   *  \details More details
   */
  virtual GDALDataType GetDataType() const = 0;

  /**
   *  \brief Returns file size
   *
   *  \param [out] numCols number of columns
   *  \param [out] numRows number of rows
   *
   *  \details More details
   */
  virtual void GetSize(int32_t &numCols, int32_t &numRows) const = 0;

  /**
   *  \return Returns the coordinate system
   *
   *  \details https://www.gdal.org/classOGRSpatialReference.html
   */
  virtual OGRSpatialReference GetCoordinateSystem() const = 0;

  /**
   *  \return Returns file geocoding
   */
  virtual std::unique_ptr<Geocoding> GetGeocoding() const = 0;

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
  virtual std::unique_ptr<ImageWindow> ReadRaster(const int32_t colOffset,
                                                  const int32_t rowOffset,
                                                  const int32_t numCols,
                                                  const int32_t numRows) = 0;

  /**
   *  \brief Reads and populates a given image window
   *
   *  \param [in] imageWindow Pre-allocated image window whose content has to be
   *  populated with data
   *
   *  \details More details
   */
  virtual void ReadRaster(std::unique_ptr<ImageWindow> &imageWindow) = 0;
};
} // namespace watxtract
