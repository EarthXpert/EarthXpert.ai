/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/pointcloud/ipointcloudwriter.h"

#include "laswriter.hpp"
#include <vector>

namespace earthxpert {
/**
 *  The class PointcloudWriter provides functios to write into a points file
 *  with LASlib as backend, and serialize points in a Pointcloud object
 *  having specific fields, such as XYZ, intensity, classification etc.
 *
 *  \todo update an existing file : public IPointcloudReader
 */
class EXIO_EXPORTS PointcloudWriter final : public IPointcloudWriter {
public:
  /**
   *  \brief Open/create the las file for write
   *
   *  \param [in] lasFile the file name to receive points
   */
  PointcloudWriter(const std::string& lasFile);
  PointcloudWriter(const std::string& lasFile, const std::string format);
  PointcloudWriter(const PointcloudWriter&) = delete;
  PointcloudWriter(PointcloudWriter&&) = delete;
  ~PointcloudWriter() = default;

  PointcloudWriter& operator=(const PointcloudWriter&) = delete;
  PointcloudWriter& operator=(PointcloudWriter&&) = delete;

  /**
   *  \brief Inject metadata tags
   *
   *  \param [in] metaData the metadata tags
   */
  void SetMetadata(const VectorMetadata* metaData) override;

  /**
   *  \brief Inject the ground coordinate system where the image file operates
   *
   *  \param [in] spatialref the coordinate system
   */
  void SetCoordinateSystem(const SpatialReferenceSystem* srs) override;

  /**
   *  \brief Write an array of points into the opened file.
   *
   *  \param [in] pointCloud the Pointcloud object holding an array of points
   */
  void WritePointcloud(Pointcloud const* const pointCloud) override;

private:
  std::unique_ptr<LASwriter> mLasWriter;

  void ObtainLasInfo();
};
} // namespace earthxpert
