/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */

#pragma once

#include <iostream>

#include "io/raster/iimagereader.h"

namespace watxtract {
/**
 *  Image writer interface
 */
class WXIO_EXPORTS IImageWriter {
public:
  virtual ~IImageWriter() = default;

  /**
   *  \brief set the coordinate system
   *
   *  \param [in] spatialref the coordinate system
   *
   *  \details to be replaced with OGRSpatialReference see
   *  \https://www.gdal.org/classOGRSpatialReference.html
   */
  virtual void SetCoordinateSystem(const OGRSpatialReference &spatialref) = 0;

  /**
   *  \brief set the geocoding information to the open raster file
   *  \	The ownership of geo_info will be transfer to the write and
   *  \	ImageWriter will release the memory
   *
   *  \param [in] geocoding geocoding information
   *
   */
  virtual void SetGeocoding(const std::unique_ptr<Geocoding> &geocoding) = 0;

  /**
   *  \brief Write the raster block buffer to the file. The owenership for
   *	\	ImageWindow will be transfer to the writer and the writer will
   *  \ release the memory and the buffer allocated for the buffer
   *
   *  \param [in] imagewindow the image data
   *
   */
  virtual void WriteRaster(const std::unique_ptr<ImageWindow> &imageWindow) = 0;
};
} // namespace watxtract
