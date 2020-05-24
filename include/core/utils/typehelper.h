/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include <limits>
#include <type_traits>
#include <tuple>

namespace earthxpert {
using Tuple3d = std::tuple<double, double, double>;
using Tuple4d = std::tuple<double, double, double, double>;

template <class _Ty>
using Tuple3 = std::tuple<_Ty, _Ty, _Ty>;

/**
 *  Cast scoped enum type to underlying type.
 *
 *  \param _En the enum type
 *  \param [in] ev the scoped enum value
 */
template<typename _En>
    constexpr auto Enum(_En ev) {
  static_assert(std::is_enum_v<_En>, "Requires an enum type");
  return static_cast<std::underlying_type_t<_En>>(ev);
}

template <typename _Ty> struct Equal {
  _Ty tol{}; // tolerance to judge the equality

  Equal(const _Ty tolerance = std::numeric_limits<_Ty>::epsilon())
      : tol(tolerance) {}

  bool operator()(const _Ty lhs, const _Ty rhs) const {
    return (fabs(lhs - rhs) <= tol);
  }

  void Reset(const _Ty tolerance) { tol = tolerance; }
};

using EqualD = Equal<double>;
} // namespace earthxpert
