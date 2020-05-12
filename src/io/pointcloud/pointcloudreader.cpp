/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/pointcloud/pointcloudreader.h"
#include "core/exceptions/wxexcept.h"

#include <limits>

using namespace watxtract;

/**
 *  \brief constractor, open the las file for read
 *
 *  \param [in] lasFile file name
 */
PointcloudReader::PointcloudReader(const std::string& lasFile) 
  : PointcloudReader(lasFile, {}) {
}

/**
 *  \brief constractor, open the las file for read
 *
 *  \param [in] lasFile file name
 *  \param [in] fieldGroup field group to read
 */
PointcloudReader::PointcloudReader(const std::string& lasFile,
  const FieldGroup fieldGroup) : mFieldGroup(fieldGroup) {
  LASreadOpener lasReadOpener;
  mLasReader.reset(lasReadOpener.open(lasFile.c_str()));
  if (!mLasReader)
    throw IOError("Cannot open .las file for read.");

  ObtainLasInfo();
}

/**
 *  \brief Set the margin size.
 *
 *  \param [in] marginX the horizontal margin size
 *  \param [in] marginY the vertical margin size
 */
void PointcloudReader::SetMargins(const float marginX, const float marginY) {
  mMarginX = marginX;
  mMarginY = marginY;
}

/**
 *  \return Returns margin size.
 */
std::pair<float, float> PointcloudReader::GetMargins() const noexcept {
  return {mMarginX, mMarginY};
}

/**
 *  \return Return header information of the las file
 */
PointcloudHeader* PointcloudReader::GetHeader() const {
  return nullptr;
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
std::unique_ptr<Pointcloud> PointcloudReader::ReadPointcloud(
  const double xOffset, const double yOffset, const double width, const double height) {
  return nullptr;
}

/**
 *  \brief Reads point range defined by the start index, number of points.
 *
 *  \param [in] offset the index of start point
 *  \param [in] pointCount number of points
 *
 *  \return Return specified number of points
 */
std::unique_ptr<Pointcloud> PointcloudReader::ReadPointcloud(
  const size_t offset, const size_t pointCount) {
  if (offset+ pointCount > mFilePointCount) {
    throw std::out_of_range("ReadPointcloud");
  }

  auto pc = std::make_unique<Pointcloud>(pointCount, mFieldGroup);

  mLasReader->seek(offset);
  for (int64 i = 0; i < pointCount; ++i) {
    mLasReader->read_point();
  }

  return pc;
}

/**
 *  \return Return all points in the las file
 */
std::unique_ptr<Pointcloud> PointcloudReader::ReadPointcloud() {
  return ReadPointcloud(0, mFilePointCount);
}

/**
 *  \brief Reads a point range defined by the passed cloud object, populates
 *  the associated buffer.
 *
 *  \param [in/out] cloud the container object to be populated with points.
 */
void PointcloudReader::ReadPointcloud(Pointcloud* const cloud) {
}

/**
 *  \brief obtain dataset information.
 */
void PointcloudReader::ObtainLasInfo() {
  assert(mLasReader);
  mLasHeader = &mLasReader->header;

  mFilePointCount = mLasHeader->number_of_point_records;

  if (mFieldGroup.none()) {
    mFieldGroup = mFileFieldGroup;
  }
  else {

  }
}
