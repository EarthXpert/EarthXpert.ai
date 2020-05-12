/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */

#pragma once

#include "io/wxio.h"

#include <iostream>
#include <memory>

namespace watxtract {
class PointsHeader;
class PointsWindow;

/**
 *  Point Cloud Reader interface. Read the las file into PointsWindow containing
 *  point cloud data.
 */
class WXIO_EXPORTS IPointCloudReader {
public:
  virtual ~IPointCloudReader() = default;

  /**
   *  \brief get the header information for las file
   *
   *	\return pointer to PointsHeader
   */
  virtual std::unique_ptr<PointsHeader> GetHeader() const = 0;

  /**
   *  \brief Get the total number of points
   *
   *  \return the number of points
   *
   */
  virtual size_t GetNumberPoints() const = 0;

  /**
   *  \brief Get the boundary of points
   *
   *  \param [out] maxX maximum x axis value
   *  \param [out] minX minimum x axis value
   *  \param [out] maxY maximum y axis value
   *  \param [out] minY minimum x axis value
   *
   */
  virtual void GetBoundary(double &maxX, double &minX, double &maxY,
                           double &minY) const = 0;

  /**
   *  \brief Reads the points within the square tile
   *
   *  \param [in] x lower left x axis;
   *  \param [in] y lower left y axis;
   *  \param [in] length the length of the side of square tile
   *  \param [in] margin the length of the margin to read
   *
   *  \return a pointer to the points window
   */
  virtual std::unique_ptr<PointsWindow>
  ReadPoints(double x, double y, double length, double margin) = 0;

  /**
   *  \brief Reads all the points in the las file. 
   *
   *  \return a pointer to the points window
   */
  virtual std::unique_ptr<watxtract::PointsWindow> ReadPoints() = 0;
};
} // namespace watxtract
