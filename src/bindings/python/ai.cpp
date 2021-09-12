/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "bindings/python/expyext.h"
#include "bindings/python/ai.h"

namespace pb = pybind11;
using namespace earthxpert;

EXPY_SUBMODULE(ai, m) {
  m.doc() = R"pbdoc(
   AI module for python
    -------------------------------

       // data types

       //structures

       //classes

       //free functions
  )pbdoc";

  // python module definitions
  #include "detail/ai_m.hpp"
}
