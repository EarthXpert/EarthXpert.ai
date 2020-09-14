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
#include <optional>
#include <type_traits>
#include <vector>

class LASpoint;
class LASquantizer;

namespace earthxpert {
/**
 *  The class Pointcloud stores an array of points in groups of fields.
 *
 *  \todo optimize performance: accept pre-allocated buffers
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
  auto GetPointCapacity() const noexcept { return mPointCapacity; }

  /**
   *  \breif Set actual number of points
   */
  void SetPointCount(const size_t pointCount) {
    if (pointCount > mPointCapacity) 
      throw std::invalid_argument("SetPointCount: pointCount");
    mPointCount = pointCount;
  }

  /**
   *  \return Returns total number of points actually held.
   */
  auto GetPointCount() const noexcept { return mPointCount; }

  /**
   *  \return Returns field groups.
   */
  auto GetFieldGroup() const noexcept { return mFieldGroup; }

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
   *  \brief set an array of field objects. For XYZ coordinates, their values have been 
   *  shifted w.r.t. the origin.
   *
   *  \param [in] fields an array of FieldTy objects
   */
  template <typename FieldTy> void SetField(std::vector<FieldTy>&& fields) {
    VerifyFieldType<FieldTy>();
    assert(mPointCapacity == fields.size());
    SetField_Unchecked<FieldTy>(std::move(fields));
  }

  /**
   *  \brief set a field object at specified index.
   *  For XYZ coordinates, their values have been shifted w.r.t. the origin
   *
   *  \param [in] i index of point
   *  \param [in] field the field object.
   */
  template <typename FieldTy> void SetField(const size_t i, const FieldTy& field) {
    VerifyFieldType<FieldTy>();
    assert(i < mPointCapacity);
    SetField_Unchecked<FieldTy>(i, field);
  }

  /**
   *  \brief set XYZ coordinates at specified index
   *
   *  \param [in] i index of point
   *  \param [in] xyz XYZ coordinates
   */
  void SetXYZ(const size_t i, const Point3f xyz) {
    assert(mFieldGroup.test(FieldGroup::kPOSXYZ));
    assert(i < mPointCapacity);
    mXYZ[i].x = xyz.x;
    mXYZ[i].y = xyz.y;
    mXYZ[i].z = xyz.z;
  }

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
   *  \return Returns origin.
   */
  const Point3d& GetOrigin() const noexcept { return mOrigin; }
  Point3d& GetOrigin() noexcept { return mOrigin; }

  /**
   *  \return Returns the array of field objects. XYZ coordinates are w.r.t. the origin.
   */
  template <typename FieldTy> const std::vector<FieldTy>& GetField() const {
    VerifyFieldType<FieldTy>();
    return GetField_Unchecked<FieldTy>();
  }
  template <typename FieldTy> std::vector<FieldTy>& GetField() {
    VerifyFieldType<FieldTy>();
    return GetField_Unchecked<FieldTy>();
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
   *  \param FieldTy the field type of a WxPoint<> component
   *  \param [in] i index of point
   *  \return Returns a field at given index.
   */
  template <typename FieldTy> const FieldTy& GetField(const size_t i) const {
    VerifyFieldType<FieldTy>();
    if (mPointCapacity <= i) 
      throw std::out_of_range("Pointcloud::GetField()");

    return GetField_Unchecked<FieldTy>(i);
  }

  template <typename FieldTy> FieldTy& GetField(const size_t i) {
    VerifyFieldType<FieldTy>();
    if (mPointCapacity <= i) 
      throw std::out_of_range("Pointcloud::GetField()");

    return GetField_Unchecked<FieldTy>(i);
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
    if (mPointCapacity <= i) 
      throw std::out_of_range("Pointcloud::SetPoint()");
    VerifyPointFieldBits(PointTy::kPointFieldBits);

    SetPoint_Unchecked<PointTy>(i, point);
  }

  /**
   *  \brief Set fields to given point.
   *
   *  \param [in] i index of point
   *  \param [in] point LASlib point
   */
  void SetPoint(const size_t i, const LASpoint& point, const LASquantizer* quantizer) {
    if (mPointCapacity <= i) 
      throw std::out_of_range("Pointcloud::SetPoint()");

    SetPoint_Unchecked(i, point, quantizer);
  }

  /**
   *  \param PointTy the point type of WxPoint<>, being all or a partial of available fields
   *  \param [in] i index of point
   *  \return Returns a point at given index.
   */
  template <typename PointTy> PointTy GetPoint(const size_t i) const {
    if (mPointCapacity <= i) 
      throw std::out_of_range("Pointcloud::GetPoint()");
    VerifyPointFieldBits(PointTy::kPointFieldBits);

    return GetPoint_Unchecked<PointTy>(i);
  }

  /**
   *  \param FieldTy the field type of a WxPoint<> component
   *  \param [in] fields an array of FieldTy objects
   */
  template <typename FieldTy> void SetField_Unchecked(std::vector<FieldTy>&& fields) {
    if constexpr(std::is_same_v<XYZF, FieldTy>) {
      mXYZ = std::move(fields);
    }
    if constexpr(std::is_same_v<Classification, FieldTy>) {
      mCla = std::move(fields);
    }
    if constexpr(std::is_same_v<Intensity, FieldTy>) {
      mIntensity = std::move(fields);
    }
    if constexpr(std::is_same_v<RGB16, FieldTy>) {
      mRGB = std::move(fields);
    }
    // \todo add other field groups
  }

  /**
   *  \param FieldTy the field type of a WxPoint<> component
   *  \return Return a reference to the array of FieldTy objects
   */
  template <typename FieldTy> const std::vector<FieldTy>& GetField_Unchecked() const {
    if constexpr(std::is_same_v<XYZF, FieldTy>) {
      return mXYZ;
    }
    if constexpr(std::is_same_v<Classification, FieldTy>) {
      return mCla;
    }
    if constexpr(std::is_same_v<Intensity, FieldTy>) {
      return mIntensity;
    }
    if constexpr(std::is_same_v<RGB16, FieldTy>) {
      return mRGB;
    }
    // \todo add other field groups
    assert(false);
  }

  template <typename FieldTy> std::vector<FieldTy>& GetField_Unchecked() {
    if constexpr(std::is_same_v<XYZF, FieldTy>) {
      return mXYZ;
    }
    if constexpr(std::is_same_v<Classification, FieldTy>) {
      return mCla;
    }
    if constexpr(std::is_same_v<Intensity, FieldTy>) {
      return mIntensity;
    }
    if constexpr(std::is_same_v<RGB16, FieldTy>) {
      return mRGB;
    }
    // \todo add other field groups
    assert(false);
  }

  /**
   *  \param FieldTy the field type of a WxPoint<> component
   *  \param [in] i index of point
   *  \param [in] field the FieldTy object
   */
  template <typename FieldTy> void SetField_Unchecked(const size_t i, const FieldTy& field) {
    if constexpr(std::is_same_v<XYZF, FieldTy>) {
      mXYZ[i] = field;
    }
    if constexpr(std::is_same_v<Classification, FieldTy>) {
      mCla[i] = field;
    }
    if constexpr(std::is_same_v<Intensity, FieldTy>) {
      mIntensity[i] = field;
    }
    if constexpr(std::is_same_v<RGB16, FieldTy>) {
      mRGB[i] = field;
    }
    // \todo add other field groups
  }

  /**
   *  \param FieldTy the field type of a WxPoint<> component
   *  \param [in] i index of point
   *  \return Returns a field at given index.
   */
  template <typename FieldTy> const FieldTy& GetField_Unchecked(const size_t i) const {
    if constexpr(std::is_same_v<XYZF, FieldTy>) {
      return mXYZ[i];
    }
    if constexpr(std::is_same_v<Classification, FieldTy>) {
      return mCla[i];
    }
    if constexpr(std::is_same_v<Intensity, FieldTy>) {
      return mIntensity[i];
    }
    if constexpr(std::is_same_v<RGB16, FieldTy>) {
      return mRGB[i];
    }
    // \todo add other field groups
    assert(false);
  }

  template <typename FieldTy> FieldTy& GetField_Unchecked(const size_t i) {
    if constexpr(std::is_same_v<XYZF, FieldTy>) {
      return mXYZ[i];
    }
    if constexpr(std::is_same_v<Classification, FieldTy>) {
      return mCla[i];
    }
    if constexpr(std::is_same_v<Intensity, FieldTy>) {
      return mIntensity[i];
    }
    if constexpr(std::is_same_v<RGB16, FieldTy>) {
      return mRGB[i];
    }
    // \todo add other field groups
    assert(false);
  }

  /**
   *  \brief Set fields to given point.
   *
   *  \param [in] i index of point
   *  \param PointTy the point type of WxPoint<>
   */
  template <typename PointTy>
  void SetPoint_Unchecked(const size_t i, const PointTy& point) {
    if constexpr(std::is_base_of_v<XYZF, PointTy>) {
      mXYZ[i] = static_cast<const XYZF&>(point);  // SetField<XYZF>()
    }
    if constexpr(std::is_base_of_v<Classification, PointTy>) {
      mCla[i].c = point.c;
    }
    if constexpr(std::is_base_of_v<Intensity, PointTy>) {
      mIntensity[i].i = point.i;
    }
    if constexpr(std::is_base_of_v<RGB16, PointTy>) {
      mRGB[i] = static_cast<const RGB16&>(point);
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
    if constexpr(std::is_base_of_v<XYZF, PointTy>) {
      auto& xyz = static_cast<XYZF&>(point);
      xyz = mXYZ[i];    // GetField<XYZF>(i)
    }
    if constexpr(std::is_base_of_v<Classification, PointTy>) {
      point.c = mCla[i].c;
    }
    if constexpr(std::is_base_of_v<Intensity, PointTy>) {
      point.i = mIntensity[i].i;
    }
    if constexpr(std::is_base_of_v<RGB16, PointTy>) {
      auto& rgb = static_cast<RGB16&>(point);
      rgb = mRGB[i];
    }
    // \todo add other field groups

    return point;
  }

  /**
   *  \brief Set fields of given point.
   *
   *  \param [in] i index of point
   *  \param [in] point the LASlib point
   *  \param [in] offset flag indicating if offset is different
   */
  void SetPoint_Unchecked(size_t i, const LASpoint& point, bool offset = true);

  /**
   *  \brief Get fields to given point.
   *
   *  \param [in] i index of point
   *  \param [in/out] point LASlib point
   *  \param [in] offset flag indicating if offset is different
   */
  void GetPoint_Unchecked(size_t i, LASpoint& point, bool offset = true) const;

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
    operator PointTy() const { return pc->GetPoint_Unchecked<PointTy>(i); }

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
  auto operator[](const size_t i) { return PointProxy<Pointcloud>(this, i); }

  SpatialTileD GetTile() const noexcept { return mTile; }

  /**
   *  \brief it is tedious also in-efficient to track the modification 
   *  of points and maintain their bounding box.
   *
   *  \return Returns 2D extent of region covered by points in XY
   *
   *  \remark This convenient function will traverse all points 
   *  to obtain the region extent.
   */
  SpatialTileD GetBufTile() noexcept;

  void Swap(Pointcloud& rhs) noexcept;

private:
  SpatialTileD mTile;   // 2D bounding box of focusing points
  size_t mPointCapacity{};  // max number of points to be able to hold
  FieldGroup mFieldGroup;   // available field groups
  Point3d mOrigin{};        // origin of 3D coordinate system where mPoints reside

  std::vector<XYZF> mXYZ;   // XYZ coordinates
  std::vector<Classification> mCla; // feature classification: ground
  std::vector<Intensity> mIntensity;  // laser reflectance
  std::vector<RGB16> mRGB;    // color

  // uncomment when needed
  //std::vector<GPSTime> mTime;

  /**
   *  \brief verify if point cloud contains fields requested.
   *
   *  \param [in] pointFieldGroup The field groups bit mask requested.
   *  \throw Throws invalid_argument if point cloud does not contain all the requested field groups.
   */
  void VerifyPointFieldBits(const size_t pointFieldBits) const {
    bool good{ true };
    for (auto [pos, bit] : FieldGroup::kPOSBIT) {
      if (bit & pointFieldBits) {
        good = good && mFieldGroup.test(pos);
      }
    }

    if (!good) 
      throw std::invalid_argument("VerifyPointFieldBits: pointFieldBits");
  }

  /**
   *  \param FieldTy the field type of a WxPoint<> component
   *  \throw Throws invalid_argument if point cloud does not contain the requested field group.
   */
  template <typename FieldTy> void VerifyFieldType() const {
    static_assert(std::is_same_v<XYZF, FieldTy> || std::is_same_v<Classification, FieldTy> ||
      std::is_same_v<Intensity, FieldTy> || std::is_same_v<RGB16, FieldTy>, "VerifyFieldType: FieldTy");
    if (!mFieldGroup.test(FieldTy::kFieldPos)) 
      throw std::invalid_argument("VerifyFieldType: FieldTy");
  }

  // cache
  size_t mPointCount{};     // actual number of points held, default to mPointCapacity
  std::optional<SpatialTileD> mBufTile;   // 2D bounding box of all points including focused & buffered
  std::tuple<std::vector<XYZF>*, std::vector<Classification>*, std::vector<Intensity>*,
    std::vector<RGB16>*> mData{ &mXYZ, &mCla, &mIntensity, &mRGB }; // indexed by kPOS
};

using UPPointcloud = std::unique_ptr<Pointcloud>;
using SPPointcloud = std::shared_ptr<Pointcloud>;
}

// overload std::swap
namespace std {
inline void swap(earthxpert::Pointcloud& lhs, 
  earthxpert::Pointcloud& rhs) noexcept { lhs.Swap(rhs); }
}
