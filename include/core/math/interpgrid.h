/* ----------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "core/utils/typehelper.h"
#include "3rd_party/opencv/exopencv.h"

#include <cassert>
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
 * \param _Ty A fundamental data type of elements on the grid
 * \remark The 2D coordinate system
 */
template <typename _Ty> class EXCORE_EXPORTS InterpGrid {
  static_assert(std::is_floating_point_v<_Ty>, "Requires that template argument be floating data type");

public:
  using ValueType = _Ty;

  InterpGrid(unsigned int dim, _Ty minX, _Ty minY, _Ty maxX, _Ty maxY, _Ty spcX,
             _Ty spcY);
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
   * Set the dimension of points that are stored in the grid.
   *
   * \param [in] dim New dimension of points for the grid
   */
  void SetDimension(const unsigned int dim) { mDim = dim; }

  void LayoutNodes(_Ty minX, _Ty minY, _Ty maxX, _Ty maxY, _Ty spcX,
                       _Ty spcY);

  /**
   * Interpolate a value for the given X,Y position.
   */
  void GetVal(cv::Point_<_Ty> point, _Ty *const val) const {
    GetVal(point.x, point.y, val);
  }
  void GetVal(_Ty X, _Ty Y, _Ty *const val) const;

  // \return Return value of the 1st dimension
  _Ty GetVal1(cv::Point_<_Ty> point) const {
    return GetVal1(point.x, point.y);
  }
  _Ty GetVal1(_Ty X, _Ty Y) const
  {
    assert(1 <= mDim);
    _Ty val;
    GetVal(X, Y, &val);
    return val;
  }

  // \return Return values of the beginning two dimensions
  std::pair<_Ty, _Ty> GetVal2(cv::Point_<_Ty> point) const {
    return GetVal2(point.x, point.y);
  }
  std::pair<_Ty, _Ty> GetVal2(_Ty X, _Ty Y) const
  {
    assert(2 <= mDim);
    _Ty val[2];
    GetVal(X, Y, val);
    return {val[0], val[1]};
  }

  // \return Return values of the beginning three dimensions
  Tuple3<_Ty> GetVal3(cv::Point_<_Ty> point) const {
    return GetVal3(point.x, point.y);
  }
  Tuple3<_Ty> GetVal3(_Ty X, _Ty Y) const
  {
    assert(3 <= mDim);
    _Ty val[3];
    GetVal(X, Y, val);
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
  std::pair<_Ty, _Ty> GetOffsets() const noexcept {
    return { mOffX, mOffY };
  }

  /**
   * Get the X- and Y-spacing that are used to compute indices within the
   * interpolation grid.
   *
   * \return Returns {X spacing, Y spacing}
   */
  std::pair<_Ty, _Ty> GetSpacing(_Ty &spcX, _Ty &spcY) const noexcept {
    return { mSpcX, mSpcY };
  }

  void GetNodes(cv::Point_<_Ty> *const gridNodes) const;
  void GetNodes(_Ty *const gridNodes) const;

  /**
   * Get a pointer to the array storing the values for the grid points.  They
   * are stored in the same order as the node points populated by
   * GetNodes().
   *
   * return Pointer to data array
   */
  _Ty *GetArray() noexcept { return mGrid.data(); }

  const _Ty *GetArray() const noexcept { return mGrid.data(); }

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
  std::pair<unsigned int, unsigned int> GetSize() const {
    return { mNumX, mNumY };
  }

  /**
   * Get the size of the data array in bytes.
   *
   * \return Returns size of the data array in bytes.
   */
  size_t GetSizeBytes() const {
    return sizeof(_Ty) * mNumX * mNumY * mDim;
  }

private:
  unsigned int mDim{1};   // Dimension of node points stored in the grid.
  unsigned int mNumX{0};  // Number of grid points in the X-dimension
  unsigned int mNumY{0};  // Number of grid points in the Y-dimension
  _Ty mOffX{0};           // X coordinate offset
  _Ty mOffY{0};           // Y coordinate offset
  _Ty mSpcX{1};           // X coordinate spacing of grid points
  _Ty mSpcY{1};           // Y coordinate spacing of grid points
  std::vector<_Ty> mGrid; // Array of target coordinates for node points

  void CopyFrom(const InterpGrid &rhs);
  void MoveFrom(InterpGrid &&rhs) noexcept;
  void Clear() noexcept;

  // cached temporaries for performance
  _Ty mInvSpcX{1};
  _Ty mInvSpcY{1};
};

using InterpGridF = InterpGrid<float>;
using InterpGridD = InterpGrid<double>;
// using InterpGridLD = InterpGrid<long double>;
} // namespace watxtract
