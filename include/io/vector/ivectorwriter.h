/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/exio.h"
#include "3rdparty/gdal/exgdal.h"

namespace earthxpert {
class ILayer;
class VectorMetadata;

/**
 *  The interface IVectorWriter defines the behaviors for writing into
 *  a vector file, and serialize vector block in an ILayer object.
 */
class EXIO_EXPORTS IVectorWriter {
public:
  virtual ~IVectorWriter() {}

  /**
   *  \brief Inject metadata tags
   *
   *  \param [in] metaData the metadata tags
   */
  virtual void SetMetadata(const VectorMetadata* metaData) = 0;

  /**
   *  \brief Inject the ground coordinate system where the image file operates
   *
   *  \param [in] spatialref the coordinate system
   */
  virtual void SetCoordinateSystem(const SpatialReferenceSystem* srs) = 0;

  /**
   *  \brief Write the vector block contained in the layer object into vector file.
   *
   *  \param [in] layer the layer object to be written into vector file on disk
   */
  virtual void WriteLayer(ILayer* const layer) = 0;

  /**
   *  \brief Generate levels of detail overviews.
   */
  virtual void BuildLods() = 0;
};

using UPIVectorWriter = std::unique_ptr<IVectorWriter>;
using SPIVectorWriter = std::shared_ptr<IVectorWriter>;
} // namespace earthxpert
