/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/math/math.h"

namespace earthxpert {
// Solver traits
struct SolverTraits {
  static constexpr size_t kPOSCERES = 0;
  static constexpr size_t kPOSCERESTINY = 1;
  static constexpr size_t kPOSEIGEN = 2;
  static constexpr size_t kPOSEXAI = 3;

  static constexpr size_t kBITCERES = exp2(kPOSCERES);
  static constexpr size_t kBITCERESTINY = exp2(kPOSCERESTINY);
  static constexpr size_t kBITEIGEN = exp2(kPOSEIGEN);
  static constexpr size_t kBITEXAI = exp2(kPOSEXAI);
};

/**
 *  \brief enum LayoutKind specifies the major direction of tile division.
 */
enum class LayoutKind {
  eHORIZONTAL,
  eVERTICAL
};
}
