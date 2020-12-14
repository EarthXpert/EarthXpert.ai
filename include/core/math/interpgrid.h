/* ----------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/utils/typehelper.h"
#include "core/utils/spatialtile.h"

#include <cassert>
#include <climits>
#include <memory>
#include <vector>
#include <utility>

namespace earthxpert {
/**
 * The class InterpGrid stores a 2D grid and performs bilinear
 * interpolation to provide values at specified locations. If the
 * requested location falls outside the grid, it will extrapolate from
 * the nearest points.
 *
 * \param Ty A fundamental data type of elements on the grid
 * \remark The 2D coordinate system
 */
template <typename Ty> class EXCORE_EXPORTS InterpGrid {
  static_assert(std::is_floating_point_v<Ty>, "Requires that template argument be floating data type");

public:
  using ValueType = Ty;

  InterpGrid(unsigned int dim, Ty minX, Ty minY, Ty maxX, Ty maxY, Ty spcX,
    Ty spcY);
  InterpGrid(unsigned int dim, const cv::Point_<Ty>& min,
    const cv::Point_<Ty>& max, const cv::Point_<Ty>& spc);
  InterpGrid(unsigned int dim, const SpatialTile<Ty>& range, Ty spcX, Ty spcY);
  InterpGrid(unsigned int dim, const SpatialTile<Ty>& range,
    const cv::Point_<Ty>& spc);
  InterpGrid(const InterpGrid &rhs);
  InterpGrid(InterpGrid &&rhs) noexcept;
  ~InterpGrid() = default;

  InterpGrid &operator=(const InterpGrid &rhs);
  InterpGrid &operator=(InterpGrid &&rhs) noexcept;

  bool operator==(const InterpGrid &rhs) const;
  bool operator!=(const InterpGrid &rhs) const;

  std::unique_ptr<InterpGrid> Clone() const {
    return std::make_unique<InterpGrid>(*this);
  }

  /**
   * Interpolate a value for the given X,Y position.
   */
  void GetVal(cv::Point_<Ty> point, Ty *const val) const {
    assert(val);
    GetVal(point.x, point.y, val);
  }
  void GetVal(Ty X, Ty Y, Ty *const val, uint32_t len=UINT_MAX) const;

  // \return Return value of the 1st dimension
  Ty GetVal1(cv::Point_<Ty> point) const {
    return GetVal1(point.x, point.y);
  }
  Ty GetVal1(Ty X, Ty Y) const {
    assert(1 <= mDim);
    Ty val;
    GetVal(X, Y, &val, 1);
    return val;
  }

  // \return Return values of the beginning two dimensions
  std::pair<Ty, Ty> GetVal2(cv::Point_<Ty> point) const {
    return GetVal2(point.x, point.y);
  }
  std::pair<Ty, Ty> GetVal2(Ty X, Ty Y) const {
    assert(2 <= mDim);
    Ty val[2];
    GetVal(X, Y, val, 2);
    return {val[0], val[1]};
  }

  // \return Return values of the beginning three dimensions
  Tuple3<Ty> GetVal3(cv::Point_<Ty> point) const {
    return GetVal3(point.x, point.y);
  }
  Tuple3<Ty> GetVal3(Ty X, Ty Y) const {
    assert(3 <= mDim);
    Ty val[3];
    GetVal(X, Y, val, 3);
    return { val[0], val[1], val[2] };
  }

  /**
   * Get the dimension of points stored in the grid
   *
   * \return Point dimension
   */
  unsigned int GetDimension() const noexcept { return mDim; }

  /**
   * Get the X- and Y-coordinate that are used to compute indices within the
   * interpolation grid.
   *
   * \return Returns {X-offset, Y-offset}
   * \remark The X grid index is computed as (X - offX) / spcX
   */
  std::pair<Ty, Ty> GetOffsets() const noexcept { return { mOffX, mOffY }; }

  /**
   * Get the X- and Y-spacing that are used to compute indices within the
   * interpolation grid.
   *
   * \return Returns {X spacing, Y spacing}
   */
  std::pair<Ty, Ty> GetSpacing(Ty &spcX, Ty &spcY) const noexcept {
    return { mSpcX, mSpcY };
  }

  void GetNodes(cv::Point_<Ty> *const gridNodes) const;
  void GetNodes(Ty *const gridNodes) const;

  /**
   * Get a pointer to the array storing the values for the grid points.  They
   * are stored in the same order as the node points populated by
   * GetNodes().
   *
   * return Pointer to data array
   */
  Ty *GetArray() noexcept { return mGrid.data(); }

  const Ty *GetArray() const noexcept { return mGrid.data(); }

  /**
   * Get the number of node points defining the grid.
   *
   * \return Number of grid nodes
   */
  size_t GetNumNodes() const noexcept {
    return static_cast<size_t>(mNumX) * static_cast<size_t>(mNumY);
  }

  /**
   * Get the number of grid nodes in the X and Y directions
   *
   * \return Returns Number of grid nodes in X & Y directions
   */
  std::pair<unsigned int, unsigned int> GetSize() const { return { mNumX, mNumY }; }

  /**
   * Get the size of the data array in bytes.
   *
   * \return Returns size of the data array in bytes.
   */
  size_t GetSizeBytes() const { return sizeof(Ty) * mNumX * mNumY * mDim; }

private:
  unsigned int mDim{1};   // Dimension of node points stored in the grid.
  unsigned int mNumX{0};  // Number of grid nodes in the X-dimension
  unsigned int mNumY{0};  // Number of grid nodes in the Y-dimension
  Ty mOffX{0};           // X coordinate offset
  Ty mOffY{0};           // Y coordinate offset
  Ty mSpcX{1};           // X coordinate spacing of grid points
  Ty mSpcY{1};           // Y coordinate spacing of grid points
  std::vector<Ty> mGrid; // Array of target coordinates for node points

  /**
   * Set the dimension of points that are stored in the grid.
   *
   * \param [in] dim New dimension of points for the grid
   */
  void SetDimension(const unsigned int dim) { mDim = dim; }

  void LayoutNodes(Ty minX, Ty minY, Ty width, Ty height, Ty spcX, Ty spcY);
  inline void LayoutNodes(const cv::Point_<Ty>& min, 
    const cv::Point_<Ty>& max, const cv::Point_<Ty>& spc) {
    LayoutNodes(min.x, min.y, max.x-min.x, max.y-min.y, spc.x, spc.y);
  }
  inline void LayoutNodes(const SpatialTile<Ty>& range, Ty spcX, Ty spcY) {
    LayoutNodes(range.minX, range.minY, range.width, range.height, spcX, spcY);
  }

  void CopyFrom(const InterpGrid &rhs);
  void MoveFrom(InterpGrid &&rhs) noexcept;
  void Clear() noexcept;

  // cached temporaries for performance
  Ty mInvSpcX{1};
  Ty mInvSpcY{1};
};

using InterpGridF = InterpGrid<float>;
using InterpGridD = InterpGrid<double>;
// using InterpGridLD = InterpGrid<long double>;

#include "detail/interpgrid.hh"
} // namespace earthxpert
