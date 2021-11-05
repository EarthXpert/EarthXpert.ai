/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"

#include <functional>
#include <tuple>
#include <utility>

namespace earthxpert::utils {
namespace detail {
  template<typename Output, typename... Inputs>
  void tempFunc(Output& output, Inputs const&... inputs) {
    // implementation of f
  }

  template<typename... InputsOutput, size_t... InputIndexes>
  void tempFunc(std::tuple<InputsOutput...> inputsOutput,
    std::index_sequence<InputIndexes...>) {
    constexpr auto OutputIndex = sizeof...(InputsOutput) - 1;
    tempFunc(std::get<OutputIndex>(inputsOutput), std::get<InputIndexes>(inputsOutput)...);
  }

  template<typename Array, std::size_t... Is>
  auto a2t_impl(const Array& a, std::index_sequence<Is...>) {
    return std::make_tuple(a[Is]...);
  }
}

// usage: tempFunc(input1, input2, input3, output);
template<typename... InputsOutput>
void tempFunc(InputsOutput&&... inputsOutput) {
  detail::tempFunc(std::forward_as_tuple(inputsOutput...), 
    std::make_index_sequence<sizeof...(inputsOutput) - 1>{});
}

// Convert array into a tuple
template<typename T, std::size_t N, typename Is = std::make_index_sequence<N>>
auto a2t(const std::array<T, N>& a) {
  return detail::a2t_impl(a, Is{});
}

template<typename Ty>
using UPDx = std::unique_ptr<Ty, std::function<void(Ty*)>>;

/**
 *  \brief a generic checked cast function that verifies our casts in debug mode
 *  and performs casts as fast as possible in release mode.
 *
 *  \tparam Ty1 source type
 *  \tparam Ty2 destination type
 */
template <typename Ty1, typename Ty2>
auto safe_cast(const Ty1& v) -> Ty2 {
  using namespace std;
  constexpr auto is_same_type = is_same_v<Ty1, Ty2>;
  constexpr auto is_pointer_to_pointer =
    is_pointer_v<Ty1> && is_pointer_v<Ty2>;
  constexpr auto is_float_to_float =
    is_floating_point_v<Ty1> && is_floating_point_v<Ty2>;
  constexpr auto is_number_to_number =
    is_arithmetic_v<Ty1> && is_arithmetic_v<Ty2>;
  constexpr auto is_intptr_to_ptr =
    (is_same_v<uintptr_t, Ty1> || is_same_v<intptr_t, Ty1>)
    && is_pointer_v<Ty2>;
  constexpr auto is_ptr_to_intptr =
    is_pointer_v<Ty1> &&
    (is_same_v<uintptr_t, Ty2> || is_same_v<intptr_t, Ty2>);

  if constexpr (is_same_type) {
    return v;
  }
  else if constexpr (is_intptr_to_ptr || is_ptr_to_intptr) {
    return reinterpret_cast<Ty2>(v);
  }
  else if constexpr (is_pointer_to_pointer) {
    assert(dynamic_cast<Ty2>(v) != nullptr);
    return static_cast<Ty2>(v);
  }
  else if constexpr (is_float_to_float) {
    auto casted = static_cast<Ty2>(v);
    auto casted_back = static_cast<Ty1>(v);
    assert(!isnan(casted_back) && !isinf(casted_back));
    return casted;
  }
  else if constexpr (is_number_to_number) {
    auto casted = static_cast<Ty2>(v);
    auto casted_back = static_cast<Ty1>(casted);
    assert(casted == casted_back);
    return casted;
  }
  else {
    static_assert(false, "CastError");
    return Ty2{}; // This can never happen,
  }
}
}
