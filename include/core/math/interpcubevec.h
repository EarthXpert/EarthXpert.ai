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
 * The class InterpCubeVec stores a 3D grid and performs trilinear
 * interpolation to provide values at specified locations. If the
 * requested location falls outside the grid, it will extrapolate from
 * the nearest points.
 *
 * \param Ty A fundamental data type of elements on the cube
 * \param dim dimension of point
 */
template <typename Ty, int dim=1, typename = std::true_type> 
class EXCORE_EXPORTS InterpCubeVec {
  static_assert(std::is_floating_point_v<Ty>, "Requires that template argument be floating data type");

public:
  using ValueType = Ty;
  using VecType = cv::Vec<Ty, dim>;

  ~InterpCubeVec() = default;

private:
  std::vector<VecType> mCube; // Array of target coordinates for node points
};

// specialized for the cube having only a single 3D cell
template <typename Ty, int dim>
class EXCORE_EXPORTS InterpCubeVec<Ty, dim, std::false_type> {
  static_assert(std::is_floating_point_v<Ty>, "Requires that template argument be floating data type");

public:
  using ValueType = Ty;
  using VecType = cv::Vec<Ty, dim>;

  InterpCubeVec() {}
  ~InterpCubeVec() = default;

  /**
   *  \param Ty2 scalar type of x & y fractions
   *
   *  \param [in] xf x fraction
   *  \param [in] yf y fraction
   *  \param [in] zf z fraction
   *  \return Returns the interpolated point
   */
  template<typename Ty2>
  inline VecType GetVal(const Ty2 xf, const Ty2 yf, const Ty2 zf) const {
    return {};
  }

private:
  const VecType& mUlT;  // upper-left corner on top 
  const VecType& mUrT;  // upper-right corner
  const VecType& mLlT;  // lower-left corner
  const VecType& mLrT;  // lower-right corner
  const VecType& mUlB;  // upper-left corner on bottom 
  const VecType& mUrB;  // upper-right corner
  const VecType& mLlB;  // lower-left corner
  const VecType& mLrB;  // lower-right corner
};
}
