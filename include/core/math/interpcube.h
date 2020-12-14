/* ----------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/utils/typehelper.h"
#include "core/utils/spatialcube.h"

#include <cassert>
#include <climits>
#include <memory>
#include <vector>
#include <utility>

namespace earthxpert {
/**
 * The class InterpCube stores a 3D grid and performs trilinear
 * interpolation to provide values at specified locations. If the
 * requested location falls outside the grid, it will extrapolate from
 * the nearest points.
 *
 * \param Ty A fundamental data type of elements on the cube
 */
template <typename Ty> class EXCORE_EXPORTS InterpCube {
  static_assert(std::is_floating_point_v<Ty>, "Requires that template argument be floating data type");

public:
  using ValueType = Ty;

  InterpCube(unsigned int dim, Ty minX, Ty minY, Ty minZ,
    Ty maxX, Ty maxY, Ty maxZ, Ty spcX, Ty spcY, Ty spcZ);
  InterpCube(unsigned int dim, const cv::Point3_<Ty>& min,
    const cv::Point3_<Ty>& max, const cv::Point3_<Ty>& spc);
  InterpCube(unsigned int dim, const SpatialCube<Ty>& range, 
    Ty spcX, Ty spcY, Ty spcZ);
  InterpCube(unsigned int dim, const SpatialCube<Ty>& range, 
    const cv::Point3_<Ty>& spc);
  InterpCube(const InterpCube &rhs);
  InterpCube(InterpCube &&rhs) noexcept;
  ~InterpCube() = default;

  InterpCube &operator=(const InterpCube &rhs);
  InterpCube &operator=(InterpCube &&rhs) noexcept;

  bool operator==(const InterpCube &rhs) const;
  bool operator!=(const InterpCube &rhs) const;

  std::unique_ptr<InterpCube> Clone() const {
    return std::make_unique<InterpCube>(*this);
  }

  /**
   * Interpolate a value for the given X,Y,Z position.
   */
  void GetVal(cv::Point3_<Ty> point, Ty *const val) const {
    GetVal(point.x, point.y, point.z, val);
  }
  void GetVal(Ty X, Ty Y, Ty Z, Ty *const val, uint32_t len=UINT_MAX) const;

  // \return Return value of the 1st dimension
  Ty GetVal1(cv::Point3_<Ty> point) const {
    return GetVal1(point.x, point.y, point.z);
  }
  Ty GetVal1(Ty X, Ty Y, Ty Z) const {
    assert(1 <= mDim);
    Ty val;
    GetVal(X, Y, Z, &val, 1);
    return val;
  }

  // \return Return values of the beginning two dimensions
  std::pair<Ty, Ty> GetVal2(cv::Point3_<Ty> point) const {
    return GetVal2(point.x, point.y, point.z);
  }
  std::pair<Ty, Ty> GetVal2(Ty X, Ty Y, Ty Z) const {
    assert(2 <= mDim);
    Ty val[2];
    GetVal(X, Y, Z, val, 2);
    return { val[0], val[1] };
  }

  // \return Return values of the beginning three dimensions
  Tuple3<Ty> GetVal3(cv::Point3_<Ty> point) const {
    return GetVal3(point.x, point.y, point.z);
  }
  Tuple3<Ty> GetVal3(Ty X, Ty Y, Ty Z) const {
    assert(3 <= mDim);
    Ty val[3];
    GetVal(X, Y, Z, val, 3);
    return { val[0], val[1], val[2] };
  }

  /**
   * Get the dimension of points stored in the cube
   *
   * \return Point dimension
   */
  unsigned int GetDimension() const noexcept { return mDim; }

  /**
   * Get the X- and Y-coordinate that are used to compute indices within the
   * interpolation cube.
   *
   * \return Returns {X-offset, Y-offset}
   * \remark The X grid index is computed as (X - offX) / spcX
   */
  std::tuple<Ty, Ty, Ty> GetOffsets() const noexcept {
    return { mOffX, mOffY, mOffZ };
  }

  void GetNodes(cv::Point3_<Ty> *const nodes) const;
  void GetNodes(Ty *const nodes) const;

  /**
   * Get a pointer to the array storing the values for the cube points.  They
   * are stored in the same order as the node points populated by
   * GetNodes().
   *
   * return Pointer to data array
   */
  Ty *GetArray() noexcept { return mCube.data(); }

  const Ty *GetArray() const noexcept { return mCube.data(); }

  /**
   * Get the number of node points defining the grid.
   *
   * \return Number of grid nodes
   */
  size_t GetNumNodes() const noexcept {
    return static_cast<size_t>(mNumX) * static_cast<size_t>(mNumY) *
      static_cast<size_t>(mNumZ);
  }

  /**
   * Get the number of cubic nodes
   *
   * \return Returns Number of cube nodes in the X,Y,Z directions
   */
  std::tuple<unsigned int, unsigned int, unsigned int> GetSize() const {
    return { mNumX, mNumY, mNumZ };
  }

  /**
   * Get the size of the data array [byte].
   *
   * \return Returns size of the data array in bytes.
   */
  size_t GetSizeBytes() const { return sizeof(Ty) * mNumX * mNumY * mNumZ * mDim; }

private:
  unsigned int mDim{ 1 };   // Dimension of node points stored in the cube.
  unsigned int mNumX{ 0 };  // Number of grid points in the X-dimension
  unsigned int mNumY{ 0 };  // Number of grid points in the Y-dimension
  unsigned int mNumZ{ 0 };  // Number of grid points in the Z-dimension
  Ty mOffX{ 0 };           // X coordinate offset
  Ty mOffY{ 0 };           // Y coordinate offset
  Ty mOffZ{ 0 };           // Z coordinate offset
  Ty mSpcX{ 1 };           // X coordinate spacing of cube points
  Ty mSpcY{ 1 };           // Y coordinate spacing of cube points
  Ty mSpcZ{ 1 };           // Z coordinate spacing of cube points
  std::vector<Ty> mCube; // Array of target coordinates for node points

  /**
   * Set the dimension of points that are stored in the grid.
   *
   * \param [in] dim New dimension of points for the grid
   */
  void SetDimension(const unsigned int dim) { mDim = dim; }

  void LayoutNodes(Ty minX, Ty minY, Ty minZ, 
    Ty length, Ty width, Ty height, Ty spcX, Ty spcY, Ty spcZ);

  /**
   * Allocate nodes large enough to cover the cube at the provided spacing.
   *
   * \param [in] min Minimum XYZ-coordinate to cover
   * \param [in] max Maximum XYZ-coordinate to cover
   * \param [in] spc XYZ spacing of cube points
   */
  inline void LayoutNodes(const cv::Point3_<Ty>& min,
    const cv::Point3_<Ty>& max, const cv::Point3_<Ty>& spc) {
    LayoutNodes(min.x, min.y, min.z, max.x - min.x, max.y - min.y,
      max.z - min.z, spc.x, spc.y, spc.z);
  }
  inline void LayoutNodes(const SpatialCube<Ty>& range, 
    const Ty spcX, const Ty spcY, const Ty spcZ) {
    LayoutNodes(range.minX, range.minY, range.minZ, 
      range.length, range.width, range.height, spcX, spcY, spcZ);
  }

  void CopyFrom(const InterpCube &rhs);
  void MoveFrom(InterpCube &&rhs) noexcept;
  void Clear() noexcept;

  // cached temporaries for performance
  Ty mInvSpcX{ 1 };
  Ty mInvSpcY{ 1 };
  Ty mInvSpcZ{ 1 };
};

using InterpCubeF = InterpCube<float>;
using InterpCubeD = InterpCube<double>;

#include "detail/interpcube.hh"
} // namespace earthxpert
