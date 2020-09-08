/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include <limits>
#include <type_traits>
#include <tuple>

namespace earthxpert {
// sized tuple
template <int N, typename Head, typename... Ty>
struct Tuple {
  using type = typename Tuple<N - 1, Head, Head, Ty...>::type;
};

template <typename... Ty>
struct Tuple<1, Ty...> {
  using type = std::tuple<Ty...>;
};

template <typename Ty>
using Tuple3 = typename Tuple<3, Ty>::type;

template <typename Ty>
using Tuple4 = typename Tuple<4, Ty>::type;

using Tuple3d = typename Tuple<3, double>::type;
using Tuple4d = typename Tuple<4, double>::type;
using Tuple6d = typename Tuple<6, double>::type;

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

/**
 *  \tparam Ts callable types such as functor, lambda.
 */
template <typename ...Ts>
struct Overloaded : Ts... {
  using Ts::operator()...;
};
} // namespace earthxpert
