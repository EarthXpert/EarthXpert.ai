/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/pointcloud/ipointcloudwriter.h"
#include <laswriter.hpp>

#include <vector>

namespace watxtract {
class WXIO_EXPORTS PointCloudWriter : public IPointCloudWriter {
public:
  /**
   *  \brief Open the las file for write
   *
   *  \param [in] lasFile the file name to open
   *  \param [in] header the header information for las file
   */
  PointCloudWriter(const std::string &lasFile, PointsHeader const *const header);
  PointCloudWriter(const PointCloudWriter &) = delete;
  PointCloudWriter &operator=(const PointCloudWriter &) = delete;
  ~PointCloudWriter() = default;

  /**
   *  \brief Write the file into the opened file. the area and offset will be
   *	\stored in PointsWindow and the writer will handle the merge of the
   *  \points
   *
   *  \param [in] pointsWindow the pointswindow that containing the points data
   */
  void WritePoints(PointsWindow const *const pointsWindow) override;

private:
  LASheader mLasHeader{};
  std::unique_ptr<LASwriter> mLasWriter;

  /**
   *  \brief Copy data from watxtract::point to LASpoint
   */
  void CopyPoint(const Point &point, LASpoint &lasPoint);
};
} // namespace watxtract
