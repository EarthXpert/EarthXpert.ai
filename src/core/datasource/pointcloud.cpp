/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/datasource/pointcloud.h"

#include "LASlib/laszip.hpp"
#include "LASlib/laspoint.hpp"
#include <type_traits>

using namespace earthxpert;

/**
 *  \brief Construct Pointcloud object.
 *
 *  \param [in] pointCount number of points in cloud
 *  \param [in] fieldGroup pointa contain which field groups
 */
Pointcloud::Pointcloud(const size_t pointCount, const FieldGroup fieldGroup)
  : mPointCount(pointCount), mFieldGroup(fieldGroup) {
  if (0 == mPointCount) {
    throw std::invalid_argument("pointCount");
  }
  if (mFieldGroup.none()) {
    throw std::invalid_argument("fieldGroup");
  }

  for (auto pos : FieldGroup::kPOS) {
    if (mFieldGroup.test(pos)) {
      switch (pos) {
      case FieldGroup::kPOSXYZ:
        mXYZ.resize(mPointCount);
        break;
      case FieldGroup::kPOSCLA:
        mCla.resize(mPointCount);
        break;
      case FieldGroup::kPOSINT:
        mIntensity.resize(mPointCount);
        break;
      case FieldGroup::kPOSRGB:
        mRGB.resize(mPointCount);
        break;
      default:
        assert(false);
      }
    }
  }
}

/**
 *  \brief Construct Pointcloud object.
 *
 *  \param [in] origin origin of XYZ coordinates
 *  \param [in] xyz array of XYZ coordinates
 */
Pointcloud::Pointcloud(const Point3d& origin, std::vector<XYZF>&& xyz)
  : mOrigin(origin), mXYZ(std::move(xyz)) {
  mFieldGroup.set(FieldGroup::kPOSXYZ);
  mPointCount = mXYZ.size();
}

/**
 *  \brief Construct Pointcloud object.
 *
 *  \param [in] origin origin of XYZ coordinates
 *  \param [in] xyz array of XYZ coordinates
 *  \param [in] cla array of classifications
 */
Pointcloud::Pointcloud(const Point3d& origin, std::vector<XYZF>&& xyz,
  std::vector<Classification>&& cla)
  : Pointcloud(origin, std::move(xyz)) {
  mCla = std::move(cla);
  mFieldGroup.set(FieldGroup::kPOSCLA);
  assert(mCla.size() == mPointCount);
}

/**
 *  \brief Construct Pointcloud object.
 *
 *  \param [in] origin origin of XYZ coordinates
 *  \param [in] xyz array of XYZ coordinates
 *  \param [in] cla array of classifications
 */
Pointcloud::Pointcloud(const Point3d& origin, std::vector<XYZF>&& xyz,
  std::vector<Classification>&& cla, std::vector<RGB16>&& rgb)
  : Pointcloud(origin, std::move(xyz), std::move(cla)) {
  mRGB = std::move(rgb);
  mFieldGroup.set(FieldGroup::kPOSRGB);
  assert(mRGB.size() == mPointCount);
}

/**
 * Equality operator
 *
 * \param [in] rhs Object to compare with
 * \return true if same object or equal, false otherwise.
 * \todo
 */
bool Pointcloud::operator==(const Pointcloud& rhs) const {
  if (this != &rhs) {
    return false;
  }
  return true;
}

/**
 * Test inequality.
 *
 * \param [in] rhs Object to compare with
 * \return true if not equal.
 */
bool Pointcloud::operator!=(const Pointcloud& rhs) const {
  return !(operator==(rhs));
}

/**
 *  \brief Set fields to given point.
 *
 *  \param [in] i index of point
 *  \param [in] point LASlib point
 *  \param [in] quantizer scale and offset parameters
 *
 *  \remark mOrigin must be equal to offsets given in quantizer
 */
void Pointcloud::SetPoint_Unchecked(const size_t i, 
  const LASpoint& point, const LASquantizer* quantizer) {
  // \todo sanity check
  if (!mXYZ.empty()) {
    mXYZ[i].x = static_cast<float>(point.X * quantizer->x_scale_factor);
    mXYZ[i].y = static_cast<float>(point.Y * quantizer->y_scale_factor);
    mXYZ[i].z = static_cast<float>(point.Z * quantizer->z_scale_factor);
  }
  if (!mCla.empty()) {
    mCla[i].c = point.classification;
  }
  if (!mIntensity.empty()) {
    mIntensity[i].i = point.intensity;
  }
  if (!mRGB.empty()) {
    mRGB[i].r = point.rgb[0];
    mRGB[i].g = point.rgb[1];
    mRGB[i].b = point.rgb[2];
  }
  // \todo add other field groups
}
