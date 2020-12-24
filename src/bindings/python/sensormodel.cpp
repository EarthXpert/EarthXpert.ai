/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "bindings/python/expyext.h"
#include "bindings/python/sensormodel.h"

namespace pb = pybind11;
using namespace earthxpert;

EXPY_SUBMODULE(sensormodel, m) {
  m.doc() = R"pbdoc(
    Sensor models module for python
    -------------------------------

       // data types

       //structures

       //classes

       //free functions
  )pbdoc";

  // python module definitions
  #include "detail/sensormodel_m.hpp"
}
