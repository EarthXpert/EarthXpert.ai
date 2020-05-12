/* ----------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/math/interpcube.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <type_traits>

using watxtract::InterpCube;

/**
 * Constructor
 *
 * \param [in] dim Dimension of points for the cube
 * \param [in] min Minimum XYZ-coordinate to cover
 * \param [in] max Maximum XYZ-coordinate to cover
 * \param [in] spc XYZ spacing of cube points
 */
template <typename Ty>
InterpCube<Ty>::InterpCube(const unsigned int dim, const cv::Point3_<Ty>& min,
  const cv::Point3_<Ty>& max, const cv::Point3_<Ty>& spc)
  : InterpCube(dim, min.x, min.y, min.z, max.x, max.y, max.z, spc.x, spc.y,spc.z) {
}

/**
 * Constructor
 *
 * \param [in] dim Dimension of points for the cube
 * \param [in] minX Minimum X-coordinate to cover
 * \param [in] minY Minimum Y-coordinate to cover
 * \param [in] minZ Minimum Z-coordinate to cover
 * \param [in] maxX Maximum X-coordinate to cover
 * \param [in] maxXY Maximum Y-coordinate to cover
 * \param [in] maxXZ Maximum Z-coordinate to cover
 * \param [in] spcX X spacing of cube points
 * \param [in] spcY Y spacing of cube points
 * \param [in] spcZ Z spacing of cube points
 */
template <typename Ty>
InterpCube<Ty>::InterpCube(const unsigned int dim, const Ty minX,
  const Ty minY, const Ty minZ, const Ty maxX, const Ty maxY,
  const Ty maxZ, const Ty spcX, const Ty spcY, const Ty spcZ)
  : mDim(dim) {
  static_assert(std::is_floating_point_v<Ty>);
  LayoutNodes(minX, minY, minZ, maxX, maxX, maxZ, spcX, spcY, spcZ);
}

template <typename Ty> InterpCube<Ty>::InterpCube(const InterpCube &rhs) {}

template <typename Ty>
InterpCube<Ty>::InterpCube(InterpCube &&rhs) noexcept {}

template <typename Ty>
InterpCube<Ty> &InterpCube<Ty>::operator=(const InterpCube &rhs) {
  return *this;
}

template <typename Ty>
InterpCube<Ty> &InterpCube<Ty>::operator=(InterpCube &&rhs) noexcept {
  return *this;
}

template <typename Ty>
bool InterpCube<Ty>::operator==(const InterpCube &rhs) const {
  return true;
}

template <typename Ty>
bool InterpCube<Ty>::operator!=(const InterpCube &rhs) const {
  return !(operator==(rhs));
}

/**
 * Allocate nodes large enough to cover the cube at the provided spacing.
 *
 * \param [in] min Minimum XYZ-coordinate to cover
 * \param [in] max Maximum XYZ-coordinate to cover
 * \param [in] spc XYZ spacing of cube points
 */
template <typename Ty>
void InterpCube<Ty>::LayoutNodes(const cv::Point3_<Ty>& min,
  const cv::Point3_<Ty>& max, const cv::Point3_<Ty>& spc) {
  LayoutNodes(min.x, min.y, min.z, max.x, max.y, max.z, spc.x, spc.y, spc.z);
}

/**
 * Allocate nodes large enough to cover the cube at the provided spacing.
 *
 * \param [in] minX Minimum X-coordinate to cover
 * \param [in] minY Minimum Y-coordinate to cover
 * \param [in] minZ Minimum Z-coordinate to cover
 * \param [in] maxX Maximum X-coordinate to cover
 * \param [in] maxXY Maximum Y-coordinate to cover
 * \param [in] maxXZ Maximum Z-coordinate to cover
 * \param [in] spcX X spacing of cube points
 * \param [in] spcY Y spacing of cube points
 * \param [in] spcZ Z spacing of cube points
 */
template <typename Ty>
void InterpCube<Ty>::LayoutNodes(const Ty minX,
  const Ty minY, const Ty minZ, const Ty maxX, const Ty maxY,
  const Ty maxZ, const Ty spcX, const Ty spcY, const Ty spcZ) {
}

/**
 * Populate an array with the X,Y,Z positions of the node points defining the
 * cube.
 *
 * \param [in/out] nodes Array of point X,Y,Z coordinates.
 *      The array must be allocated by the caller.
 * \remark The ordering of the cube nodes is undefined.  The user
 *      need only know that these nodes and the values returned by
 *      GetArray() are provided in the same order.
 */
template <typename Ty>
void InterpCube<Ty>::GetNodes(cv::Point3_<Ty> *const node) const {
}

template <typename Ty>
void InterpCube<Ty>::GetNodes(Ty *const nodes) const {
}

/**
 * Interpolate a value for the given X,Y position.
 *
 * \param [in] X X-position
 * \param [in] Y Y-position
 * \param [in,out] val Array of interpolated (or extrapolated) values
 */
template <typename Ty>
void InterpCube<Ty>::GetVal(const Ty X, const Ty Y, const Ty Z, Ty *const val) const {
}

/**
 * Copy values from another object
 *
 * \param [in] rhs Object to copy from
 */
template <typename Ty>
void InterpCube<Ty>::CopyFrom(const InterpCube<Ty> &rhs) {
  mDim = rhs.mDim;
  mNumX = rhs.mNumX;
  mNumY = rhs.mNumY;
  mOffX = rhs.mOffX;
  mOffY = rhs.mOffY;
  mSpcX = rhs.mSpcX;
  mSpcY = rhs.mSpcY;
  mCube = rhs.mCube;
}

/**
 * Move values from another object
 *
 * \param [in] rhs Object to move from
 */
template <typename Ty> void InterpCube<Ty>::MoveFrom(InterpCube<Ty> &&rhs) noexcept {
  mDim = rhs.mDim;
  mNumX = rhs.mNumX;
  mNumY = rhs.mNumY;
  mNumZ = rhs.mNumZ;
  mOffX = rhs.mOffX;
  mOffY = rhs.mOffY;
  mOffZ = rhs.mOffZ;
  mSpcX = rhs.mSpcX;
  mSpcY = rhs.mSpcY;
  mSpcZ = rhs.mSpcZ;
  mCube = std::move(rhs.mCube);

  rhs.Clear();
}

template <typename Ty> void InterpCube<Ty>::Clear() noexcept {
  mDim = 1;
  mNumX = 0;
  mNumY = 0;
  mNumZ = 0;
  mOffX = 0;
  mOffY = 0;
  mOffZ = 0;
  mSpcX = 1;
  mSpcY = 1;
  mSpcZ = 1;
}

template class InterpCube<float>;
template class InterpCube<double>;
