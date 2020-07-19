/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/exio.h"
#include "core/datasource/pointcloud.h"
#include "3rdparty//gdal/exgdal.h"

namespace earthxpert {
class VectorMetadata;

/**
 *  The interface IPointcloudWriter specifies the behaviours for
 *  serielizing an array of points in a Pointcloud object into a disk file.
 */
class EXIO_EXPORTS IPointcloudWriter {
public:
  virtual ~IPointcloudWriter() {}

  /**
   *  \return Returns fields group
   */
  virtual FieldGroup GetFieldGroup() const noexcept = 0;

  /**
   *  \brief Inject metadata tags
   *
   *  \param [in] metaData the metadata tags
   */
  virtual void SetMetadata(const VectorMetadata* metaData) = 0;

  /**
   *  \brief Inject the ground coordinate system
   *
   *  \param [in] spatialref the coordinate system
   */
  virtual void SetCoordinateSystem(const SpatialReferenceSystem* srs) = 0;

  /**
   *  \brief Write an array of points into the opened file.
   *
   *  \param [in] pointCloud the Pointcloud object holding an array of points
   *  \return Returns number of points written
   */
  virtual size_t WritePointcloud(Pointcloud const *const pointCloud) = 0;

  /**
   *  \brief Write an array of points into the opened file.
   *
   *  \param [in] pointCloud the Pointcloud object holding an array of points
   *  \param [in] start the index of start point
   *  \param [in] numPoints number of points to write
   *  \return Returns number of points written
   */
  virtual size_t WritePointcloud(Pointcloud const* const pointCloud,
    size_t start, size_t numPoints) = 0;
};

using UPIPointcloudWriter = std::unique_ptr<IPointcloudWriter>;
using SPIPointcloudWriter = std::shared_ptr<IPointcloudWriter>;
} // namespace earthxpert
