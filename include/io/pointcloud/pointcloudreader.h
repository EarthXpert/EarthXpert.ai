/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/pointcloud/ipointcloudreader.h"

#include "lasreader.hpp"
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
  PointcloudReader(const PointcloudReader &) = delete;
  PointcloudReader(PointcloudReader &&) = delete;
  ~PointcloudReader() = default;

  PointcloudReader &operator=(const PointcloudReader &) = delete;
  PointcloudReader &operator=(PointcloudReader &&) = delete;

  /**
   *  \return Returns fields group
   */
  FieldGroup GetFieldGroup() const noexcept override {
    return mFieldGroup;
  }

  /**
   *  \return Returns metadata tags
   */
  std::unique_ptr<VectorMetadata> GetMetadata() const override;

  /**
   *  \brief Set the margin size.
   *
   *  \param [in] marginX the horizontal margin size
   *  \param [in] marginY the vertical margin size
   */
  void SetMargins(float marginX, float marginY) override;

  /**
   *  \return Returns margin size.
   */
  std::pair<float, float> GetMargins() const noexcept override;

  /**
   *  \return Returns pointer to a coordinate system
   */
  std::unique_ptr<SpatialReferenceSystem> GetCoordinateSystem() const override;

  /**
   *  \return Returns total number of points of las file
   */
  int64 GetPointCount() const noexcept override {
    return mFilePointCount;
  }

  /**
   *  \return Return all points in the las file
   */
  std::unique_ptr<Pointcloud> ReadPointcloud() override;

  /**
   *  \brief Reads point range defined by the start index, number of points.
   *
   *  \param [in] start the index of start point
   *  \param [in] numPoints number of points to read
   *
   *  \return Return specified number of points
   */
  std::unique_ptr<Pointcloud> ReadPointcloud(size_t start, size_t numPoints) override;

  /**
   *  \brief Reads a point range defined by the passed cloud object, 
   *  starting from the current point, and populates the associated buffer.
   *
   *  \param [in/out] pointCloud the container object to be populated with points.
   *  \return Returns number of points actually read
   */
  size_t ReadPointcloud(Pointcloud* const pointCloud) override;

  /**
   *  \brief Reads point tile defined by tile, internal margin size.
   *
   *  \param [in] xmin x location of the lower-left corner of the tile [unit: meter]
   *  \param [in] ymin y location of the lower-left corner of the tile
   *  \param [in] width the tile width [unit: meter]
   *  \param [in] height the tile height
   *
   *  \return Return Pointcloud container object.
   */
  std::unique_ptr<Pointcloud> ReadPointcloud(double xmin, double ymin,
    double width, double height) override;

  /**
   *  \brief Reads point tile defined by tile, internal margin size.
   *
   *  \param [in] tile The horizontal extent of region to read
   *
   *  \return Return Pointcloud container object.
   */
  std::unique_ptr<Pointcloud> ReadPointcloud(const RegionTileD& tile) override;

  /**
   *  \return Returns LASreader object.
   */
  LASreader* GetLasReader() const noexcept {
    return mReader.get();
  }

  /**
   *  \return Returns LASreader object.
   */
  const LASheader* GetLasHeader() const noexcept {
    return mHeader;
  }

private:
  std::unique_ptr<LASreader> mReader{};
  FieldGroup mFieldGroup{};   // fields group to read

  // \remark margin of point cloud seems not quite meaningful. remove?
  float mMarginX{};
  float mMarginY{};

  mutable std::mutex mMutex;    // ranged reading
  mutable std::mutex mMutexB;   // boxed reading

  // cache
  const LASheader *mHeader{};   // file format infomation
  //const LASindex* mIndex{};     // quadtree indexing
  //const LASfilter* mFilter{};   // 
  //const LAStransform* mTransform{};

  FieldGroup mFileFieldGroup;
  size_t mFilePointCount{};  // total number of point records in file

  void ObtainLasInfo();
};
} // namespace earthxpert
