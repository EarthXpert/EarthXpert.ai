/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

namespace earthxpert {
// Solver types
enum SolverType {
  eST_CERES = 0,
  eST_EIGEN = 1,
  eST_EXAI = 2
};

/**
 *  \brief enum LayoutKind specifies the major direction of tile division.
 */
enum class LayoutKind {
  eHORIZONTAL,
  eVERTICAL
};
}
