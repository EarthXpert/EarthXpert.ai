/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/pc/pointcloudwriter.h"
#include "core/exceptions/wxexcept.h"

using namespace watxtract;

/**
 *  \brief Open/create the las file for write
 *
 *  \param [in] lasFile the file name to receive points
 */
PointcloudWriter::PointcloudWriter(const std::string& lasFile) 
  : PointcloudWriter(lasFile, "LAS") {
}

/**
 *  \brief Open/create the las file for write
 *
 *  \param [in] lasFile the file name to receive points
 */
PointcloudWriter::PointcloudWriter(const std::string& lasFile, const std::string format) {
  LASwriteOpener lasWriteOpener;
  lasWriteOpener.set_file_name(lasFile.c_str());
  lasWriteOpener.set_format(format.c_str());

  mLasWriter.reset(lasWriteOpener.open(nullptr));
  if (!mLasWriter)
    throw IOError("Cannot open las file for write");

  ObtainLasInfo();
}

/**
 *  \brief Write an array of points into the opened file.
 *
 *  \param [in] pointCloud the Pointcloud object holding an array of points
 */
void PointcloudWriter::WritePointcloud(Pointcloud const* const pointCloud) {
}

/**
 *  \brief obtain dataset information.
 */
void PointcloudWriter::ObtainLasInfo() {
  assert(mLasWriter);
}
