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
#include <type_traits>
#include <vector>
#include <utility>

namespace earthxpert {
/**
 * The class InterpGridVec stores a 2D grid and performs bilinear
 * interpolation to provide values at specified locations. If the
 * requested location falls outside the grid, it will extrapolate from
 * the nearest points.
 *
 * \param Ty A fundamental data type of elements on the grid
 * \param dim dimension of point
 * \remark The 2D coordinate system
 */
template <typename Ty, int dim=1, typename=std::true_type> 
class EXCORE_EXPORTS InterpGridVec {
  static_assert(std::is_floating_point_v<Ty>, "Requires that template argument be floating data type");

public:
  using ValueType = Ty;
  using VecType = cv::Vec<Ty, dim>;

  ~InterpGridVec() = default;

private:
  std::vector<VecType> mGrid; // Array of target coordinates for node points
};

// specialized for the grid having only a single 2D cell
template <typename Ty, int dim> 
class EXCORE_EXPORTS InterpGridVec<Ty, dim, std::false_type> {
  static_assert(std::is_floating_point_v<Ty>, "Requires that template argument be floating data type");

public:
  using ValueType = Ty;
  using VecType = cv::Vec<Ty, dim>;

  InterpGridVec(const VecType& ul, const VecType& ur, const VecType& ll, 
    const VecType& lr) : mUl(ul), mUr(ur), mLl(ll), mLr(lr) {}
  InterpGridVec(const Tuple4<VecType&>& t4) : mUl(std::get<0>(t4)), 
    mUr(std::get<1>(t4)), mLl(std::get<2>(t4)), mLr(std::get<3>(t4)) {}
  ~InterpGridVec() = default;

  /**
   *  \param Ty2 scalar type of x & y fractions
   * 
   *  \param [in] xf x fraction
   *  \param [in] yf y fraction
   *  \return Returns the interpolated point
   * 
   *  \ref LinearInterp
   */
  template<typename Ty2>
  inline VecType GetVal(const Ty2 xf, const Ty2 yf) const {
    return (1 - yf) * ((1 - xf) * mUl + xf * mUr) +
      yf * ((1 - xf) * mLl + xf * mLr);
  }

private:
  const VecType& mUl;  // upper-left corner
  const VecType& mUr;  // upper-right corner
  const VecType& mLl;  // lower-left corner
  const VecType& mLr;  // lower-right corner
};
}
