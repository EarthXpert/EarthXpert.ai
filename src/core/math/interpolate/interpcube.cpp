/* ----------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/math/interpcube.h"
#include "core/exceptions/exexcept.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <type_traits>

using earthxpert::InterpCube;

/**
 * Constructor
 *
 * \param [in] dim Dimension of points for the cube
 * \param [in] minX Minimum x-coordinate to cover
 * \param [in] minY Minimum y-coordinate to cover
 * \param [in] minZ Minimum z-coordinate to cover
 * \param [in] maxX Maximum x-coordinate to cover
 * \param [in] maxXY Maximum y-coordinate to cover
 * \param [in] maxXZ Maximum z-coordinate to cover
 * \param [in] spcX x spacing of cube points
 * \param [in] spcY y spacing of cube points
 * \param [in] spcZ z spacing of cube points
 */
template <typename Ty>
InterpCube<Ty>::InterpCube(const unsigned int dim, const Ty minX,
  const Ty minY, const Ty minZ, const Ty maxX, const Ty maxY,
  const Ty maxZ, const Ty spcX, const Ty spcY, const Ty spcZ) : mDim(dim) {
  LayoutNodes(minX, minY, minZ, maxX-minX, maxY-minY, maxZ-minZ, spcX, spcY, spcZ);
}

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
  const cv::Point3_<Ty>& max, const cv::Point3_<Ty>& spc) : mDim(dim) {
  LayoutNodes(min, max, spc);
}

template <typename Ty>
InterpCube<Ty>::InterpCube(const unsigned int dim, const SpatialCube<Ty>& range,
  const Ty spcX, const Ty spcY, const Ty spcZ) : mDim(dim) {
  LayoutNodes(range, spcX, spcY, spcZ);
}

template <typename Ty>
InterpCube<Ty>::InterpCube(const unsigned int dim, const SpatialCube<Ty>& range,
  const cv::Point3_<Ty>& spc) : mDim(dim) {
  LayoutNodes(range, spc.x, spc.y, spc.z);
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

/**
 * Allocate nodes large enough to cover the cube at the provided spacing.
 *
 * \param [in] minX Minimum x-coordinate 
 * \param [in] minY Minimum y-coordinate 
 * \param [in] minZ Minimum z-coordinate 
 * \param [in] legnth horizontal length to cover
 * \param [in] width horizontal width to cover
 * \param [in] height vertical extent to cover
 * \param [in] spcX x spacing of cube points
 * \param [in] spcY y spacing of cube points
 * \param [in] spcZ z spacing of cube points
 */
template <typename Ty>
void InterpCube<Ty>::LayoutNodes(const Ty minX,
  const Ty minY, const Ty minZ, const Ty length, const Ty width,
  const Ty height, const Ty spcX, const Ty spcY, const Ty spcZ) {
}

/**
 * \brief Populate an array with the x,y,z positions of the node points 
 * defining the cube.
 *
 * \param [in/out] nodes Array of point x,y,z coordinates allocated by the caller.
 */
template <typename Ty>
void InterpCube<Ty>::GetNodes(cv::Point3_<Ty> *const node) const {
}

template <typename Ty>
void InterpCube<Ty>::GetNodes(Ty *const nodes) const {
}

/**
 * Interpolate a value for the given x,y position.
 *
 * \param [in] x x-position
 * \param [in] y y-position
 * \param [in] z z-position
 * \param [in,out] v Array of interpolated (or extrapolated) values
 */
template <typename Ty>
void InterpCube<Ty>::GetVal(const Ty x, const Ty y, const Ty z, 
  Ty *const v, const uint32_t len) const {
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
  mNumZ = rhs.mNumZ;
  mOffX = rhs.mOffX;
  mOffY = rhs.mOffY;
  mOffZ = rhs.mOffZ;
  mSpcX = rhs.mSpcX;
  mSpcY = rhs.mSpcY;
  mSpcZ = rhs.mSpcZ;
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
