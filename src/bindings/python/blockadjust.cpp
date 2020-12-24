/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "bindings/python/expyext.h"
#include "bindings/python/blockadjust.h"

namespace pb = pybind11;
using namespace earthxpert;

EXPY_SUBMODULE(blockadjust, m) {
  m.doc() = R"pbdoc(
   Block adjustment module for python
    -------------------------------

       // data types

       //structures

       //classes

       //free functions
  )pbdoc";

  // python module definitions
  #include "detail/blockadjust_m.hpp"
}
