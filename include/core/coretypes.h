/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

namespace earthxpert {
// Solver types
constexpr int kST_CERES = 0;
constexpr int kST_EIGEN = 1;
constexpr int kST_EXAI = 2;

/**
 *  \brief enum LayoutKind specifies the major direction of tile division.
 */
enum class LayoutKind {
  eHORIZONTAL,
  eVERTICAL
};
}
