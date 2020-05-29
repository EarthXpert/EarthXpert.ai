/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/pointcloud/pointcloudreader.h"
#include "3rd_party/laslib/exlaslib.h"
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
  mLasReader.reset(lasReadOpener.open(lasFile.c_str()));
  if (!mLasReader)
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
 *  \return Return header information of the las file
 */
PointcloudHeader* PointcloudReader::GetHeader() const {
  return nullptr;
}

/**
 *  \return Returns pointer to the spatial coordinate system
 */
std::unique_ptr<SpatialReferenceSystem> PointcloudReader::GetCoordinateSystem() const {
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
 *  \param [in] offset the index of start point to read
 *  \param [in] pointCount number of points
 *
 *  \return Return specified number of points
 */
std::unique_ptr<Pointcloud> PointcloudReader::ReadPointcloud(
  const size_t offset, const size_t pointCount) {
  if (offset + pointCount > mFilePointCount) {
    throw std::out_of_range("ReadPointcloud");
  }

  std::lock_guard<std::mutex> lock(mMutex);
  mLasReader->seek(offset);

  auto pc = std::make_unique<Pointcloud>(pointCount, mFieldGroup);
  const auto readNumPoints = ReadPointcloud(pc.get());
  if (readNumPoints != pointCount) {
    throw IOError("ReadPointcloud");
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
 *  \brief Reads a point range defined by the passed cloud object,
 *  starting from the current point, and populates the associated buffer.
 *
 *  \param [in/out] pointCloud the container object to be populated with points.
 *  \return Returns number of points actually read
 *
 *  \remark thread unsafe: donot call from mulitple threads simutaneously
 */
size_t PointcloudReader::ReadPointcloud(Pointcloud* const pointCloud) {
  if (pointCloud->GetFieldGroup() > mFieldGroup) {
    throw std::out_of_range("Requested fields are out of the range of read fields");
  }

  pointCloud->SetOrigin(Point3d(mLasHeader->x_offset, mLasHeader->y_offset, mLasHeader->z_offset));

  const size_t pointCount = std::min<size_t>(pointCloud->GetPointCount(),
    mLasReader->npoints - mLasReader->p_count);
  for (size_t i = 0; i < pointCount; ++i) {
    // \todo sanity check 
    mLasReader->read_point();
    pointCloud->SetPoint_Unchecked(i, mLasReader->point, mLasHeader);
  }

  return pointCount;
}

/**
 *  \brief obtain dataset information.
 */
void PointcloudReader::ObtainLasInfo() {
  assert(mLasReader);
  mLasHeader = &(mLasReader->header);

  // obtain fields
  mFileFieldGroup.set(FieldGroup::kPOSXYZ);
  mFileFieldGroup.set(FieldGroup::kPOSCLA);
  mFileFieldGroup.set(FieldGroup::kPOSINT);

  switch (mLasHeader->point_data_format) {
  case PointDataTraits::kPDF1:
    mFileFieldGroup.set(FieldGroup::kPOSTIM);
    break;
  case PointDataTraits::kPDF2:
    mFileFieldGroup.set(FieldGroup::kPOSRGB);
    break;
  case PointDataTraits::kPDF3:
    mFileFieldGroup.set(FieldGroup::kPOSRGB);
    mFileFieldGroup.set(FieldGroup::kPOSTIM);
    break;
  case PointDataTraits::kPDF4:
    mFileFieldGroup.set(FieldGroup::kPOSTIM);
    break;
  case PointDataTraits::kPDF5:
    mFileFieldGroup.set(FieldGroup::kPOSRGB);
    mFileFieldGroup.set(FieldGroup::kPOSTIM);
    break;
  case PointDataTraits::kPDF6:
    mFileFieldGroup.set(FieldGroup::kPOSTIM);
    break;
  case PointDataTraits::kPDF7:
    mFileFieldGroup.set(FieldGroup::kPOSRGB);
    mFileFieldGroup.set(FieldGroup::kPOSTIM);
    break;
  case PointDataTraits::kPDF8:
    mFileFieldGroup.set(FieldGroup::kPOSRGB);
    mFileFieldGroup.set(FieldGroup::kPOSNIR);
    mFileFieldGroup.set(FieldGroup::kPOSTIM);
    break;
  case PointDataTraits::kPDF9:
    mFileFieldGroup.set(FieldGroup::kPOSTIM);
    break;
  case PointDataTraits::kPDF10:
    mFileFieldGroup.set(FieldGroup::kPOSRGB);
    mFileFieldGroup.set(FieldGroup::kPOSNIR);
    mFileFieldGroup.set(FieldGroup::kPOSTIM);
    break;
  default:
    break;
  }

  // turn off fields not used till now: intensity, time, nir 
  mFileFieldGroup.reset(FieldGroup::kPOSINT);
  mFileFieldGroup.reset(FieldGroup::kPOSNIR);
  mFileFieldGroup.reset(FieldGroup::kPOSTIM);

  if (mFieldGroup.none()) {
    mFieldGroup = mFileFieldGroup;
  }
  else {
    if (mFieldGroup > mFileFieldGroup)
      throw std::out_of_range("Requested fields are out of the range of file fields");
  }

  mFilePointCount = mLasReader->npoints;
  assert(mLasHeader->number_of_point_records == mFilePointCount);  // extended_number_of_point_records 
}
