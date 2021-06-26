/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"

namespace earthxpert {
// vertex described by 2d coordinates
template <typename Ty>
struct DTVertex2 {
  Ty x{};
  Ty y{};
};
using DTVertex2f = DTVertex2<float>;
using DTVertex2d = DTVertex2<double>;

// vertex described by 3d coordinates
template <typename Ty>
struct DTVertex3 {
  Ty x{};
  Ty y{};
  Ty z{};
};
using DTVertex3f = DTVertex3<float>;
using DTVertex3d = DTVertex3<double>;

// edge described by start and end vertices Ids indexed into vertices array
struct EX3RDPARTY_EXPORTS DTEdge {   // Point2i
  int v0{}; // from vertex
  int v1{}; // to vertex
};

// triangle described by three edge Ids indexed into edges array
// or three vertex Ids indexed into vertices array
struct EX3RDPARTY_EXPORTS DTTriangle {   // Point3i
  int t0{}; // 1st edge/vertex
  int t1{}; // 2nd edge/vertex
  int t2{}; // 3rd edge/vertex
};
} // namespace earthxpert
