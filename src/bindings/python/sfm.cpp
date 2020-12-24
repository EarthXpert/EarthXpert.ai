/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "bindings/python/expyext.h"
#include "bindings/python/sfm.h"

namespace pb = pybind11;
//using namespace earthxpert;

EXPY_SUBMODULE(sfm, m) {
  m.doc() = R"pbdoc(
   Structure from motion module for python
    -------------------------------

       // data types

       //structures

       //classes

       //free functions
  )pbdoc";

  // python module definitions
#include "detail/sfm_m.hpp"
}
