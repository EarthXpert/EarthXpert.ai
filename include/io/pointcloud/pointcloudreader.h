/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/pointcloud/ipointcloudreader.h"
#include "core/utils/fsutils.h"
#include "core/utils/spatialtile.h"

#include <lasreader.hpp>
#include <iostream>
#include <mutex>
#include <vector>

namespace earthxpert {
/**
 *  The class PointcloudReader provides format in-dependent functios to read from
 *  a point cloud file with LASlib as backend, and extract points in a Cloudpoint object
 *  having specified fields, such as XYZ, intensity, classification etc.
 *  Each PointcloudReader instance is connected and thus bound to specific fields.
 *  Therefore a new instance is needed to read from different groups of available fields.
 *
 *  \todo filter, coordinate transform
 */
class EXIO_EXPORTS PointcloudReader final : public IPointcloudReader {
public:
  PointcloudReader(const std::string& lasFile, FieldGroup fieldGroup = {});
  PointcloudReader(const fs::path& lasFile, FieldGroup fieldGroup = {});
  PointcloudReader(const PointcloudReader &) = delete;
  PointcloudReader(PointcloudReader &&) = delete;
  ~PointcloudReader() = default;

  PointcloudReader &operator=(const PointcloudReader &) = delete;
  PointcloudReader &operator=(PointcloudReader &&) = delete;

  /**
   *  \return Returns the backend LASreader object. PointcloudReader
   *  exports partially the LASlib functions. The caller could exploit
   *  the full LASlib capabilities via the returned object pointer.
   */
  LASreader* GetLasReader() const noexcept { return mReader.get(); }

  /**
   *  \return Returns fields group
   */
  FieldGroup GetFieldGroup() const noexcept override { return mFieldGroup; }

  /**
   *  \return Returns metadata tags
   */
  std::unique_ptr<VectorMetadata> GetMetadata() const override;

  /**
   *  \brief Set the buffer size.
   *
   *  \param [in] bufferX the horizontal buffer size
   *  \param [in] bufferY the vertical buffer size
   */
  void SetBuffers(float bufferX, float bufferY) override {
    mBufferX = bufferX;
    mBufferY = bufferY;
  }

  /**
   *  \return Returns buffer size.
   */
  std::pair<float, float> GetBuffers() const noexcept override {
    return { mBufferX, mBufferY };
  }

  /**
   *  \return Returns pointer to a coordinate system
   */
  UPSpatialRefSys GetCoordinateSystem() const override;

  /**
   *  \return Returns total number of points of las file
   */
  int64 GetPointCount() const noexcept override { return mFilePointCount; }

  const SpatialTileD& GetBBox2D() const noexcept { return mFileBBox2D; }

  /**
   *  \return Return all points in the las file
   */
  UPPointcloud ReadPointcloud() override {
    return ReadPointcloud(0, mFilePointCount);
  }

  /**
   *  \brief Reads point range defined by the start index, number of points.
   *
   *  \param [in] start the index of start point
   *  \param [in] numPoints number of points to read
   *
   *  \return Return specified number of points
   */
  UPPointcloud ReadPointcloud(size_t start, size_t numPoints) override;

  /**
   *  \brief Reads a point range defined by the passed cloud object,
   *  starting from the start index, and populates the associated buffer.
   *
   *  \param [in] start the index of start point
   *  \param [in/out] pointCloud the container object to be populated with points.
   *                  The requested fields must be available from the dataset.
   *  \return Returns number of points actually read
   */
  size_t ReadPointcloud(size_t start, Pointcloud* const pointCloud) override;

  /**
   *  \brief Reads point tile defined by tile, internal buffer size.
   *
   *  \param [in] minX x location of the lower-left corner of the tile [unit: meter]
   *  \param [in] minY y location of the lower-left corner of the tile
   *  \param [in] width the tile width [unit: meter]
   *  \param [in] height the tile height
   *
   *  \return Return Pointcloud container object.
   */
  UPPointcloud ReadPointcloud(double minX, double minY,
    double width, double height) override {
    return ReadPointcloud({ minX, minY,width, height });
  }

  /**
   *  \brief Reads point tile defined by tile, internal buffer size.
   *
   *  \param [in] tile The horizontal extent of region to read
   *
   *  \return Return Pointcloud container object.
   */
  UPPointcloud ReadPointcloud(const SpatialTileD& tile) override;

  /**
   *  \return Returns LASreader object.
   */
  const LASheader* GetLasHeader() const noexcept { return mHeader; }

private:
  std::unique_ptr<LASreader> mReader{};
  FieldGroup mFieldGroup{};   // fields group to read

  // \todo
  float mBufferX{};
  float mBufferY{};

  mutable std::mutex mMutex;    // ranged reading

  /**
   *  \brief Reads a point range defined by the passed cloud object,
   *  starting from the current point, and populates the associated buffer.
   *
   *  \param [in/out] pointCloud the container object to be populated with points.
   *  \return Returns number of points actually read
   */
  size_t ReadPointcloud(Pointcloud* const pointCloud);

  // cache
  const LASheader *mHeader{};   // file format infomation
  //const LASindex* mIndex{};     // quadtree indexing
  //const LASfilter* mFilter{};   // 
  //const LAStransform* mTransform{};

  FieldGroup mFileFieldGroup;
  size_t mFilePointCount{};  // total number of point records in file
  SpatialTileD mFileBBox2D;   // 2D bounding box of all points in file

  void ObtainLasInfo();
};
} // namespace earthxpert
