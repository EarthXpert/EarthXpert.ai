/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"
#include <libInterpolate/Interpolate.hpp>
#include <libInterpolate/AnyInterpolator.hpp>

#include <functional>
#include <vector>

/**
 *  \todo 

      template<typename X> typename std::enable_if_t<!std::is_pointer<X>::value>
      setData(X&& x, X&& y); or
      setData(X&& x, X&& y, X&& z);
 */
namespace earthxpert {
//
enum InterpAlgo1D {
  eIA_Cubic,
  eIA_Linear,
  eIA_Monotonic
};

enum InterpAlgo2D {
  eIA_Bicubic,
  eIA_Bilinear,
  eIA_Tps       // thin plate spline
};

template<typename Ty>
using InterpFunc1D = std::function<double(double)>;
template<typename Ty>
using AnyInterp1D = _1D::AnyInterpolator<Ty, void(const std::vector<Ty>&, const std::vector<Ty>&)>;

template<typename Ty>
using InterpFunc2D = std::function<double(double, double)>;
template<typename Ty>
using AnyInterp2D = _2D::AnyInterpolator<Ty, void(const std::vector<Ty>&, const std::vector<Ty>&, const std::vector<Ty>&)>;

template<InterpAlgo1D interpFunc, typename Ty> InterpFunc1D<Ty> 
CreateInterpFunc(const std::vector<Ty>& x, const std::vector<Ty>& y) {
  InterpFunc1D<Ty> interp;

  if constexpr (eIA_Cubic == interpFunc) {
    interp = _1D::CubicSplineInterpolator<Ty>();
    interp.target<_1D::CubicSplineInterpolator<Ty>>()->setData(x, y);
  }
  else if constexpr (eIA_Linear == interpFunc) {
    interp = _1D::LinearInterpolator<Ty>();
    interp.target<_1D::LinearInterpolator<Ty>>()->setData(x, y);
  }
  else {  // eIA_Monotonic == interpFunc
    interp = _1D::MonotonicInterpolator<Ty>();
    interp.target<_1D::MonotonicInterpolator<Ty>>()->setData(x, y);
  }

  return interp;
}

template<InterpAlgo1D interpFunc, typename Ty> AnyInterp1D<Ty> 
CreateAnyInterp(const std::vector<Ty>& x, const std::vector<Ty>& y) {
  AnyInterp1D<Ty> interp;

  if constexpr (eIA_Cubic == interpFunc) {
    interp = _1D::CubicSplineInterpolator<Ty>();
  }
  else if constexpr (eIA_Linear == interpFunc) {
    interp = _1D::LinearInterpolator<Ty>();
  }
  else {  // eIA_Monotonic == interpFunc
    interp = _1D::MonotonicInterpolator<Ty>();
  }
  interp.setData(x, y);

  return interp;
}

template<InterpAlgo2D interpFunc, typename Ty> InterpFunc2D<Ty> 
CreateInterpFunc(const std::vector<Ty>& x, const std::vector<Ty>& y, const std::vector<Ty>& z) {
  InterpFunc2D<Ty> interp;

  if constexpr (eIA_Bicubic == interpFunc) {
    interp = _2D::CubicSplineInterpolator<Ty>();
    interp.target<_2D::BicubicInterpolator<Ty>>()->setData(x, y, z);
  }
  else if constexpr (eIA_Bilinear == interpFunc) {
    interp = _2D::LinearInterpolator<Ty>();
    interp.target<_2D::BilinearInterpolator<Ty>>()->setData(x, y, z);
  }
  else {  // eIA_Tps == interpFunc
    interp = _2D::MonotonicInterpolator<Ty>();
    interp.target<_2D::ThinPlateSplineInterpolator<Ty>>()->setData(x, y, z);
  }

  return interp;
}

template<InterpAlgo2D interpFunc, typename Ty> AnyInterp2D<Ty> 
CreateAnyInterp(const std::vector<Ty>& x, const std::vector<Ty>& y, const std::vector<Ty>& z) {
  AnyInterp2D<Ty> interp;

  if constexpr (eIA_Bicubic == interpFunc) {
    interp = _2D::BicubicInterpolator<Ty>();
  }
  else if constexpr (eIA_Bilinear == interpFunc) {
    interp = _2D::BilinearInterpolator<Ty>();
  }
  else {  // eIA_Tps == interpFunc
    interp = _2D::ThinPlateSplineInterpolator<Ty>();
  }
  interp.setData(x, y, z);

  return interp;
}
} // namespace earthxpert
