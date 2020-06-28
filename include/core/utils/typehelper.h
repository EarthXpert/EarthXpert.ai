/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include <limits>
#include <type_traits>
#include <tuple>

namespace earthxpert {
template <class Ty>
using Tuple3 = std::tuple<Ty, Ty, Ty>;

using Tuple3d = Tuple3<double>;
using Tuple4d = std::tuple<double, double, double, double>;

/**
 *  Cast scoped enum type to underlying type.
 *
 *  \param _En the enum type
 *  \param [in] ev the scoped enum value
 */
template<typename En>
    constexpr auto Enum(En ev) {
  static_assert(std::is_enum_v<En>, "Requires an enum type");
  return static_cast<std::underlying_type_t<En>>(ev);
}

template <typename Ty> struct Equal {
  static_assert(std::is_arithmetic_v<Ty>, "Requires an arithmetic type");
  Ty tol{}; // tolerance to judge the equality

  Equal(const Ty tolerance = std::numeric_limits<Ty>::epsilon())
      : tol(tolerance) {}

  bool operator()(const Ty lhs, const Ty rhs) const {
    return (fabs(lhs - rhs) <= tol);
  }

  void Reset(const Ty tolerance) { tol = tolerance; }
};

using EqualD = Equal<double>;
} // namespace earthxpert
