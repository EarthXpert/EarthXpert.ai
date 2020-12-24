/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "bindings/python/expyext.h"
#include "bindings/python/imageproc.h"

namespace pb = pybind11;
//using namespace earthxpert;

EXPY_SUBMODULE(imageproc, m) {
  m.doc() = R"pbdoc(
   Image processing module for python
    -------------------------------

       // data types

       //structures

       //classes

       //free functions
  )pbdoc";

  // python module definitions
#include "detail/imageproc_m.hpp"
}
