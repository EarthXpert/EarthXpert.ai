/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"
#include <vector>

class EX3RDPARTY_EXPORTS BitVector
{
public:
  BitVector() = default;
  explicit BitVector(std::vector<bool> const& bv) : bv(bv) {}

  bool operator[](size_t const i) { return bv[i]; }

  inline bool any() const {
    for (auto b : bv) if (b) return true;
    return false;
  }

  inline bool all() const {
    for (auto b : bv) if (!b) return false;
    return true;
  }

  inline bool none() const { return !any(); }

  inline size_t count() const {
    return std::count(bv.cbegin(), bv.cend(), true);
  }

  inline size_t size() const { return bv.size(); }

  inline BitVector& add(bool const value) {
    bv.push_back(value);
    return *this;
  }

  inline BitVector& remove(size_t const index) {
    if (index >= bv.size())
      throw std::out_of_range("Index out of range");
    bv.erase(bv.begin() + index);
    return *this;
  }

  inline BitVector& set(bool const value = true) {
    for (size_t i = 0; i < bv.size(); ++i)
      bv[i] = value;
    return *this;
  }

  inline BitVector& set(size_t const index, bool const value = true) {
    if (index >= bv.size())
      throw std::out_of_range("Index out of range");
    bv[index] = value;
    return *this;
  }

  inline BitVector& reset() {
    for (size_t i = 0; i < bv.size(); ++i) bv[i] = false;
    return *this;
  }

  inline BitVector& reset(size_t const index) {
    if (index >= bv.size())
      throw std::out_of_range("Index out of range");
    bv[index] = false;
    return *this;
  }

  inline BitVector& flip() {
    bv.flip();
    return *this;
  }

  std::vector<bool>& data() { return bv; }

private:
  std::vector<bool> bv;
};
