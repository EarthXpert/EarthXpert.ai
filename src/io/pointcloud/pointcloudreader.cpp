/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/pointcloud/pointcloudreader.h"
#include "3rd_party/laslib/exlaslib.h"
#include "core/datasource/pointcloudutils.h"
#include "core/exceptions/exexcept.h"

#include "boost/format.hpp"
#include "lasfilter.hpp"
#include <limits>
#include <omp.h>
#include <optional>
#include <sstream>

using namespace earthxpert;
using earthxpert::thirdparty::PointDataTraits;

/**
 *  \brief constractor, open the las file for read
 *
 *  \param [in] lasFile file name
 *  \param [in] fieldGroup field group to read
 */
PointcloudReader::PointcloudReader(const std::string& lasFile,
  const FieldGroup fieldGroup) : mFieldGroup(fieldGroup) {
  LASreadOpener readOpener;
  mReader.reset(readOpener.open(lasFile.c_str()));
  if (!mReader)
    throw IOError("Cannot open las file for read.");

  ObtainLasInfo();
}

/**
 *  \return Returns metadata tags
 */
std::unique_ptr<VectorMetadata> PointcloudReader::GetMetadata() const {
  return {};
}

/**
 *  \brief Set the margin size.
 *
 *  \param [in] marginX the horizontal margin size
 *  \param [in] marginY the vertical margin size
 *
 *  \todo pending
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
 *  \return Returns pointer to the spatial coordinate system
 */
std::unique_ptr<SpatialReferenceSystem> PointcloudReader::GetCoordinateSystem() const {
  return nullptr;
}

/**
 *  \return Return all points in the las file
 */
std::unique_ptr<Pointcloud> PointcloudReader::ReadPointcloud() {
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
std::unique_ptr<Pointcloud> 
PointcloudReader::ReadPointcloud(const size_t start, const size_t numPoints) {
  if (start + numPoints > mFilePointCount) {
    throw std::out_of_range("ReadPointcloud");
  }

  std::lock_guard<std::mutex> lock(mMutex);
  mReader->seek(start);

  auto pc = std::make_unique<Pointcloud>(numPoints, mFieldGroup);
  ReadPointcloud(pc.get());
  
  return pc;
}

/**
 *  \brief Reads a point range defined by the passed cloud object,
 *  starting from the start index, and populates the associated buffer.
 *
 *  \param [in] start the index of start point
 *  \param [in/out] pointCloud the container object to be populated with points.
 *                  The requested fields must be available from the dataset.
 *  \return Returns number of points actually read
 */
size_t PointcloudReader::ReadPointcloud(const size_t start, Pointcloud* const pointCloud) {
  std::lock_guard<std::mutex> lock(mMutex);
  mReader->seek(start);

  return ReadPointcloud(pointCloud);
}

/**
 *  \brief Reads a point range defined by the passed cloud object,
 *  starting from the current point, and populates the associated buffer.
 *
 *  \param [in/out] pointCloud the container object to be populated with points.
 *                  The requested fields must be available from the dataset.
 *  \return Returns number of points actually read
 *
 *  \remark thread unsafe: donot call directly from mulitple threads simutaneously
 */
size_t PointcloudReader::ReadPointcloud(Pointcloud* const pointCloud) {
  assert(pointCloud);
  if (pointCloud->GetFieldGroup() > mFieldGroup) {
    throw std::out_of_range("Requested fields are out of the range of read fields");
  }
  if (0 == pointCloud->GetPointCapacity()) {
    throw std::invalid_argument("ReadPointcloud: pointCloud's buffer is empty");
  }

  pointCloud->SetOrigin(Point3d(mHeader->x_offset, 
    mHeader->y_offset, mHeader->z_offset));

  const size_t numPoints = std::min<size_t>(pointCloud->GetPointCapacity(), // capacity
    mReader->npoints - mReader->p_count);                           // available

  for (size_t i = 0; i < numPoints; ++i) {
    mReader->read_point();
    pointCloud->SetPoint_Unchecked(i, mReader->point, false);
  }
  pointCloud->SetPointCount(numPoints);

  return numPoints;
}

/**
 *  \brief Reads point tile defined by tile.
 *
 *  \param [in] minX x location of the lower-left corner of the tile
 *  \param [in] minY y location of the lower-left corner of the tile
 *  \param [in] width the tile width [unit: meter]
 *  \param [in] height the tile height
 *
 *  \return Return Pointcloud container object.
 *
 *  \todo reduce memory usage
 */
std::unique_ptr<Pointcloud> PointcloudReader::ReadPointcloud(
  const double minX, const double minY, const double width, const double height) {
  return ReadPointcloud({ minX, minY,width, height });
}

/**
 *  \brief Reads point tile defined by tile, internal margin size.
 *
 *  \param [in] tile The horizontal extent of region to read
 *
 *  \return Return Pointcloud container object.
 */
std::unique_ptr<Pointcloud> 
PointcloudReader::ReadPointcloud(const RegionTileD& tile) {
  // faster: direct reading if covering the entire file
  if (tile.ImproperContain(mFileBBox2D)) {
    return ReadPointcloud();
  }

  // slower: filtered reading
  std::stringstream ss;
  ss << boost::format("-keep_xy %1% %2% %3% %4%") \
    % tile.minX % tile.minY % (tile.minX + tile.width) % (tile.minY + tile.height);
  std::string str{ ss.str() };

  auto filter = std::make_unique<LASfilter>();
  filter->parse(str.data());

  std::lock_guard<std::mutex> lock(mMutex);
  mReader->set_filter(filter.get());

  // 1st pass: obtain number of points
  mReader->seek(0);

  std::optional<size_t> firstPoint;   // index of first inclusion point
  size_t numPoints{};   // actual #points
  while (TRUE == mReader->read_point()) {
    if (!firstPoint) {
      firstPoint = numPoints;
    }
    ++numPoints;
  }
  if (0 == numPoints) return {};

  // 2nd pass: actually obtain point data
  mReader->seek(firstPoint.value());

  auto pointCloud = std::make_unique<Pointcloud>(numPoints, mFieldGroup);
  pointCloud->SetOrigin(Point3d(mHeader->x_offset,
    mHeader->y_offset, mHeader->z_offset));

  size_t i{};
  while (TRUE == mReader->read_point()) {
    pointCloud->SetPoint_Unchecked(i, mReader->point, false);
    ++i;
  }

  assert(numPoints == i);
  mReader->set_filter(nullptr);

  return pointCloud;
}

/**
 *  \brief obtain dataset information.
 */
void PointcloudReader::ObtainLasInfo() {
  assert(mReader);
  mHeader = &(mReader->header);
  //mIndex = mReader->get_index();
  //mFilter = mReader->get_filter();
  //mTransform = mReader->get_transform();

  const int fmt = mHeader->point_data_format;
  if (!PointDataTraits::IsValidFormat(fmt)) {
    throw IOError("ObtainLasInfo");
  }
  assert(PointDataTraits::IsValidRecordLength(mHeader->point_data_record_length));

  mFileFieldGroup = FormatToFieldGroup(fmt);

  // turn off fields not used till now: intensity, time, nir 
  mFileFieldGroup.reset(FieldGroup::kPOSINT);   // comment out if using intenisty
  mFileFieldGroup.reset(FieldGroup::kPOSTIM);   // comment out if using GpsTime
  mFileFieldGroup.reset(FieldGroup::kPOSNIR);

  if (mFieldGroup.none()) {
    mFieldGroup = mFileFieldGroup;
  }
  else {
    if (mFieldGroup > mFileFieldGroup)
      throw std::out_of_range("Requested fields are out of the range of file fields");
  }

  mFilePointCount = mReader->npoints;
  if (0 == mFilePointCount) {
    throw IOError("ObtainLasInfo: empty points file");
  }
  assert(mHeader->number_of_point_records == mFilePointCount);  // extended_number_of_point_records 

  mFileBBox2D.SetMinXY(mHeader->min_x, mHeader->min_y);
  mFileBBox2D.SetSize(mHeader->max_x - mHeader->min_x, mHeader->max_y - mHeader->min_y);
}
