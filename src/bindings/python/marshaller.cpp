/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "bindings/python/expyext.h"
#include "bindings/python/marshaller.h"

namespace pb = pybind11;
using namespace earthxpert;

EXPY_SUBMODULE(marshaller, m) {
  m.doc() = R"pbdoc(
   Marshaller module for python
    -------------------------------

       // data types

       //structures

       //classes

       //free functions
  )pbdoc";

  // python module definitions
#include "detail/marshaller_m.hpp"
}
