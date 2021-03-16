/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "core/datasource/pointtypes.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/spatialtile.h"

#include <laszip.hpp>
#include <cassert>
#include <optional>
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

  // \todo support tile range
  Pointcloud(size_t pointCapacity, FieldGroup fieldGroup);
  //Pointcloud(const SpatialTile& tile, size_t pointCapacity, FieldGroup fieldGroup);
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
  void SetPointCount(const size_t pointCount);

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
  size_t GetBufferNumBytes() const noexcept;

  /**
   *  \brief set origin
   *
   *  \param [in] origin origin point
   */
  void SetOrigin(const Point3d& origin);

  /**
   *  \brief set an array of field objects. For XYZ coordinates, their values have been
   *  shifted w.r.t. the origin.
   *
   *  \param [in] fields an array of FieldTy objects
   */
  template <typename FieldTy> void SetField(std::vector<FieldTy>&& fields);

  /**
   *  \brief set a field object at specified index.
   *  For XYZ coordinates, their values have been shifted w.r.t. the origin
   *
   *  \param [in] i index of point
   *  \param [in] field the field object.
   */
  template <typename FieldTy> void SetField(size_t i, const FieldTy& field);

  /**
   *  \brief set XYZ coordinates at specified index
   *
   *  \param [in] i index of point
   *  \param [in] xyz XYZ coordinates
   */
  void SetXYZ(size_t i, const Point3f& xyz);

  void SetXYZ(size_t i, const XYZD& xyz);
  void SetXYZ(size_t i, const Point3d& xyz);

  /**
   *  \return Returns origin.
   */
  const Point3d& GetOrigin() const noexcept { return mOrigin; }
  Point3d& GetOrigin() noexcept { return mOrigin; }

  /**
   *  \return Returns the array of field objects. XYZ coordinates are w.r.t. the origin.
   */
  template <typename FieldTy> const std::vector<FieldTy>& GetField() const;
  template <typename FieldTy> std::vector<FieldTy>& GetField();

  /**
 *  \param [in] i index of point
 *  \return Returns XYZ coordinates at specified index.
 */
  XYZD GetXYZD(size_t i) noexcept;
  Point3d GetPoint3d(size_t i) noexcept;

  /**
   *  \param FieldTy the field type of a WxPoint<> component
   *  \param [in] i index of point
   *  \return Returns a field at given index.
   */
  template <typename FieldTy> const FieldTy& GetField(size_t i) const;

  template <typename FieldTy> FieldTy& GetField(size_t i);

  /**
   *  \brief Set fields to given point.
   *
   *  \param PointTy the point type of WxPoint<>, being all or a partial of available fields
   *  \param [in] i index of point
   *  \param [in] i point object of type PointTy
   */
  template <typename PointTy> void SetPoint(size_t i, const PointTy& point);

  /**
   *  \brief Set fields to given point.
   *
   *  \param [in] i index of point
   *  \param [in] point LASlib point
   */
  void SetPoint(size_t i, const LASpoint& point, const LASquantizer* quantizer);

  /**
   *  \param PointTy the point type of WxPoint<>, being all or a partial of available fields
   *  \param [in] i index of point
   *  \return Returns a point at given index.
   */
  template <typename PointTy> PointTy GetPoint(size_t i) const;

  /**
   *  \param FieldTy the field type of a WxPoint<> component
   *  \param [in] fields an array of FieldTy objects
   */
  template <typename FieldTy> void SetField_Unchecked(std::vector<FieldTy>&& fields);

  /**
   *  \param FieldTy the field type of a WxPoint<> component
   *  \return Return a reference to the array of FieldTy objects
   */
  template <typename FieldTy> const std::vector<FieldTy>& GetField_Unchecked() const;

  template <typename FieldTy> std::vector<FieldTy>& GetField_Unchecked();

  /**
   *  \param FieldTy the field type of a WxPoint<> component
   *  \param [in] i index of point
   *  \param [in] field the FieldTy object
   */
  template <typename FieldTy>
  void SetField_Unchecked(size_t i, const FieldTy& field);

  /**
   *  \param FieldTy the field type of a WxPoint<> component
   *  \param [in] i index of point
   *  \return Returns a field at given index.
   */
  template <typename FieldTy> const FieldTy& GetField_Unchecked(size_t i) const;

  template <typename FieldTy> FieldTy& GetField_Unchecked(size_t i);

  /**
   *  \brief Set fields to given point.
   *
   *  \param [in] i index of point
   *  \param PointTy the point type of WxPoint<>
   */
  template <typename PointTy>
  void SetPoint_Unchecked(size_t i, const PointTy& point);

  /**
   *  \param [in] i index of point
   *  \return Returns a point at given index.
   */
  template <typename PointTy> PointTy GetPoint_Unchecked(size_t i) const;

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
    static_assert(std::is_same_v<Pointcloud, std::remove_cv_t<PointcloudTy>>, "Pointcloud");
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
  SpatialTileD GetBufTile();

  void Swap(Pointcloud& rhs) noexcept;

private:
  SpatialTileD mTile;   // 2D bounding box of focusing points within range of AOI
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
  void VerifyPointFieldBits(size_t pointFieldBits) const;

  /**
   *  \param FieldTy the field type of a WxPoint<> component
   *  \throw Throws invalid_argument if point cloud does not contain the requested field group.
   */
  template <typename FieldTy> void VerifyFieldType() const;

  // cache
  size_t mPointCount{};     // actual number of points held, default to mPointCapacity
  std::optional<SpatialTileD> mBufTile;   // 2D bounding box of all points including focused & buffered
  std::tuple<std::vector<XYZF>*, std::vector<Classification>*,
    std::vector<Intensity>*, std::vector<RGB16>*> mData{
    &mXYZ, &mCla, &mIntensity, &mRGB }; // indexed by kPOS
};

using UPPointcloud = std::unique_ptr<Pointcloud>;
using SPPointcloud = std::shared_ptr<Pointcloud>;

#include "detail/pointcloud.hh"
}

// overload std::swap
namespace std {
inline void swap(earthxpert::Pointcloud& lhs,
  earthxpert::Pointcloud& rhs) noexcept { lhs.Swap(rhs); }
}
