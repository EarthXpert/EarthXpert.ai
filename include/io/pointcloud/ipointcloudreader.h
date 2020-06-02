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
   *  \brief Set the margin size.
   *
   *  \param [in] marginX the horizontal margin size
   *  \param [in] marginY the vertical margin size
   */
  virtual void SetMargins(float marginX, float marginY) = 0;

  /**
   *  \return Returns margin size.
   */
  virtual std::pair<float, float> GetMargins() const noexcept = 0;

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
   *  \brief Reads a point range defined by the passed cloud object, populates
   *  the associated buffer.
   *
   *  \param [in/out] pointCloud the container object to be populated with points.
   *  \return Returns number of points actually read
   */
  virtual size_t ReadPointcloud(Pointcloud* const pointCloud) = 0;

  /**
   *  \brief Reads point tile defined by tile, internal margin size.
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
   *  \brief Reads point tile defined by tile, internal margin size.
   *
   *  \param [in] tile The horizontal extent of region to read
   *
   *  \return Return Pointcloud container object.
   */
  virtual std::unique_ptr<Pointcloud> ReadPointcloud(const RegionTileD& tile) = 0;
};
} // namespace earthxpert
