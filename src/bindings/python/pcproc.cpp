/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "bindings/python/expyext.h"
#include "bindings/python/pcproc.h"

namespace pb = pybind11;
using namespace earthxpert;

EXPY_SUBMODULE(pcproc, m) {
  m.doc() = R"pbdoc(
   Pointcloud processing module for python
    -------------------------------

       // data types

       //structures

       //classes

       //free functions
  )pbdoc";

  // python module definitions
#include "detail/pcproc_m.hpp"
}
