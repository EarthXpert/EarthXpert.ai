/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/pointcloud/pointcloudwriter.h"
#include "core/exceptions/wxexcept.h"

using watxtract::Point;
using watxtract::PointCloudWriter;
using watxtract::PointsHeader;
using watxtract::PointsWindow;

/**
 *  \brief Open the las file for write
 *  \param [in] lasFile the file name to open
 *  \param [in] header the header information for las file
 */
PointCloudWriter::PointCloudWriter(
    const std::string &lasFile, PointsHeader const *const header) {
  mLasHeader.point_data_format = 2;
  mLasHeader.point_data_record_length = 26;
  mLasHeader.max_x = header->maxX;
  mLasHeader.min_x = header->minX;
  mLasHeader.max_y = header->maxY;
  mLasHeader.min_y = header->minY;
  mLasHeader.max_z = header->maxZ;
  mLasHeader.min_z = header->minZ;
  mLasHeader.x_scale_factor = header->scaleX;
  mLasHeader.y_scale_factor = header->scaleY;
  mLasHeader.z_scale_factor = header->scaleZ;
  mLasHeader.x_offset = header->offsetX;
  mLasHeader.y_offset = header->offsetY;
  mLasHeader.z_offset = header->offsetZ;

  LASwriteOpener lasWriteOpener;
  lasWriteOpener.set_file_name(lasFile.c_str());
  mLasWriter.reset(lasWriteOpener.open(&mLasHeader));
  if (!mLasWriter)
    throw IOError("Cannot open las file for write");
}

/**
 *  \brief Write the file into the opened file. the area and offset will be
 *	\stored in PointsWindow and the writer will handle the merge of the
 *  \points
 *
 *  \param [in] pointsWindow the pointswindow that containing the points data
 */
void PointCloudWriter::WritePoints(PointsWindow const *const pointsWindow) {
  double maxX, minX, maxY, minY, margin;
  pointsWindow->GetBlockInfo(maxX, minX, maxY, minY, margin);

  auto& points = pointsWindow->GetPointsData();

  LASquantizer quantizer;
  quantizer.x_scale_factor = mLasHeader.x_scale_factor;
  quantizer.y_scale_factor = mLasHeader.y_scale_factor;
  quantizer.z_scale_factor = mLasHeader.z_scale_factor;
  quantizer.x_offset = mLasHeader.x_offset;
  quantizer.y_offset = mLasHeader.y_offset;
  quantizer.z_offset = mLasHeader.z_offset;

  // set up las point
  LASpoint point;
  point.init(&mLasHeader, mLasHeader.point_data_format,
             mLasHeader.point_data_record_length, 0);
  point.quantizer = &quantizer;
  for (int i = 0; i < points.size(); i++) {
    CopyPoint(points[i], point);

    if (point.inside_tile(minX, minY, maxX, maxY)) {
      mLasWriter->write_point(&point);
      mLasWriter->update_inventory(&point);
    }
  }
  mLasHeader.number_of_point_records =
      mLasWriter->inventory.extended_number_of_point_records;
  mLasWriter->update_header(&mLasHeader, TRUE, TRUE);
}

/**
 *  \brief Copy data from watxtract::point to LASpoint
 *
 */
void PointCloudWriter::CopyPoint(const Point &point, LASpoint &lasPoint) {
  lasPoint.set_x(point.x);
  lasPoint.set_y(point.y);
  lasPoint.set_z(point.z);
  lasPoint.set_intensity(point.intensity);
  lasPoint.set_classification(point.classification);
  lasPoint.set_R(point.r);
  lasPoint.set_G(point.g);
  lasPoint.set_B(point.b);
}
