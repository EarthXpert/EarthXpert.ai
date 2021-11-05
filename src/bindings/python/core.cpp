/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "bindings/python/expyext.h"
#include "bindings/python/core.h"

namespace pb = pybind11;
using namespace earthxpert;

EXPY_SUBMODULE(core, m) {
  m.doc() = R"pbdoc(
   Core module for python
    -------------------------------

       // data types

       //structures

       //classes

       //free functions
  )pbdoc";

  // python module definitions
  #include "detail/core_m.hpp"
}
