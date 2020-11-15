/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/datasource/pointcloud.h"
#include "core/exceptions/exexcept.h"
#include "core/math/math.h"

#include <laspoint.hpp>
#include <laszip.hpp>
#include <cmath>
#include <limits>
#include <omp.h>
#include <type_traits>

using namespace earthxpert;

/**
 *  \brief Construct Pointcloud object.
 *
 *  \param [in] pointCapacity number of points to be held in cloud
 *  \param [in] fieldGroup pointa contain which field groups
 */
Pointcloud::Pointcloud(const size_t pointCapacity, const FieldGroup fieldGroup)
  : mPointCapacity(pointCapacity), mFieldGroup(fieldGroup) {
  if (0 == mPointCapacity) 
    throw std::invalid_argument(_("pointCapacity"));
  if (mFieldGroup.none()) 
    throw std::invalid_argument(_("fieldGroup"));
  mPointCount = mPointCapacity; // default to be capacity

  for (auto pos : FieldGroup::kPOS) {
    if (!mFieldGroup.test(pos)) continue;

    switch (pos) {
    case FieldGroup::kPOSXYZ:
      mXYZ.resize(mPointCapacity);
      break;
    case FieldGroup::kPOSCLA:
      mCla.resize(mPointCapacity);
      break;
    case FieldGroup::kPOSINT:
      mIntensity.resize(mPointCapacity);
      break;
    case FieldGroup::kPOSRGB:
      mRGB.resize(mPointCapacity);
      break;
    default:
      assert(false && "Pointcloud");
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
  mPointCount = mPointCapacity = mXYZ.size();
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
  assert(mCla.size() == mPointCapacity);
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
  assert(mRGB.size() == mPointCapacity);
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
    if (mPointCount != rhs.GetPointCount() || 
      mFieldGroup != rhs.GetFieldGroup()) {
      return false;
    }

    // \todo
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
 *  \param [in] offset flag indicating if offset is different
 *
 *  \remark mOrigin must be equal to offsets given in quantizer
 */
void Pointcloud::SetPoint_Unchecked(const size_t i, 
  const LASpoint& point, const bool offset) {
  // \todo sanity check
  if (!mXYZ.empty()) {
    auto quantizer = point.quantizer;

    double x_offset{}, y_offset{}, z_offset{};
    if (offset) {
      x_offset = quantizer->x_offset - mOrigin.x;
      y_offset = quantizer->y_offset - mOrigin.y;
      z_offset = quantizer->z_offset - mOrigin.z;
    }
    
    mXYZ[i].x = static_cast<float>(x_offset + 
      double(point.X) * quantizer->x_scale_factor);
    mXYZ[i].y = static_cast<float>(y_offset + 
      double(point.Y) * quantizer->y_scale_factor);
    mXYZ[i].z = static_cast<float>(z_offset + 
      double(point.Z) * quantizer->z_scale_factor);
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

/**
 *  \brief Get fields to given point.
 *
 *  \param [in] i index of point
 *  \param [in/out] point LASlib point
 *  \param [in] offset flag indicating if offset is different
 *
 *  \remark mOrigin must be equal to offsets given in quantizer
 */
void Pointcloud::GetPoint_Unchecked(const size_t i, 
  LASpoint& point, const bool offset) const {
  // \todo sanity check
  if (!mXYZ.empty()) {
    auto quantizer = point.quantizer;
    
    double x_offset{}, y_offset{}, z_offset{};
    if (offset) {
      x_offset = mOrigin.x - quantizer->x_offset;
      y_offset = mOrigin.y - quantizer->y_offset;
      z_offset = mOrigin.z - quantizer->z_offset;
    }
    
    point.X = static_cast<int32_t>(std::lround((double(mXYZ[i].x) + x_offset) / quantizer->x_scale_factor));
    point.Y = static_cast<int32_t>(std::lround((double(mXYZ[i].y) + y_offset) / quantizer->y_scale_factor));
    point.Z = static_cast<int32_t>(std::lround((double(mXYZ[i].z) + z_offset) / quantizer->z_scale_factor));
  }
  if (!mCla.empty()) {
    point.classification = mCla[i].c;
  }
  if (!mIntensity.empty()) {
    point.intensity = mIntensity[i].i;
  }
  if (!mRGB.empty()) {
    point.rgb[0] = mRGB[i].r;
    point.rgb[1] = mRGB[i].g;
    point.rgb[2] = mRGB[i].b;
  }
  // \todo add other field groups
}

/**
 *  \brief it is tedious also in-efficient to track the modification 
 *  of points and maintain their bounding box.
 *
 *  \return Returns 2D extent of region covered by points in XY
 *  
 *  \remark This convenient function will traverse all points 
 *  to obtain the region extent.
 */
SpatialTileD Pointcloud::GetBufTile() {
  if (mBufTile) 
    return mBufTile.value();
  assert(mFieldGroup.test(FieldGroup::kPOSXYZ));
  if (0 == mPointCount) return {};

  // calculate extents per threads
  const int numThreads = omp_get_max_threads();
  std::vector<float> minXs(numThreads, mXYZ[0].x);
  std::vector<float> maxXs(numThreads, mXYZ[0].x);
  std::vector<float> minYs(numThreads, mXYZ[0].y);
  std::vector<float> maxYs(numThreads, mXYZ[0].y);

#pragma omp parallel if (100000 < mPointCount)
  {
    float minX = std::numeric_limits<float>::max();
    float maxX = -std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxY = -std::numeric_limits<float>::max();

#pragma omp for
    for (int64_t i = 0; i < mPointCount; ++i) {
      MinMax(minX, maxX, mXYZ[i].x);
      MinMax(minY, maxY, mXYZ[i].y);
    }

    const int tId = omp_get_thread_num();
    minXs[tId] = minX;
    maxXs[tId] = maxX;
    minYs[tId] = minY;
    maxXs[tId] = maxX;
  }

  // calculate extent of all points
  const float minX = *std::min_element(minXs.begin(), minXs.end());
  const float maxX = *std::max_element(maxXs.begin(), maxXs.end());
  const float minY = *std::min_element(minYs.begin(), minYs.end());
  const float maxY = *std::max_element(maxYs.begin(), maxYs.end());

  SpatialTileD tile{ mOrigin.x + double(minX), mOrigin.y + double(minY),
    double(maxX) - double(minX), double(maxY) - double(minY) };
  if (!tile.IsValid())
    throw DataError(_("Pointcloud: invalid data range."));
  
  mBufTile = tile;
  if (!mTile.IsValid())   // all points are within AOI
    mTile = tile;

  return tile;
}

void Pointcloud::Swap(Pointcloud& rhs) noexcept {
  std::swap(mPointCapacity, rhs.mPointCapacity);
  std::swap(mFieldGroup, rhs.mFieldGroup);
  std::swap(mOrigin, rhs.mOrigin);
  std::swap(mXYZ, rhs.mXYZ);
  std::swap(mCla, rhs.mCla);
  std::swap(mIntensity, rhs.mIntensity);
  std::swap(mRGB, rhs.mRGB);
  //std::swap(mTime, rhs.mTime);
  std::swap(mPointCount, rhs.mPointCount);
  //std::swap(mData, rhs.mData); ?
}
