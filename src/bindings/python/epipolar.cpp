/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "bindings/python/expyext.h"
#include "bindings/python/epipolar.h"

namespace pb = pybind11;
using namespace earthxpert;

EXPY_SUBMODULE(epipolar, m) {
  m.doc() = R"pbdoc(
   Epipolar module for python
    -------------------------------

       // data types

       //structures

       //classes

       //free functions
  )pbdoc";

  // python module definitions
  #include "detail/epipolar_m.hpp"
}
