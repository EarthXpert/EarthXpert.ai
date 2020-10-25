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
}
