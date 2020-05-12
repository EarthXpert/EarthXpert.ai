/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */

#pragma once

#include "core/formats/imagewindow.h"
#include "io/raster/iimagewriter.h"
#include "io/raster/imagereader.h"

namespace watxtract {
class WXIO_EXPORTS ImageWriter : public ImageReader, public IImageWriter {
public:
  /**
   *  \brief Open an existing file to write image data
   *
   *  \param [in] imageFile the file name to open
   *
   */
  ImageWriter(const std::string &imageFile);

  /**
   *  \brief Create a new file to writer image data
   *
   *  \param [in] imageFile the file name to create
   *  \param [in] numCols the number of columns in the new image file
   *  \param [in] numRows the number of rows in the new image file
   *  \param [in] numBands the number of bands in the new image file
   *  \param [in] dataType the datatype for the new file
   *
   */
  ImageWriter(const std::string &imageFile, int32_t numCols, int32_t numRows,
              int16_t numBands, GDALDataType dataType);
  ~ImageWriter() = default;

  /**

   *  \brief disable copy constractor and assignment operator
   */
  ImageWriter &operator=(const ImageWriter &) = delete;
  ImageWriter(const ImageWriter &) = delete;

  /**
   *  \brief set the coordinate system
   *
   *  \param [in] spatialref the coordinate system
   *
   *  \details to be replaced with OGRSpatialReference see
   *  \https://www.gdal.org/classOGRSpatialReference.html
   */
  void SetCoordinateSystem(const OGRSpatialReference &spatialref) override;

  /**
   *  \brief set the geocoding information to the open raster file
   *
   *  \param [in] geocoding geocoding information
   *
   */
  void SetGeocoding(const std::unique_ptr<Geocoding> &geocoding) override;

  /**
   *  \brief Write the raster block buffer to the file. The owenership for
   *
   *  \param [in] imageWindow the image data
   *
   */
  void WriteRaster(const std::unique_ptr<ImageWindow> &imageWindow) override;
};
} // namespace watxtract
