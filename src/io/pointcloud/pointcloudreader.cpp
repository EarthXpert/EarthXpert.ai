/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/pointcloud/pointcloudreader.h"
#include "3rd_party/laslib/exlaslib.h"
#include "core/datasource/utils.h"
#include "core/exceptions/exexcept.h"

#include <limits>

using namespace earthxpert;
using earthxpert::thirdparty::PointDataTraits;

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
  mReader.reset(lasReadOpener.open(lasFile.c_str()));
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
 *  \param [in] offstartset the index of start point
 *  \param [in] numPoints number of points to read
 *
 *  \return Return specified number of points
 */
std::unique_ptr<Pointcloud> PointcloudReader::ReadPointcloud(
  const size_t start, const size_t numPoints) {
  if (start + numPoints > mFilePointCount) {
    throw std::out_of_range("ReadPointcloud");
  }

  std::lock_guard<std::mutex> lock(mMutex);
  mReader->seek(start);

  auto pc = std::make_unique<Pointcloud>(numPoints, mFieldGroup);
  const auto readNumPoints = ReadPointcloud(pc.get());
  if (readNumPoints != numPoints) {
    throw IOError("ReadPointcloud");
  }
  return pc;
}

/**
 *  \brief Reads a point range defined by the passed cloud object,
 *  starting from the current point, and populates the associated buffer.
 *
 *  \param [in/out] pointCloud the container object to be populated with points.
 *                  The requested fields must be available from the dataset.
 *  \return Returns number of points actually read
 *
 *  \remark thread unsafe: donot call from mulitple threads simutaneously
 */
size_t PointcloudReader::ReadPointcloud(Pointcloud* const pointCloud) {
  assert(pointCloud);
  if (pointCloud->GetFieldGroup() > mFieldGroup) {
    throw std::out_of_range("Requested fields are out of the range of read fields");
  }
  if (0 == pointCloud->GetPointCount()) {
    throw std::invalid_argument("ReadPointcloud: pointCloud is empty");
  }

  pointCloud->SetOrigin(Point3d(mHeader->x_offset, 
    mHeader->y_offset, mHeader->z_offset));

  const size_t readNumPoints = std::min<size_t>(pointCloud->GetPointCount(), // capacity
    mReader->npoints - mReader->p_count);                           // available
  for (size_t i = 0; i < readNumPoints; ++i) {
    // \todo sanity check 
    mReader->read_point();
    pointCloud->SetPoint_Unchecked(i, mReader->point, false);
  }

  return readNumPoints;
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
 *  \brief obtain dataset information.
 */
void PointcloudReader::ObtainLasInfo() {
  assert(mReader);
  mHeader = &(mReader->header);
  mIndex = mReader->get_index();
  mFilter = mReader->get_filter();
  mTransform = mReader->get_transform();

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
  assert(mHeader->number_of_point_records == mFilePointCount);  // extended_number_of_point_records 
}
