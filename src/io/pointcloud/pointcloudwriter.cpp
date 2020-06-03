/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/pointcloud/pointcloudwriter.h"
#include "3rd_party/laslib/exlaslib.h"
#include "core/datasource/pointcloudutils.h"
#include "core/exceptions/exexcept.h"

using namespace earthxpert;
using pdt = earthxpert::thirdparty::PointDataTraits;

/**
 *  \brief Open/create the las file for write
 *
 *  \param [in] lasFile the file name to receive points
 *  \param [in] fieldGroup field group to write
 *  \param [in] quantizer the scale and offset parameters
 *  \param [in] format the file format: las, laz, bin, txt, etc.
 */
PointcloudWriter::PointcloudWriter(const std::string& lasFile,
  const FieldGroup fieldGroup, const LASquantizer& quantizer,
  const std::string format)
  : mFieldGroup(fieldGroup), mQuantizer(quantizer) {
  InitHeader();

  LASwriteOpener lasWriteOpener;
  lasWriteOpener.set_file_name(lasFile.c_str());
  lasWriteOpener.set_format(format.c_str());

  mWriter.reset(lasWriteOpener.open(&mHeader));
  if (!mWriter)
    throw IOError("Cannot open las file for write");

  ObtainLasInfo();
}

/**
 *  \brief Open/create the las file for write, default to XYZC fields
 *
 *  \param [in] lasFile the file name to receive points
 *  \param [in] quantizer the scale and offset parameters
 *  \param [in] format the file format: las, laz, bin, txt, etc.
 */
PointcloudWriter::PointcloudWriter(const std::string& lasFile,
  const LASquantizer& quantizer, const std::string format)
  : PointcloudWriter(lasFile, {FieldGroup::kBITXC}, quantizer, format) {
}

/**
 *  \brief Inject metadata tags
 *
 *  \param [in] metaData the metadata tags
 */
void PointcloudWriter::SetMetadata(const VectorMetadata* metaData) {
}

/**
 *  \brief Inject the ground coordinate system where the image file operates
 *
 *  \param [in] spatialref the coordinate system
 */
void PointcloudWriter::SetCoordinateSystem(const SpatialReferenceSystem* srs) {
}

/**
 *  \brief Write an array of points into the opened file.
 *
 *  \param [in] pointCloud the Pointcloud object holding an array of points.
 *              The fields to be written must be compatible with the dataset.
 *  \return Returns number of points written
 *
 *  \todo filter
 */
size_t PointcloudWriter::WritePointcloud(Pointcloud const* const pointCloud) {
  assert(mWriter);
  return WritePointcloud(pointCloud, 0, pointCloud->GetPointCount());
}

/**
 *  \brief Write an array of points into the opened file.
 *
 *  \param [in] pointCloud the Pointcloud object holding an array of points.
 *              The fields to be written must be compatible with the dataset.
 *  \param [in] start the index of start point
 *  \param [in] numPoints number of points to write
 *  \return Returns number of points written
 *
 *  \todo filter
 */
size_t PointcloudWriter::WritePointcloud(Pointcloud const* const pointCloud,
  const size_t start, const size_t numPoints) {
  if (pointCloud->GetFieldGroup() > mFieldGroup) {
    throw std::out_of_range("Fields to be written are out of the range of write fields");
  }
  const auto pointCount = pointCloud->GetPointCount();
  if (!pointCloud || 0 == pointCount) {
    return 0;
  }

  bool offset{true};  //

  LASpoint point;
  point.init(&mQuantizer, mHeader.point_data_format, mHeader.point_data_record_length);

  std::lock_guard<std::mutex> lock(mMutex);
  const size_t writeNumPoints = std::min<size_t>(numPoints, pointCount - start);
  for (size_t i = start; i < start + writeNumPoints; ++i) {
    pointCloud->GetPoint_Unchecked(i, point, offset);

    mWriter->write_point(&point);
    mWriter->update_inventory(&point);
  }

  mWriter->update_header(&mHeader, TRUE);
  return writeNumPoints;
}

/**
 *  \brief initialize LASheader for writing.
 */
void PointcloudWriter::InitHeader() {
  mHeader.point_data_format = FieldGroupToFormat(mFieldGroup);
  mHeader.point_data_record_length = pdt::FormatToRecLen(mHeader.point_data_format);

  auto quantizer = static_cast<LASquantizer*>(&mHeader);
  *quantizer = mQuantizer;

  strcpy(mHeader.generating_software, "EXAI");

  // \todo vlr
}

/**
 *  \brief obtain dataset information.
 */
void PointcloudWriter::ObtainLasInfo() {
  assert(mWriter);
  mInventory = &(mWriter->inventory);
}
