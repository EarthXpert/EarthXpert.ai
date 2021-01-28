/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

namespace schrono = std::chrono;

namespace earthxpert {
// print a 1D array
template<typename Ty, char delim = ' '>
std::ostream& operator<<(std::ostream& s, const std::vector<Ty>& v) {
  s.put('[');
  char comma[2] = { '\0', '\0' };
  for (const auto& e : v) {
    s << comma << e;
    comma[0] = delim;
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
inline std::vector<size_t> GetSortIndex(const std::vector<Ty>& v) {
  // initialize original index locations
  std::vector<size_t> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);

  // sort indexes based on comparing values in v
  std::sort(idx.begin(), idx.end(),
    [&v](const size_t i1, const size_t i2) { return v[i1] < v[i2]; });

  return idx;
}

template <typename Ty>
inline void SortUnique(std::vector<Ty>& v) {
  std::sort(v.begin(), v.end());
  std::erase(std::unique(v.begin(), v.end()), v.end());
}

/**
 *  Convert two vectors of the same length into one vector of pairs
 */
template<typename Ty1, typename Ty2 = Ty1> std::vector<std::pair<Ty1, Ty2>> 
inline MakeVecOfPairs(const std::vector<Ty1>& v1, const std::vector<Ty1>& v2) {
  assert(v1.size() = v2.size());
  std::vector<std::pair<Ty1, Ty2>> vp(v1.size());

  std::transform(v1.begin(), v1.end(), v2.begin(), vp.begin(),
    [](Ty1 a, Ty2 b) { return std::make_pair(a, b); });

  return vp;
}

template <class Ty>
using SP = std::shared_ptr<Ty>;

template<class Ty, class Deleter = std::default_delete<Ty>>
using UP = class std::unique_ptr<Ty, Deleter>;
} // namespace earthxpert
