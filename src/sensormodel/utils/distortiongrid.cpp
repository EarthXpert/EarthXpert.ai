/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/distortiongrid.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <type_traits>

using earthxpert::DistortionGrid;

/**
 * Initializer constructor
 *
 * \param [in] cameraName name of camera
 * \param [in] vX X positions in grid
 * \param [in] vY Y positions in grid
 * \param [in] vdX X distortions in grid
 * \param [in] vdY Y distortions in grid
 */
template <typename Ty>
DistortionGrid<Ty>::DistortionGrid(std::string cameraName,
                                    const std::vector<Ty> &vX,
                                    const std::vector<Ty> &vY,
                                    const std::vector<Ty> &vdX,
                                    const std::vector<Ty> &vdY)
    : mCameraName(std::move(cameraName)) {
  static_assert(std::is_floating_point<Ty>::value);
  EstablishGrid(vX, vY, vdX, vdY);
}

/**
 * Initializer constructor
 *
 * \param [in] vX X positions in grid
 * \param [in] vY Y positions in grid
 * \param [in] vdX X distortions in grid
 * \param [in] vdY Y distortions in grid
 */
template <typename Ty>
DistortionGrid<Ty>::DistortionGrid(const std::vector<Ty> &vX,
                                    const std::vector<Ty> &vY,
                                    const std::vector<Ty> &vdX,
                                    const std::vector<Ty> &vdY) 
    : DistortionGrid("", vX, vY, vdX, vdY) {
}

/**
 * Copy constructor
 */
template <typename Ty>
DistortionGrid<Ty>::DistortionGrid(const DistortionGrid<Ty> &rhs) {
  CopyFrom(rhs);
}

/**
 * Move constructor
 */
template <typename Ty>
DistortionGrid<Ty>::DistortionGrid(DistortionGrid<Ty> &&rhs) noexcept {
  MoveFrom(std::move(rhs));
}

/**
 * Copy values from another object
 *
 * \param [in] rhs Object to copy from
 * \return A reference to the newly-modified object
 */
template <typename Ty>
DistortionGrid<Ty> &DistortionGrid<Ty>::
operator=(const DistortionGrid<Ty> &rhs) {
  if (this != &rhs) {
    CopyFrom(rhs);
  }
  return *this;
}

/**
 * Move values from another object
 *
 * \param [in] rhs Object to move from
 * \return A reference to the newly-modified object
 */
template <typename Ty>
DistortionGrid<Ty> &DistortionGrid<Ty>::
operator=(DistortionGrid<Ty> &&rhs) noexcept {
  if (this != &rhs) {
    MoveFrom(std::move(rhs));
  }
  return *this;
}

/**
 * Test equality.
 *
 * \param [in] rhs Object to compare with
 * \return true if same or equal.
 */
template <typename Ty>
bool DistortionGrid<Ty>::operator==(const DistortionGrid<Ty> &rhs) const {
  if (this != &rhs) {
    if (mCameraName != rhs.mCameraName)
      return false;
    if (!(mGrid) != !(rhs.mGrid))
      return false;
    if (bool(mGrid) && (*mGrid != *rhs.mGrid))
      return false;
  }
  return true;
}

/**
 * Test inequality.
 *
 * \param [in] rhs Object to compare with
 * \return true if not equal.
 */
template <typename Ty>
bool DistortionGrid<Ty>::operator!=(const DistortionGrid<Ty> &rhs) const {
  return !(operator==(rhs));
}

/**
 * Interpolate a distortion amount at the given X,Y position.
 *
 * \param [in] X X position
 * \param [in] Y Y position
 * \param [in,out] d Array of interpolated (or extrapolated) distortions
 */
template <typename Ty>
bool DistortionGrid<Ty>::GetDistortion(const Ty X, const Ty Y,
                                        Ty *const d) const {
  if (mGrid) {
    assert(d);
    mGrid->GetVal(X, Y, d);
    return true;
  }
  return false;
}

/**
 * Interpolate a distortion value for the given X,Y position.
 *
 * \param [in] X X position
 * \param [in] Y Y position
 * \return Returns interpolated (or extrapolated) X & Y distortions
 */
template <typename Ty>
std::pair<Ty, Ty> DistortionGrid<Ty>::GetDistortion(const Ty X, const Ty Y) const {
  if (mGrid) {
    assert(2 == mGrid->GetDimension());
    return mGrid->GetVal2(X, Y);
  }
  return {0.f, 0.f};
}

/**
 * Fetch the distortion grid.
 *
 * \param [out] numCols Number of columns
 * \param [out] numRows Number of rows
 * \param [out] vX X positions in grid
 * \param [out] vY Y positions in grid
 * \param [out] vdX X distortions in grid
 * \param [out] vdY Y distortions in grid
 * \return true if getting grid points successfully.
 */
template <typename Ty>
bool DistortionGrid<Ty>::FetchGrid(unsigned int &numCols,
                                    unsigned int &numRows, std::vector<Ty> &vX,
                                    std::vector<Ty> &vY, std::vector<Ty> &vdX,
                                    std::vector<Ty> &vdY) const {
  if (!mGrid)
    return false;
  std::tie(numCols, numRows) = mGrid->GetSize();

  const unsigned int dim = mGrid->GetDimension();
  if (2 != dim) {
    throw std::exception("Distortion grid must be 2D.");
  }

  const auto numPts = mGrid->GetNumNodes();

  vX.resize(numPts);
  vY.resize(numPts);
  vdX.resize(numPts);
  vdY.resize(numPts);

  std::vector<Ty> gridNodes(numPts * 2);
  mGrid->GetNodes(gridNodes.data());

  auto gridCorr = mGrid->GetArray();

  for (size_t k = 0; k < numPts; ++k) {
    const unsigned int X = static_cast<unsigned int>(k / numRows);
    const unsigned int Y = static_cast<unsigned int>(k % numRows);
    const size_t idx = size_t(Y) * size_t(numCols) + size_t(X);

    vX[idx] = gridNodes[k * 2];
    vY[idx] = gridNodes[k * 2 + 1];
    vdX[idx] = gridCorr[k * 2];
    vdY[idx] = gridCorr[k * 2 + 1];
  }

  return true;
}

/**
 * Establish the distortion grid.
 *
 * \param [in] vX X positions in grid
 * \param [in] vY Y positions in grid
 * \param [in] vdX X distortions in grid
 * \param [in] vdY Y distortions in grid
 */
template <typename Ty>
void DistortionGrid<Ty>::EstablishGrid(const std::vector<Ty> &vX,
                                        const std::vector<Ty> &vY,
                                        const std::vector<Ty> &vdX,
                                        const std::vector<Ty> &vdY) {
  const auto itX = std::minmax_element(std::begin(vX), std::end(vX));
  const Ty minX = *itX.first;
  const Ty maxX = *itX.second;

  const auto itY = std::minmax_element(std::begin(vY), std::end(vY));
  const Ty minY = *itY.first;
  const Ty maxY = *itY.second;

  const Ty spcX = fabs(vX[1] - vX[0]);

  auto it = std::adjacent_find(std::begin(vY), std::end(vY), std::less<Ty>());
  const Ty spcY = fabs(*it - *std::next(it));

  mGrid = std::make_unique< InterpGrid<Ty>>(2, minX, minY, maxX, maxY, spcX, spcY);
  auto grid = mGrid->GetArray();
  auto [numX, numY] = mGrid->GetSize();

  const size_t numPts = mGrid->GetNumNodes();

  for (size_t k = 0; k < numPts; ++k) {
    const unsigned int X = static_cast<unsigned int>(k / numY);
    const unsigned int Y = static_cast<unsigned int>(k % numY);

    const size_t idx = size_t(Y) * size_t(numX) + size_t(X);
    grid[k * 2] = vdX[idx];
    grid[k * 2 + 1] = vdY[idx];
  }
}

/**
 * Copy values from another object
 *
 * \param [in] rhs Object to copy from
 */
template <typename Ty>
void DistortionGrid<Ty>::CopyFrom(const DistortionGrid<Ty> &rhs) {
  mCameraName = rhs.mCameraName;
  mGrid = rhs.mGrid->Clone();
}

/**
 * Move values from another object
 *
 * \param [in] rhs Object to move from
 */
template <typename Ty>
void DistortionGrid<Ty>::MoveFrom(DistortionGrid<Ty> &&rhs) noexcept {
  mCameraName = std::move(rhs.mCameraName);
  mGrid = std::move(rhs.mGrid);
}

template class earthxpert::DistortionGrid<float>;
template class earthxpert::DistortionGrid<double>;
