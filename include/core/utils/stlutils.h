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

  template<typename... InputsThenOutput, size_t... InputIndexes>
  void tempFunc(std::tuple<InputsThenOutput...> inputsThenOutputs,
    std::index_sequence<InputIndexes...>) {
    auto constexpr OutputIndex = sizeof...(InputsThenOutput) - 1;
    tempFunc(std::get<OutputIndex>(inputsThenOutputs), std::get<InputIndexes>(inputsThenOutputs)...);
  }

  template<typename Array, std::size_t... I>
  auto a2t_impl(const Array& a, std::index_sequence<I...>) {
    return std::make_tuple(a[I]...);
  }
}

// usage: tempFunc(input1, input2, input3, output);
template<typename... InputsThenOutput>
void tempFunc(InputsThenOutput&&... inputsThenOutput) {
  detail::tempFunc(std::forward_as_tuple(inputsThenOutput...), 
    std::make_index_sequence<sizeof...(inputsThenOutput) - 1>{});
}

// Convert array into a tuple
template<typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
auto a2t(const std::array<T, N>& a) {
  return detail::a2t_impl(a, Indices{});
}

template<typename Ty>
using UPDx = std::unique_ptr<Ty, std::function<void(Ty*)>>;
}
