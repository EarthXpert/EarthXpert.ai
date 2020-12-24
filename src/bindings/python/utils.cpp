/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "bindings/python/expyext.h"
#include "bindings/python/utils.h"

namespace pb = pybind11;
//using namespace earthxpert;

EXPY_SUBMODULE(utils, m) {
  m.doc() = R"pbdoc(
    Utility module for python
    -------------------------

       // data types

       //structures

       //classes

       //free functions
  )pbdoc";

  #include "detail/utils_m.hpp"
}
