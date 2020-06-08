/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/exio.h"
#include "3rd_party/gdal/exgdal.h"
#include "core/datasource/metadata.h"
#include "core/datasource/pointcloud.h"

#include <memory>

namespace earthxpert {
/**
 *  The interface PointcloudReader specifies the behaviours for
 *  extracting an array of points in a Pointcloud object.
 */
class EXIO_EXPORTS IPointcloudReader {
public:
  virtual ~IPointcloudReader() {}

  /**
   *  \return Returns fields group
   */
  virtual FieldGroup GetFieldGroup() const noexcept = 0;

  /**
   *  \return Returns metadata tags
   */
  virtual std::unique_ptr<VectorMetadata> GetMetadata() const = 0;

  /**
   *  \brief Set the buffer size.
   *
   *  \param [in] bufferX the horizontal buffer size
   *  \param [in] bufferY the vertical buffer size
   */
  virtual void SetBuffers(float bufferX, float bufferY) = 0;

  /**
   *  \return Returns buffer size.
   */
  virtual std::pair<float, float> GetBuffers() const noexcept = 0;

  /**
   *  \return Returns pointer to a coordinate system
   */
  virtual std::unique_ptr<SpatialReferenceSystem> GetCoordinateSystem() const = 0;

  /**
   *  \return Returns total number of points of las file
   */
  virtual int64 GetPointCount() const noexcept = 0;

  /**
   *  \return Return all points in the las file
   */
  virtual std::unique_ptr<Pointcloud> ReadPointcloud() = 0;

  /**
   *  \brief Reads point range defined by the start index, number of points.
   *
   *  \param [in] satrt the index of start point
   *  \param [in] numPoints number of points to read
   *
   *  \return Return specified number of points
   */
  virtual std::unique_ptr<Pointcloud> ReadPointcloud(size_t start, size_t numPoints) = 0;

  /**
   *  \brief Reads a point range defined by the passed cloud object,
   *  starting from the start index, and populates the associated buffer.
   *
   *  \param [in] start the index of start point
   *  \param [in/out] pointCloud the container object to be populated with points.
   *                  The requested fields must be available from the dataset.
   *  \return Returns number of points actually read
   */
  virtual size_t ReadPointcloud(const size_t start, Pointcloud* const pointCloud) = 0;

  /**
   *  \brief Reads point tile defined by tile, internal buffer size.
   *
   *  \param [in] xmin x location of the lower-left corner of the tile [unit: meter]
   *  \param [in] ymin y location of the lower-left corner of the tile
   *  \param [in] width the tile width [unit: meter]
   *  \param [in] height the tile height
   *
   *  \return Return Pointcloud container object.
   */
  virtual std::unique_ptr<Pointcloud> ReadPointcloud(double xmin, double ymin,
    double width, double height) = 0;

  /**
   *  \brief Reads point tile defined by tile, internal buffer size.
   *
   *  \param [in] tile The horizontal extent of region to read
   *
   *  \return Return Pointcloud container object.
   */
  virtual std::unique_ptr<Pointcloud> ReadPointcloud(const RegionTileD& tile) = 0;
};
} // namespace earthxpert
