/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/pointcloud/ipointcloudwriter.h"

#include "lasquantizer.hpp"
#include "laswriter.hpp"
#include <mutex>
#include <vector>

namespace earthxpert {
/**
 *  The class PointcloudWriter provides functios to write into a points file
 *  with LASlib as backend, and serialize points in a Pointcloud object
 *  having specific fields, such as XYZ, intensity, classification etc.
 *
 *  \todo coordinate transform
 *  \todo how to update an existing file?
 */
class EXIO_EXPORTS PointcloudWriter final : public IPointcloudWriter {
public:
  /**
   *  \brief Open/create the las file for write
   *
   *  \param [in] lasFile the file name to receive points
   */
  PointcloudWriter(const std::string& lasFile, const FieldGroup fieldGroup,
    const LASquantizer& quantizer, const std::string format = "LAS");
  PointcloudWriter(const std::string& lasFile, 
    const LASquantizer& quantizer, const std::string format = "LAS");
  PointcloudWriter(const PointcloudWriter&) = delete;
  PointcloudWriter(PointcloudWriter&&) = delete;
  ~PointcloudWriter() = default;

  PointcloudWriter& operator=(const PointcloudWriter&) = delete;
  PointcloudWriter& operator=(PointcloudWriter&&) = delete;

  /**
   *  \return Returns the backend LASreader object. PointcloudWriter
   *  exports partially the LASlib functions. The caller could exploit
   *  the full LASlib capabilities via the returned object pointer.
   */
  LASwriter* GetLasWriter() const noexcept {
    return mWriter.get();
  }

  /**
   *  \return Returns fields group
   */
  FieldGroup GetFieldGroup() const noexcept override {
    return mFieldGroup;
  }

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
   *  \return Returns number of points written
   */
  size_t WritePointcloud(Pointcloud const* const pointCloud) override;

  /**
   *  \brief Write an array of points into the opened file.
   *
   *  \param [in] pointCloud the Pointcloud object holding an array of points
   *  \param [in] start the index of start point
   *  \param [in] numPoints number of points to write
   *  \return Returns number of points written
   */
  size_t WritePointcloud(Pointcloud const* const pointCloud,
    size_t start, size_t numPoints) override;

private:
  std::unique_ptr<LASwriter> mWriter;
  FieldGroup mFieldGroup;   // fields group to write
  LASquantizer mQuantizer;  // scale & offset parameters used to normalize XYZ

  mutable std::mutex mMutex;

  // cache
  LASheader mHeader;    // file format infomation
  LASinventory* mInventory{};   // inventory statistics
  size_t mFilePointCount{};  // total number of point records in file

  void InitHeader();
  void ObtainLasInfo();
};
} // namespace earthxpert
