/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/datasource/pointtypes.h"
#include "core/exceptions/wxexcept.h"

#include <cassert>
#include <type_traits>
#include <vector>

namespace earthxpert {
/**
 *  The class Pointcloud stores an array of points in groups of fields.
 */
class WXCORE_EXPORTS Pointcloud {
public:
  Pointcloud(size_t pointCount, FieldGroup fieldGroup);
  Pointcloud(const Point3d& origin, std::vector<XYZF>&& xyz);
  Pointcloud(const Point3d& origin, std::vector<XYZF>&& xyz,
    std::vector<Classification>&& cla);
  Pointcloud(const Pointcloud& rhs) = default;
  Pointcloud(Pointcloud&& rhs) noexcept = default;
  ~Pointcloud() = default;

  Pointcloud& operator=(const Pointcloud& rhs) = default;
  Pointcloud& operator=(Pointcloud&& rhs) noexcept = default;

  bool operator==(const Pointcloud& rhs) const;
  bool operator!=(const Pointcloud& rhs) const;

  /**
   *  \return Returns total number of points.
   */
  auto GetPointCount() const {
    return mPointCount;
  }

  /**
   *  \return Returns field groups.
   */
  auto GetFieldGroup() const {
    return mFieldGroup;
  }

  /**
   *  \brief set origin
   *
   *  \param [in] origin origin point
   */
  void SetOrigin(const Point3d& origin) {
    assert(mFieldGroup.test(FieldGroup::kPOSXYZ));
    mOrigin = origin;
  }

  /**
   *  \brief set an array of XYZ coordinates
   *
   *  \param [in] xyz an array of XYZ coordinates
   */
  void SetXYZ(std::vector<XYZF>&& xyz) {
    assert(mFieldGroup.test(FieldGroup::kPOSXYZ));
    assert(mPointCount == xyz.size());
    mXYZ = std::move(xyz);
  }

  /**
   *  \brief set XYZ coordinates at specified index
   *
   *  \param [in] i index of point
   *  \param [in] xyz XYZ coordinates
   */
  void SetXYZ(const size_t i, const XYZF xyz) {
    assert(mFieldGroup.test(FieldGroup::kPOSXYZ));
    assert(i < mPointCount);
    mXYZ[i] = xyz;
  }

  /**
   *  \brief set an array of classifications
   *
   *  \param [in] cla an array of classifications
   */
  void SetCla(std::vector<Classification>&& cla) {
    assert(mFieldGroup.test(FieldGroup::kPOSCLA));
    assert(mPointCount == cla.size());
    mCla = std::move(cla);
  }

  /**
   *  \brief set classification at specified index
   *
   *  \param [in] i index of point
   *  \param [in] cla classification
   */
  void SetCla(const size_t i, const Classification cla) {
    assert(mFieldGroup.test(FieldGroup::kPOSCLA));
    assert(i < mPointCount);
    mCla[i] = cla;
  }

  /**
   *  \return Returns origin.
   */
  const Point3d& GetOrigin() const noexcept {
    return mOrigin;
  }
  Point3d& GetOrigin() noexcept {
    return mOrigin;
  }

  /**
   *  \return Returns the array of XYZ coordinates.
   */
  const auto& GetXYZ() const noexcept {
    return mXYZ;
  }
  auto& GetXYZ() noexcept {
    return mXYZ;
  }

  /**
   *  \param [in] i index of point
   *  \return Returns XYZ coordinates at specified index.
   */
  const auto& GetXYZ(const size_t i) const noexcept {
    return mXYZ[i];
  }
  auto& GetXYZ(const size_t i) noexcept {
    return mXYZ[i];
  }

  /**
   *  \return Returns array of classifications.
   */
  const auto& GetCla() const noexcept {
    return mCla;
  }
  auto& GetCla() noexcept {
    return mCla;
  }

  /**
   *  \param [in] i index of point
   *  \return Returns classification at specified index.
   */
  const auto& GetCla(const size_t i) const noexcept {
    return mCla[i];
  }
  auto& GetCla(const size_t i) noexcept {
    return mCla[i];
  }

  /**
   *  \brief Set fields to given point.
   *
   *  \param PointTy the point type of WxPoint<>
   *  \param [in] i index of point
   */
  template <typename PointTy>
  void SetPoint(const size_t i, const PointTy& point) {
    if (mPointCount <= i) {
      throw std::out_of_range("Pointcloud::GetPoint()");
    }
    if (!VerifyPointFieldGroup(PointTy::kPointFieldGroup)) {
      throw std::invalid_argument("PointTy");
    }

    SetPoint_Unchecked<PointTy>(i, point);
  }

  /**
   *  \param PointTy the point type of WxPoint<>
   *  \param [in] i index of point
   *  \return Returns a point at given index.
   */
  template <typename PointTy>
  PointTy GetPoint(const size_t i) const {
    if (mPointCount <= i) {
      throw std::out_of_range("Pointcloud::GetPoint()");
    }
    if (!VerifyPointFieldGroup(PointTy::kPointFieldGroup)) {
      throw std::invalid_argument("PointTy");
    }

    return GetPoint_Unchecked<PointTy>(i);
  }

  /**
   *  \brief subscript access via proxy
   *
   *  \param PointcloudTy const or non-const Pointcloud
   *  \remark Use for occasional point access. 
   */
  template <typename PointcloudTy>
  struct PointProxy {
    static_assert(std::is_same_v<Pointcloud, std::remove_cv_t<PointcloudTy>>, "Must be Pointcloud");
    using Pointcloud_Pointer = std::add_pointer_t<PointcloudTy>;
    
    PointProxy(Pointcloud_Pointer pointCloud, const size_t index) : pc(pointCloud), i(index) {}

    template <typename PointTy>
    PointProxy& operator=(const PointTy& point) {
      pc->SetPoint_Unchecked<PointTy>(i, point);
      return *this;
    }

    template <typename PointTy>
    operator PointTy() const {
      return pc->GetPoint_Unchecked<PointTy>(i);
    }

    Pointcloud_Pointer pc{};
    size_t i{};
  };

  /**
   *  \param [in] i index of point
   *  \return Returns proxy of a point at given index.
   */
  auto operator[](const size_t i) const {
    return PointProxy<const Pointcloud>(this, i);
  }
  auto operator[](const size_t i) {
    return PointProxy<Pointcloud>(this, i);
  }

  /**
   *  \brief verify if point cloud contains fields requested.
   *
   *  \param [in] ptFieldGroup The field groups bit mask requested.
   *  \return Returns true if point cloud contains all the requested field groups, otherwise false.
   */
  bool VerifyPointFieldGroup(const size_t pointFieldGroup) const {
    bool ret{true};
    for (auto [pos, bit] : FieldGroup::kPOSBIT) {
      if (bit & pointFieldGroup) {
        ret = ret && mFieldGroup.test(pos);
      }
    }

    return ret;
  }

  /**
   *  \brief Set fields to given point.
   *
   *  \param [in] i index of point
   *  \param PointTy the point type of WxPoint<>
   */
  template <typename PointTy>
  void SetPoint_Unchecked(const size_t i, const PointTy& point) {
    if constexpr (std::is_base_of_v<XYZF, PointTy>) {
      mXYZ[i].x = point.x;
      mXYZ[i].y = point.y;
      mXYZ[i].z = point.z;
    }
    if constexpr (std::is_base_of_v<Classification, PointTy>) {
      mCla[i].c = point.c;
    }
    if constexpr (std::is_base_of_v<Intensity, PointTy>) {
      mIntensity[i].i = point.i;
    }
    // \todo add other field groups
  }

  /**
   *  \param [in] i index of point
   *  \return Returns a point at given index.
   */
  template <typename PointTy>
  PointTy GetPoint_Unchecked(const size_t i) const {
    PointTy point;
    if constexpr (std::is_base_of_v<XYZF, PointTy>) {
      point.x = mXYZ[i].x;
      point.y = mXYZ[i].y;
      point.z = mXYZ[i].z;
    }
    if constexpr (std::is_base_of_v<Classification, PointTy>) {
      point.c = mCla[i].c;
    }
    if constexpr (std::is_base_of_v<Intensity, PointTy>) {
      point.i = mIntensity[i].i;
    }
    // \todo add other field groups

    return point;
  }

private:
  size_t mPointCount{};
  FieldGroup mFieldGroup;   // available field groups

  Point3d mOrigin{};        // origin of 3D coordinate system where mPoints reside
  std::vector<XYZF> mXYZ;   // XYZ coordinates
  std::vector<Classification> mCla; // feature classification: ground
  std::vector<Intensity> mIntensity;
  // uncomment when needed
  //std::vector<RGB<uint32_t>> mRGB;
  //std::vector<GPSTime> mTime;
};
}
