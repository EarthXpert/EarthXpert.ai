/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/exio.h"
#include "core/datasource/pointcloud.h"

namespace earthxpert {
/**
 *  The interface IPointcloudWriter specifies the behaviours for
 *  serielizing an array of points in a Pointcloud object into a disk file.
 */
class EXIO_EXPORTS IPointcloudWriter {
public:
  virtual ~IPointcloudWriter() {}

  /**
   *  \brief Write an array of points into the opened file.
   *
   *  \param [in] pointCloud the Pointcloud object holding an array of points
   */
  virtual void WritePointcloud(Pointcloud const *const pointCloud) = 0;
};
} // namespace earthxpert
