/* ----------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/math/interpgrid.h"
#include "core/math/math.h"
#include "core/utils/layoututils.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <type_traits>

using namespace earthxpert;

/**
 * Constructor
 *
 * \param [in] dim Dimension of points for the grid
 * \param [in] minX Minimum X-coordinate to cover
 * \param [in] minY Minimum Y-coordinate to cover
 * \param [in] maxX Maximum X-coordinate to cover
 * \param [in] maxY Maximum Y-coordinate to cover
 * \param [in] spcX X spacing of grid points
 * \param [in] spcY Y spacing of grid points
 */
template <typename Ty>
InterpGrid<Ty>::InterpGrid(const unsigned int dim, const Ty minX, const Ty minY, 
  const Ty maxX, const Ty maxY, const Ty spcX, const Ty spcY) : mDim(dim) {
  LayoutNodes(minX, minY, maxX-minX, maxY-minY, spcX, spcY);
}

template <typename Ty>
InterpGrid<Ty>::InterpGrid(unsigned int dim, const cv::Point_<Ty>& min,
  const cv::Point_<Ty>& max, const cv::Point_<Ty>& spc) {
  LayoutNodes(min, max, spc);
}

template <typename Ty>
InterpGrid<Ty>::InterpGrid(const unsigned int dim,
  const SpatialTile<Ty>& range, const Ty spcX, const Ty spcY) : mDim(dim) {
  LayoutNodes(range, spcX, spcY);
}

template <typename Ty>
InterpGrid<Ty>::InterpGrid(const unsigned int dim,
  const SpatialTile<Ty>& range, const cv::Point_<Ty>& spc) : mDim(dim) {
  LayoutNodes(range, spc.x, spc.y);
}

/**
 * Copy constructor
 *
 * \param [in] rhs Object to copy from
 */
template <typename Ty>
InterpGrid<Ty>::InterpGrid(const InterpGrid<Ty> &rhs) {
  CopyFrom(rhs);
}

/**
 * Move constructor
 *
 * \param [in] rhs Object to move from
 */
template <typename Ty>
InterpGrid<Ty>::InterpGrid(InterpGrid<Ty> &&rhs) noexcept {
  MoveFrom(std::move(rhs));
}

/**
 * Assignment operator
 *
 * \param [in] rhs Object to copy from
 * \return A reference to the newly modified object
 */
template <typename Ty>
InterpGrid<Ty> &InterpGrid<Ty>::operator=(const InterpGrid<Ty> &rhs) {
  if (this != &rhs) {
    CopyFrom(rhs);
  }
  return *this;
}

/**
 * Move assignment operator
 *
 * \param [in] rhs Object to move from
 * \return Returns a reference to the newly modified object
 */
template <typename Ty>
InterpGrid<Ty> &InterpGrid<Ty>::operator=(InterpGrid<Ty> &&rhs) noexcept {
  if (this != &rhs) {
    MoveFrom(std::move(rhs));
  }
  return *this;
}

/**
 * Equality operator
 *
 * \param [in] rhs Object to compare with
 * \return Returns true if same object or equal, false otherwise.
 */
template <typename Ty>
bool InterpGrid<Ty>::operator==(const InterpGrid<Ty> &rhs) const {
  if (this != &rhs) {
    if (mDim != rhs.mDim || mNumX != rhs.mNumX || mNumY != rhs.mNumY) {
      return false;
    }
    Equal<Ty> Eq(std::numeric_limits<Ty>::min());

    if (!Eq(mOffX, rhs.mOffX) || !Eq(mOffY, rhs.mOffY))
      return false;
    if (!Eq(mSpcX, rhs.mSpcX) || !Eq(mSpcY, rhs.mSpcY))
      return false;
    if (mGrid != rhs.mGrid)
      return false;
  }
  return true;
}

/**
 * Test inequality.
 *
 * \param [in] rhs Object to compare with
 * \return Returns true if not equal.
 */
template <typename Ty>
bool InterpGrid<Ty>::operator!=(const InterpGrid<Ty> &rhs) const {
  return !(operator==(rhs));
}

/**
 * Allocate nodes large enough to cover the grid at the provided spacing.
 *
 * \param [in] minX Minimum X-coordinate 
 * \param [in] minY Minimum Y-coordinate 
 * \param [in] width horizontal extent to cover
 * \param [in] height vertical extent to cover
 * \param [in] spcX X spacing of grid points
 * \param [in] spcY Y spacing of grid points
 */
template <typename Ty>
void InterpGrid<Ty>::LayoutNodes(const Ty minX, const Ty minY,
  const Ty width, const Ty height, const Ty spcX, const Ty spcY) {
  mSpcX = spcX;
  mSpcY = spcY;
  mOffX = minX;
  mOffY = minY;

  // Adjust spacing if it is zero.
  if (0 == mSpcX) {
    mSpcX = width / 4;
    if (0 == mSpcX)
      mSpcX = 1;
  }
  mInvSpcX = 1 / mSpcX;

  if (0 == mSpcY) {
    mSpcY = height / 4;
    if (0 == mSpcY)
      mSpcY = 1;
  }
  mInvSpcY = 1 / mSpcY;

  mNumX = static_cast<unsigned int>(width * mInvSpcX + 0.5) + 1;
  mNumY = static_cast<unsigned int>(height * mInvSpcY + 0.5) + 1;

  mNumX = std::max(mNumX, 2U);
  mNumY = std::max(mNumY, 2U);

  mGrid.resize(size_t(mDim) * size_t(mNumX) * size_t(mNumY));
}

/**
 * \brief Populate an array with the X & Y positions of the node points 
 * defining the grid.
 *
 * \param [in/out] nodes Array of point X,Y coordinates allocated by the caller.
 */
template <typename Ty>
void InterpGrid<Ty>::GetNodes(cv::Point_<Ty> *const nodes) const {
  assert(nodes);
  size_t k = 0;
  for (unsigned int i = 0; i < mNumX; ++i) {
    const Ty X = mOffX + i * mSpcX;
    for (unsigned int j = 0; j < mNumY; ++j, ++k) {
      const Ty Y = mOffY + j * mSpcY;

      nodes[k] = { X, Y };
    }
  }
}

template <typename Ty>
void InterpGrid<Ty>::GetNodes(Ty *const nodes) const {
  assert(nodes);
  size_t k = 0;
  for (unsigned int i = 0; i < mNumX; ++i) {
    const Ty X = mOffX + i * mSpcX;
    for (unsigned int j = 0; j < mNumY; ++j, ++k) {
      const Ty Y = mOffY + j * mSpcY;

      nodes[k * 2] = X;
      nodes[k * 2 + 1] = Y;
    }
  }
}

/**
 * Interpolate a value for the given X,Y position.
 *
 * \param [in] X X-position
 * \param [in] Y Y-position
 * \param [in,out] val Array of interpolated (or extrapolated) values
 */
template <typename Ty>
void InterpGrid<Ty>::GetVal(const Ty X, const Ty Y, 
  Ty *const val, const uint32_t len) const {
  assert(val);
  const Ty fIdxX = (X - mOffX) * mInvSpcX;
  const Ty fIdxY = (Y - mOffY) * mInvSpcY;

  int idxX = static_cast<int>(floor(fIdxX));
  int idxY = static_cast<int>(floor(fIdxY));
  idxX = std::min(std::max(0, idxX), int(mNumX) - 2);
  idxY = std::min(std::max(0, idxY), int(mNumY) - 2);

  // Fractional values for interpolation
  const Ty xf = fIdxX - idxX;
  const Ty yf = fIdxY - idxY;
  const auto [w1, w2, w3, w4] = LinearWeights<MatOrder::eCOLMAHOR>(xf, yf);
  const auto [i1, i2, i3, i4] = CornersIndex<MatOrder::eCOLMAHOR>(idxX, idxY, mNumY, mDim);

  // Interpolate the values.
  for (unsigned int i = 0; i < mDim && i < len; ++i) {
    val[i] = w1 * mGrid[i1 + i] + w2 * mGrid[i2 + i] +
             w3 * mGrid[i3 + i] + w4 * mGrid[i4 + i];
  }
}

/**
 * Copy values from another object
 *
 * \param [in] rhs Object to copy from
 */
template <typename Ty>
void InterpGrid<Ty>::CopyFrom(const InterpGrid<Ty> &rhs) {
  mDim = rhs.mDim;
  mNumX = rhs.mNumX;
  mNumY = rhs.mNumY;
  mOffX = rhs.mOffX;
  mOffY = rhs.mOffY;
  mSpcX = rhs.mSpcX;
  mSpcY = rhs.mSpcY;
  mGrid = rhs.mGrid;
}

/**
 * Move values from another object
 *
 * \param [in] rhs Object to move from
 */
template <typename Ty> void InterpGrid<Ty>::MoveFrom(InterpGrid<Ty> &&rhs) noexcept {
  mDim = rhs.mDim;
  mNumX = rhs.mNumX;
  mNumY = rhs.mNumY;
  mOffX = rhs.mOffX;
  mOffY = rhs.mOffY;
  mSpcX = rhs.mSpcX;
  mSpcY = rhs.mSpcY;
  mGrid = std::move(rhs.mGrid);

  rhs.Clear();
}

template <typename Ty> void InterpGrid<Ty>::Clear() noexcept {
  mDim = 1;
  mNumX = 0;
  mNumY = 0;
  mOffX = 0;
  mOffY = 0;
  mSpcX = 1;
  mSpcY = 1;
}

template class earthxpert::InterpGrid<float>;
template class earthxpert::InterpGrid<double>;
