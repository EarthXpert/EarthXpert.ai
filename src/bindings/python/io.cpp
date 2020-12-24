/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "bindings/python/expyext.h"
#include "bindings/python/io.h"

namespace pb = pybind11;
using namespace earthxpert;

EXPY_SUBMODULE(io, m) {
  m.doc() = R"pbdoc(
   Data IO module for python
    -------------------------------

       // data types

       //structures

       //classes

       //free functions
  )pbdoc";

  // python module definitions
#include "detail/io_m.hpp"
}
