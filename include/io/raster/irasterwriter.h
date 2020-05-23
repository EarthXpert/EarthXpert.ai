/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/exio.h"
#include "3rd_party/gdal/exgdal.h"

namespace earthxpert {
class Geocoding;
class IRaster;
class Metadata;

/**
 *  The interface IRasterWriter defines the behaviors for writing into 
 *  an image file, and serialize pixle block in an IRaster object.
 */
class WXIO_EXPORTS IRasterWriter {
public:
  virtual ~IRasterWriter() {}

  /**
   *  \brief Inject metadata tags
   *
   *  \param [in] metaData the metadata tags
   */
  virtual void SetMetadata(const Metadata* metaData) = 0;

  /**
   *  \brief Inject the ground coordinate system where the image file operates
   *
   *  \param [in] spatialref the coordinate system
   */
  virtual void SetCoordinateSystem(const SpatialReferenceSystem* srs) = 0;

  /**
   *  \brief Inject the geocoding information into the image file.
   *
   *  \param [in] geocoding geocoding information
   */
  virtual void SetGeocoding(const Geocoding* geocoding) = 0;

  /**
   *  \brief Write the pixel block contained in the raster object into image file.
   *
   *  \param [in] raster the raster object to be written into image file on disk
   */
  virtual void WriteRaster(IRaster* const raster) = 0;

  /**
   *  \brief Generate downsampled image overviews.
   */
  virtual void BuildPyramids() = 0;
};
} // namespace watxtract
