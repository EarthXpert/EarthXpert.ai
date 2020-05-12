/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/pointcloud/pointcloudreader.h"
#include "core/exceptions/wxexcept.h"

#include <limits>

using watxtract::Point;
using watxtract::PointCloudReader;
using watxtract::PointsHeader;
using watxtract::PointsWindow;

/**
 *  \brief constractor, open the las file for read
 *
 *  \param [in] lasFile file name
 */
PointCloudReader::PointCloudReader(const std::string &lasFile) {
  LASreadOpener lasReadOpener;
  mLasReader.reset(lasReadOpener.open(lasFile.c_str()));
  if (!mLasReader)
    throw IOError("Cannot open .las file for read.");

  mLasHeader = &mLasReader->header;
}

/**
 *  \brief get the header information for las file
 *
 *	\return pointer to PointsHeader
 */
std::unique_ptr<PointsHeader> PointCloudReader::GetHeader() const {
  assert(mLasReader);
  return std::make_unique<PointsHeader>(
      mLasHeader->max_x, mLasHeader->min_x, mLasHeader->max_y, mLasHeader->min_y,
      mLasHeader->max_z, mLasHeader->min_z,
      mLasHeader->x_scale_factor, mLasHeader->y_scale_factor,
      mLasHeader->z_scale_factor, mLasHeader->x_offset,
      mLasHeader->y_offset, mLasHeader->z_offset);
}

/**
 *  \brief Get the total number of points in the las file
 *
 *  \return the number of points
 */
size_t PointCloudReader::GetNumberPoints() const {
  assert(mLasReader);
  return mLasHeader->number_of_point_records;
}

/**
 *  \brief Get the boundary of points
 *
 *  \param [out] maxX maximum x axis value
 *  \param [out] minX minimum x axis value
 *  \param [out] maxY maximum y axis value
 *  \param [out] minY minimum x axis value
 */
void PointCloudReader::GetBoundary(double &maxX, double &minX, double &maxY,
                                   double &minY) const {
  assert(mLasReader);
  maxX = mLasHeader->max_x;
  minX = mLasHeader->min_x;
  maxY = mLasHeader->max_y;
  minY = mLasHeader->min_y;
}

/**
 *  \brief Reads the points within the square tile
 *
 *  \param [in] x lower left x axis;
 *  \param [in] y lower left y axis;
 *  \param [in] length the length of the side of square tile
 *  \param [in] margin the length of the margin to read
 *  \return a pointer to the points window
 */
std::unique_ptr<PointsWindow>
PointCloudReader::ReadPoints(const double x, const double y,
                             const double length, const double margin) {
  assert(mLasReader);

  // calculate actual square tile with margin
  double minX = x - margin;
  double minY = y - margin;
  double readLength = length + 2 * margin;
  double maxZ = -std::numeric_limits<double>::max();
  double minZ = std::numeric_limits<double>::max();

  // set boundary for read
  mLasReader->inside_tile(minX, minY, readLength);
  mLasReader->inside_tile(minX, minY, readLength);

  std::vector<Point> points;

  mLasReader->seek(0);
  while (mLasReader->read_point()) {
    if (mLasReader->get_z() > maxZ)
      maxZ = mLasReader->get_z();
    if (mLasReader->get_z() < minZ)
      minZ = mLasReader->get_z();

    points.push_back(Point(mLasReader->get_x(), mLasReader->get_y(),
                           mLasReader->get_z(), mLasReader->point.intensity,
                           mLasReader->point.classification,
                           mLasReader->point.rgb[0], mLasReader->point.rgb[1],
                           mLasReader->point.rgb[2]));
  }
  return std::make_unique<PointsWindow>(x + length, x, y + length, y, maxZ,
                                        minZ, margin, std::move(points));
}

std::unique_ptr<watxtract::PointsWindow> PointCloudReader::ReadPoints() {
  assert(mLasReader);
  std::vector<Point> points;

  mLasReader->seek(0);
  while (mLasReader->read_point()) {
    points.push_back(Point(mLasReader->get_x(), mLasReader->get_y(),
      mLasReader->get_z(), mLasReader->point.intensity,
      mLasReader->point.classification,
      mLasReader->point.rgb[0], mLasReader->point.rgb[1],
      mLasReader->point.rgb[2]));
  }

  return std::make_unique<PointsWindow>(mLasHeader->max_x,
    mLasHeader->min_x, mLasHeader->max_y,
    mLasHeader->min_y, mLasHeader->max_z,
    mLasHeader->min_z, 0, std::move(points));
}
