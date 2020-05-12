/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/wxio.h"
#include "core/formats/pointswindow.h"

namespace watxtract {
/**
 *  Point Cloud Reader interface. Write the points data into a new las file
 */
class WXIO_EXPORTS IPointCloudWriter {
public:
  virtual ~IPointCloudWriter() {}

  /**
   *  \brief Write the file into the opened file. the area and offset will be
   *	\stored in PointsWindow and the writer will handle the merge of the
   *  \points
   *
   *  \param [in] pointsWindow the pointswindow that containing the points data
   */
  virtual void WritePoints(PointsWindow const *const pointsWindow) = 0;
};
} // namespace watxtract
