/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/pointcloud/ipointcloudreader.h"

#include "lasreader.hpp"
#include <iostream>
#include <vector>

namespace watxtract {
/**
 *  The class PointcloudReader provides format in-dependent functios to read from
 *  a point cloud file with LASlib as backend, and extract points in a Cloudpoint object
 *  having specified fields, such as XYZ, intensity, classification etc.
 *  Each PointcloudReader instance is connected and thus bound to specific fields.
 *  Therefore a new instance is needed to read from different groups of available fields.
 */
class WXIO_EXPORTS PointcloudReader final : public IPointcloudReader {
public:
  PointcloudReader(const std::string& lasFile);
  PointcloudReader(const std::string &lasFile, FieldGroup fieldGroup);
  PointcloudReader(const PointcloudReader &) = delete;
  PointcloudReader(PointcloudReader &&) = delete;
  ~PointcloudReader() = default;

  PointcloudReader &operator=(const PointcloudReader &) = delete;
  PointcloudReader &operator=(PointcloudReader &&) = delete;

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
   *  \return Return header information of the las file
   */
  PointcloudHeader* GetHeader() const override;

  /**
   *  \return Returns total number of points of las file
   */
  int64 GetPointCount() const noexcept override {
    return mFilePointCount;
  }

  /**
   *  \brief Reads point tile defined by tile, internal margin size.
   *
   *  \param [in] xOffset x location of the bottom-left corner of the tile [unit: meter]
   *  \param [in] yOffset y location of the bottom-left corner of the tile
   *  \param [in] width the tile width [unit: meter]
   *  \param [in] height the tile height
   *
   *  \return Return Pointcloud container object.
   */
  std::unique_ptr<Pointcloud> ReadPointcloud(double xOffset, double yOffset, 
    double width, double height) override;

  /**
   *  \brief Reads point range defined by the start index, number of points.
   *
   *  \param [in] offset the index of start point
   *  \param [in] pointCount number of points
   *
   *  \return Return specified number of points
   */
  std::unique_ptr<Pointcloud> ReadPointcloud(size_t offset, size_t pointCount) override;

  /**
   *  \return Return all points in the las file
   */
  std::unique_ptr<Pointcloud> ReadPointcloud() override;

  /**
   *  \brief Reads a point range defined by the passed cloud object, populates
   *  the associated buffer.
   *
   *  \param [in/out] cloud the container object to be populated with points.
   */
  void ReadPointcloud(Pointcloud* const cloud) override;

private:
  std::unique_ptr<LASreader> mLasReader{};
  FieldGroup mFieldGroup{};

  float mMarginX{};
  float mMarginY{};

  // cache
  const LASheader *mLasHeader{};
  int64 mFilePointCount{};  // total number of point records in file
  FieldGroup mFileFieldGroup;

  void ObtainLasInfo();
};
} // namespace watxtract
