/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/pointcloud/pointcloudreader.h"
#include "io/pointcloud/pcutils.h"
#include "3rdparty/laslib/exlaslib.h"
#include "core/datasource/pointcloudutils.h"
#include "core/exceptions/exexcept.h"

#include <limits>
#include <omp.h>
#include <optional>

using namespace earthxpert;
using pdt = thirdparty::PointDataTraits;

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
    throw IOError(_("Cannot open las file for read."));

  ObtainLasInfo();
}

PointcloudReader::PointcloudReader(const fs::path& lasFile, 
  const FieldGroup fieldGroup) 
  : PointcloudReader(lasFile.string(), fieldGroup) {}

/**
 *  \return Returns metadata tags
 */
std::unique_ptr<VectorMetadata> PointcloudReader::GetMetadata() const {
  return {};
}

/**
 *  \return Returns pointer to the spatial coordinate system
 */
UPSpatialRefSys PointcloudReader::GetCoordinateSystem() const {
  return nullptr;
}

/**
 *  \brief Reads point range defined by the start index, number of points.
 *
 *  \param [in] start the index of start point
 *  \param [in] numPoints number of points to read
 *
 *  \return Return specified number of points
 */
UPPointcloud
PointcloudReader::ReadPointcloud(const size_t start, const size_t numPoints) {
  if (start + numPoints > mFilePointCount) 
    throw std::out_of_range(_("ReadPointcloud: requested too many points"));

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
size_t PointcloudReader::ReadPointcloud(const size_t start, 
  Pointcloud* const pointCloud) {
  std::lock_guard<std::mutex> lock(mMutex);
  mReader->seek(start);
  return ReadPointcloud(pointCloud);
}

/**
 *  \brief Reads point tile defined by tile, internal buffer size.
 *
 *  \param [in] tile The horizontal extent of region to read
 *
 *  \return Return Pointcloud container object.
 */
UPPointcloud
PointcloudReader::ReadPointcloud(const SpatialTileD& tile) {
  // fast: direct reading if covering the entire file
  if (tile >= mFileBBox2D) {
    return ReadPointcloud();
  }

  // slower: filtered reading   \todo add buffer
  auto filter = CreateLasFilter(tile);

  std::lock_guard<std::mutex> lock(mMutex);
  mReader->set_filter(filter.get());

  // 1st pass: obtain number of points
  mReader->seek(0);

  std::optional<int64_t> firstPoint;   // index of first inclusion point
  int64_t lastPoint{};                 // index of last inclusion point
  size_t numPoints{};   // actual #points
  while (TRUE == mReader->read_point()) {
    if (!firstPoint) {
      firstPoint = mReader->p_count;
    }
    lastPoint = mReader->p_count;
    ++numPoints;
  }
  if (0 == numPoints) return {};

  // 2nd pass: actually obtain point data
  mReader->seek(firstPoint.value());

  auto pointCloud = std::make_unique<Pointcloud>(numPoints, mFieldGroup);
  pointCloud->SetOrigin(Point3d(mHeader->x_offset,
    mHeader->y_offset, mHeader->z_offset));

  size_t i{};
  while (TRUE == mReader->read_point() && mReader->p_count <= lastPoint) {
    pointCloud->SetPoint_Unchecked(i, mReader->point, false);
    ++i;
  }

  assert(numPoints == i);
  mReader->set_filter(nullptr);

  return pointCloud;
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
  if (pointCloud->GetFieldGroup() > mFieldGroup) 
    throw std::out_of_range(_("Requested fields are out of the range of read fields"));
  if (0 == pointCloud->GetPointCapacity()) 
    throw std::invalid_argument(_("ReadPointcloud: pointCloud's buffer is empty"));

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
 *  \brief obtain dataset information.
 */
void PointcloudReader::ObtainLasInfo() {
  assert(mReader);
  mHeader = &(mReader->header);
  //mIndex = mReader->get_index();
  //mFilter = mReader->get_filter();
  //mTransform = mReader->get_transform();

  const int fmt = mHeader->point_data_format;
  if (!pdt::IsValidFormat(fmt)) 
    throw IOError(_("ObtainLasInfo"));
  assert(pdt::IsValidRecordLength(mHeader->point_data_record_length));

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
      throw std::out_of_range(_("Requested fields are out of the range of file fields"));
  }

  mFilePointCount = mReader->npoints;
  if (0 == mFilePointCount) 
    throw IOError(_("ObtainLasInfo: empty points file"));
  assert(mHeader->number_of_point_records == mFilePointCount);  // extended_number_of_point_records

  mFileBBox2D.SetMinXY(mHeader->min_x, mHeader->min_y);
  mFileBBox2D.SetSize(mHeader->max_x - mHeader->min_x, 
    mHeader->max_y - mHeader->min_y);
}
