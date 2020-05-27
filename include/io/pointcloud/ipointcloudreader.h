/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/exio.h"
#include "3rd_party/gdal/exgdal.h"
#include "core/datasource/metadata.h"
#include "core/datasource/pointcloud.h"
#include "core/datasource/pointcloudheader.h"

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
   *  \return Return header information of the las file
   */
  virtual PointcloudHeader* GetHeader() const = 0;

  /**
   *  \return Returns pointer to a coordinate system
   */
  virtual std::unique_ptr<SpatialReferenceSystem> GetCoordinateSystem() const = 0;

  /**
   *  \return Returns total number of points of las file
   */
  virtual int64 GetPointCount() const noexcept = 0;

  /**
   *  \brief Reads point tile defined by tile, internal margin size.
   *
   *  \param [in] xOffset x location of the bottom-left corner of the tile [unit: meter]
   *  \param [in] yOffset y location of the bottom-left corner of the tile
   *  \param [in] width the tile width [unit: meter]
   *  \param [in] height the tile height
   *
   *  \return Return Pointcloud container object.
   */
  virtual std::unique_ptr<Pointcloud> ReadPointcloud(double xOffset, double yOffset,
    double width, double height) = 0;

  /**
   *  \brief Reads point range defined by the start index, number of points.
   *
   *  \param [in] offset the index of start point
   *  \param [in] pointCount number of points
   *
   *  \return Return specified number of points
   */
  virtual std::unique_ptr<Pointcloud> ReadPointcloud(size_t offset, size_t pointCount) = 0;

  /**
   *  \return Return all points in the las file
   */
  virtual std::unique_ptr<Pointcloud> ReadPointcloud() = 0;

  /**
   *  \brief Reads a point range defined by the passed cloud object, populates
   *  the associated buffer.
   *
   *  \param [in/out] cloud the container object to be populated with points.
   */
  virtual void ReadPointcloud(Pointcloud* const cloud) = 0;
};
} // namespace earthxpert
