/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

namespace schrono = std::chrono;

namespace earthxpert {
// print a 1D array
template<typename Ty>
std::ostream& operator<<(std::ostream& s, const std::vector<Ty>& v) {
  s.put('[');
  char comma[3] = { '\0', ' ', '\0' };
  for (const auto& e : v) {
    s << comma << e;
    comma[0] = ',';
  }
  return s << ']';
}

/**
 *  \brief Get indexes of elements before having been sorted.
 *
 *  \return Returns indexes if being sorted
 *
 *  \remark using std::stable_sort instead of std::sort to avoid possible 
 *  index re-orderings of elements having equal values 
 */
template <typename Ty>
std::vector<size_t> GetSortIndex(const std::vector<Ty>& v) {
  // initialize original index locations
  std::vector<size_t> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);

  // sort indexes based on comparing values in v
  std::sort(idx.begin(), idx.end(),
    [&v](const size_t i1, const size_t i2) { return v[i1] < v[i2]; });

  return idx;
}

template <typename Ty>
void SortUnique(std::vector<Ty>& v) {
  std::sort(v.begin(), v.end());
  std::erase(std::unique(v.begin(), v.end()), v.end());
}

template <class Ty>
using SP = std::shared_ptr<Ty>;

template<class Ty, class Deleter = std::default_delete<Ty>>
using UP = class std::unique_ptr<Ty, Deleter>;
} // namespace earthxpert
