/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "core/datasource/pointtypes.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/spatialtile.h"

#include <cassert>
#include <type_traits>
#include <vector>

class LASpoint;
class LASquantizer;

namespace earthxpert {
/**
 *  The class Pointcloud stores an array of points in groups of fields.
 *
 *  \todo optimize performance
 *  \todo the container size is 1 if the corresponding field is equal for all points
 */
class EXCORE_EXPORTS Pointcloud {
public:
  enum PointcouldType {
    eLIDAR,   // scanned by lidar instruments
    ePHOTO    // matched and reconstructed from photos
  };

  Pointcloud(size_t pointCapacity, FieldGroup fieldGroup);
  Pointcloud(const Point3d& origin, std::vector<XYZF>&& xyz);
  Pointcloud(const Point3d& origin, std::vector<XYZF>&& xyz,
    std::vector<Classification>&& cla);
  Pointcloud(const Point3d& origin, std::vector<XYZF>&& xyz,
    std::vector<Classification>&& cla, std::vector<RGB16>&& rgb);
  Pointcloud(const Pointcloud& rhs) = default;
  Pointcloud(Pointcloud&& rhs) noexcept = default;
  ~Pointcloud() = default;

  Pointcloud& operator=(const Pointcloud& rhs) = default;
  Pointcloud& operator=(Pointcloud&& rhs) noexcept = default;

  bool operator==(const Pointcloud& rhs) const;
  bool operator!=(const Pointcloud& rhs) const;

  /**
   *  \return Returns max number of points to hold.
   */
  auto GetPointCapacity() const noexcept {
    return mPointCapacity;
  }

  /**
   *  \breif Set actual number of points
   */
  void SetPointCount(const size_t pointCount) {
    if (pointCount > mPointCapacity) {
      throw std::invalid_argument("SetPointCount: pointCount");
    }
    mPointCount = pointCount;
  }

  /**
   *  \return Returns total number of points actually held.
   */
  auto GetPointCount() const noexcept {
    return mPointCount;
  }

  /**
   *  \return Returns field groups.
   */
  auto GetFieldGroup() const noexcept {
    return mFieldGroup;
  }

  /**
   *  \return Returns memory size in bytes.
   */
  size_t GetBufferNumBytes() const noexcept {
    return (mXYZ.size() * sizeof(XYZF) + mCla.size() * sizeof(Classification) +
      mIntensity.size() * sizeof(Intensity) + mRGB.size() * sizeof(RGB16));
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
   *  \brief set an array of XYZ coordinates whose values have been 
   *  shifted w.r.t. the origin
   *
   *  \param [in] xyz an array of XYZ coordinates
   */
  void SetXYZ(std::vector<XYZF>&& xyz) {
    assert(mFieldGroup.test(FieldGroup::kPOSXYZ));
    assert(mPointCapacity == xyz.size());
    mXYZ = std::move(xyz);
  }

  /**
   *  \brief set XYZ coordinates at specified index
   *
   *  \param [in] i index of point
   *  \param [in] xyz XYZ coordinates that have been shifted w.r.t. the origin
   */
  void SetXYZ(const size_t i, const XYZF xyz) {
    SetXYZ(i, static_cast<Point3f>(xyz));
  }
  void SetXYZ(const size_t i, const Point3f xyz) {
    assert(mFieldGroup.test(FieldGroup::kPOSXYZ));
    assert(i < mPointCapacity);
    mXYZ[i].x = xyz.x;
    mXYZ[i].y = xyz.y;
    mXYZ[i].z = xyz.z;
  }

  /**
   *  \brief set XYZ coordinates at specified index
   *
   *  \param [in] i index of point
   *  \param [in] xyz XYZ coordinates
   */
  void SetXYZ(const size_t i, const XYZD xyz) {
    SetXYZ(i, static_cast<Point3d>(xyz));
  }
  void SetXYZ(const size_t i, const Point3d xyz) {
    assert(mFieldGroup.test(FieldGroup::kPOSXYZ));
    assert(i < mPointCapacity);
    mXYZ[i].x = xyz.x - mOrigin.x;
    mXYZ[i].y = xyz.x - mOrigin.y;
    mXYZ[i].y = xyz.x - mOrigin.y;
  }

  /**
   *  \brief set an array of classifications
   *
   *  \param [in] cla an array of classifications
   */
  void SetCla(std::vector<Classification>&& cla) {
    assert(mFieldGroup.test(FieldGroup::kPOSCLA));
    assert(mPointCapacity == cla.size());
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
    assert(i < mPointCapacity);
    mCla[i] = cla;
  }

  /**
   *  \brief set an array of intensity
   *
   *  \param [in] intensity an array of intensity
   */
  void SetIntensity(std::vector<Intensity>&& intensity) {
    assert(mFieldGroup.test(FieldGroup::kPOSINT));
    assert(mPointCapacity == intensity.size());
    mIntensity = std::move(intensity);
  }

  /**
   *  \brief set intensity at specified index
   *
   *  \param [in] i index of point
   *  \param [in] intensity intensity
   */
  void SetIntensity(const size_t i, const Intensity intensity) {
    assert(mFieldGroup.test(FieldGroup::kPOSINT));
    assert(i < mPointCapacity);
    mIntensity[i] = intensity;
  }

  /**
   *  \brief set an array of RGB
   *
   *  \param [in] rgb an array of RGB
   */
  void SetRGB(std::vector<RGB16>&& rgb) {
    assert(mFieldGroup.test(FieldGroup::kPOSRGB));
    assert(mPointCapacity == rgb.size());
    mRGB = std::move(rgb);
  }

  /**
   *  \brief set RGB at specified index
   *
   *  \param [in] i index of point
   *  \param [in] rgb RGB
   */
  void SetRGB(const size_t i, const RGB16 rgb) {
    assert(mFieldGroup.test(FieldGroup::kPOSRGB));
    assert(i < mPointCapacity);
    mRGB[i] = rgb;
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
   *  \return Returns the array of XYZ coordinates w.r.t. the origin.
   */
  const auto& GetXYZ() const noexcept {
    return mXYZ;
  }
  auto& GetXYZ() noexcept {
    return mXYZ;
  }

  /**
   *  \param [in] i index of point
   *  \return Returns XYZ coordinates at specified index, whose values are w.r.t. the origin.
   */
  const auto& GetXYZ(const size_t i) const noexcept {
    return mXYZ[i];
  }
  auto& GetXYZ(const size_t i) noexcept {
    return mXYZ[i];
  }

  /**
 *  \param [in] i index of point
 *  \return Returns XYZ coordinates at specified index.
 */
  auto GetXYZD(const size_t i) noexcept {
    return XYZD{ mOrigin.x + mXYZ[i].x, mOrigin.y + mXYZ[i].y, mOrigin.z + mXYZ[i].z };
  }
  auto GetPoint3d(const size_t i) noexcept {
    return static_cast<Point3d>(GetXYZD(i));
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
   *  \return Returns array of intensity.
   */
  const auto& GetIntensity() const noexcept {
    return mIntensity;
  }
  auto& GetIntensity() noexcept {
    return mIntensity;
  }

  /**
   *  \param [in] i index of point
   *  \return Returns intensity at specified index.
   */
  const auto& GetIntensity(const size_t i) const noexcept {
    return mIntensity[i];
  }
  auto& GetIntensity(const size_t i) noexcept {
    return mIntensity[i];
  }

  /**
   *  \return Returns array of RGB.
   */
  const auto& GetRGB() const noexcept {
    return mRGB;
  }
  auto& GetRGB() noexcept {
    return mRGB;
  }

  /**
   *  \param [in] i index of point
   *  \return Returns RGB at specified index.
   */
  const auto& GetRGB(const size_t i) const noexcept {
    return mRGB[i];
  }
  auto& GetRGB(const size_t i) noexcept {
    return mRGB[i];
  }

  /**
   *  \brief Set fields to given point.
   *
   *  \param PointTy the point type of WxPoint<>, being all or a partial of available fields
   *  \param [in] i index of point
   *  \param [in] i point object of type PointTy
   */
  template <typename PointTy>
  void SetPoint(const size_t i, const PointTy& point) {
    if (mPointCapacity <= i) {
      throw std::out_of_range("Pointcloud::SetPoint()");
    }
    if (!VerifyPointFieldGroup(PointTy::kPointFieldBits)) {
      throw std::invalid_argument("PointTy");
    }

    SetPoint_Unchecked<PointTy>(i, point);
  }

  /**
   *  \brief Set fields to given point.
   *
   *  \param [in] i index of point
   *  \param [in] point LASlib point
   */
  void SetPoint(const size_t i, const LASpoint& point, const LASquantizer* quantizer) {
    if (mPointCapacity <= i) {
      throw std::out_of_range("Pointcloud::SetPoint()");
    }

    SetPoint_Unchecked(i, point, quantizer);
  }

  /**
   *  \param PointTy the point type of WxPoint<>, being all or a partial of available fields
   *  \param [in] i index of point
   *  \return Returns a point at given index.
   */
  template <typename PointTy> PointTy GetPoint(const size_t i) const {
    if (mPointCapacity <= i) {
      throw std::out_of_range("Pointcloud::GetPoint()");
    }
    if (!VerifyPointFieldGroup(PointTy::kPointFieldBits)) {
      throw std::invalid_argument("PointTy");
    }

    return GetPoint_Unchecked<PointTy>(i);
  }

  /**
   *  \param FieldTy the field type of a WxPoint<> component
   *  \param [in] i index of point
   *  \return Returns a field at given index.
   */
  template <typename FieldTy> const FieldTy& GetField(const size_t i) const {
    static_assert(std::is_same_v<XYZF, FieldTy> || std::is_same_v<Classification, FieldTy> ||
      std::is_same_v<Intensity, FieldTy> || std::is_same_v<RGB16, FieldTy>, "GetField: FieldTy");
    if (mPointCapacity <= i) {
      throw std::out_of_range("Pointcloud::GetField()");
    }
    if (!mFieldGroup.test(FieldTy::kFieldPos)) {
      throw std::invalid_argument("GetField: FieldTy");
    }

    return GetField_Unchecked<FieldTy>(i);
  }

  template <typename FieldTy> FieldTy& GetField(const size_t i) {
    static_assert(std::is_same_v<XYZF, FieldTy> || std::is_same_v<Classification, FieldTy> ||
      std::is_same_v<Intensity, FieldTy> || std::is_same_v<RGB16, FieldTy>, "GetField: FieldTy");
    if (mPointCapacity <= i) {
      throw std::out_of_range("Pointcloud::GetField()");
    }
    if (!mFieldGroup.test(FieldTy::kFieldPos)) {
      throw std::invalid_argument("GetField: FieldTy");
    }

    return GetField_Unchecked<FieldTy>(i);
  }

  /**
   *  \brief subscript access via proxy to a point
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
   *  \param [in] pointFieldGroup The field groups bit mask requested.
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
    if constexpr (std::is_base_of_v<RGB16, PointTy>) {
      mRGB[i].r = point.r;
      mRGB[i].g = point.g;
      mRGB[i].b = point.b;
    }
    // \todo add other field groups
  }

  /**
   *  \brief Set fields of given point.
   *
   *  \param [in] i index of point
   *  \param [in] point LASlib point
   *  \param [in] quantizer scale and offset parameters
   *  \param [in] offset flag indicating if offset is different
   */
  void SetPoint_Unchecked(size_t i, const LASpoint& point, bool offset = true);

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
    if constexpr (std::is_base_of_v<RGB16, PointTy>) {
      point.r = mRGB[i].r;
      point.g = mRGB[i].g;
      point.b = mRGB[i].b;
    }
    // \todo add other field groups

    return point;
  }

  /**
   *  \param FieldTy the field type of a WxPoint<> component
   *  \param [in] i index of point
   *  \return Returns a field at given index.
   */
  template <typename FieldTy> const FieldTy& GetField_Unchecked(const size_t i) const {
    if constexpr (std::is_same_v<XYZF, FieldTy>) {
      return mXYZ[i];
    }
    if constexpr (std::is_same_v<Classification, FieldTy>) {
      return mCla[i];
    }
    if constexpr (std::is_same_v<Intensity, FieldTy>) {
      return mIntensity[i];
    }
    if constexpr (std::is_same_v<RGB16, FieldTy>) {
      return mRGB[i];
    }
    // \todo add other field groups
    assert(false);
  }

  template <typename FieldTy> FieldTy& GetField_Unchecked(const size_t i) {
    if constexpr (std::is_same_v<XYZF, FieldTy>) {
      return mXYZ[i];
    }
    if constexpr (std::is_same_v<Classification, FieldTy>) {
      return mCla[i];
    }
    if constexpr (std::is_same_v<Intensity, FieldTy>) {
      return mIntensity[i];
    }
    if constexpr (std::is_same_v<RGB16, FieldTy>) {
      return mRGB[i];
    }
    // \todo add other field groups
    assert(false);
  }

  /**
   *  \brief Get fields to given point.
   *
   *  \param [in] i index of point
   *  \param [in/out] point LASlib point
   *  \param [in] offset flag indicating if offset is different
   */
  void GetPoint_Unchecked(size_t i, LASpoint& point, bool offset = true) const;

  /**
   *  \brief it is tedious also in-efficient to track the modification of points and maintain their bounding box.
   *
   *  \return Returns 2D extent of region covered by points in XY
   *
   *  \remark This convenient function will always traverse all points to obtain the region extent.
   *  Use it at caution to avoid perforamnce penaty.
   */
  RegionTileD ObtainBoundingBox2D() const noexcept;

private:
  size_t mPointCapacity{};  // max number of points to be able to hold
  FieldGroup mFieldGroup;   // available field groups
  Point3d mOrigin{};        // origin of 3D coordinate system where mPoints reside

  std::vector<XYZF> mXYZ;   // XYZ coordinates
  std::vector<Classification> mCla; // feature classification: ground
  std::vector<Intensity> mIntensity;  // laser reflectance
  std::vector<RGB16> mRGB;    // color

  // uncomment when needed
  //std::vector<GPSTime> mTime;

  // cache
  size_t mPointCount{};     // actual number of points held, default to mPointCapacity
};
}
