/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/formats/pointswindow.h"
#include "io/pointcloud/ipointcloudreader.h"
#include "io/wxio.h"
#include <lasreader.hpp>

#include <iostream>
#include <vector>

namespace watxtract {
class WXIO_EXPORTS PointCloudReader : public IPointCloudReader {
public:
  /**
   *  \brief constractor, open the las file for read
   *
   *  \param [in] lasFile file name
   */
  PointCloudReader(const std::string &lasFile);
  PointCloudReader(const PointCloudReader &) = delete;
  PointCloudReader(PointCloudReader &&) = delete;
  ~PointCloudReader() = default;

  PointCloudReader &operator=(const PointCloudReader &) = delete;
  PointCloudReader &operator=(PointCloudReader &&) = delete;

  /**
   *  \brief get the header information for las file
   *
   *	\return pointer to PointsHeader
   */
  std::unique_ptr<PointsHeader> GetHeader() const override;

  /**
   *  \brief Get the total number of points in the las file
   *
   *  \return the number of points
   */
  size_t GetNumberPoints() const override;

  /**
   *  \brief Get the boundary of points
   *
   *  \param [out] maxX maximum x axis value
   *  \param [out] minX minimum x axis value
   *  \param [out] maxY maximum y axis value
   *  \param [out] minY minimum x axis value
   */
  void GetBoundary(double &maxX, double &minX, double &maxY,
                   double &minY) const override;

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
  std::unique_ptr<watxtract::PointsWindow>
  ReadPoints(const double x, const double y, const double length,
             const double margin) override;

  /**
   *  \brief Reads all the points in the las file. Note that memory 
   *   space might be not enough
   *
   *  \return a pointer to the points window
   */
  std::unique_ptr<watxtract::PointsWindow> ReadPoints() override;

private:
  std::unique_ptr<LASreader> mLasReader{};

  // cache
  const LASheader *mLasHeader{};
};
} // namespace watxtract
