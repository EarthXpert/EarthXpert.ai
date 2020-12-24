/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "bindings/python/expyext.h"  // python glue
#include "3rdparty/gdal/exgdal.h"

// expyext submodules
PyModuleList& ExpyModules() {
  static PyModuleList modules;
  return modules;
}

PYBIND11_MODULE(expyext, m) {
  m.doc() = R"pbdoc(
    EarthXpert package for python
    -----------------------------
  )pbdoc";

  // python module attributes
  m.attr("__version__") = "1.0.0";

  // initialize submodules
  for (const auto& subm : ExpyModules()) {
    subm(m);
  }

  earthxpert::GDALInitialize();   // register GDAL drivers
}

EXPY_SUBMODULE(expy, m) {
  m.doc() = R"pbdoc(
    Configuration & globals of EarthXpert package for python
    --------------------------------------------------------

       // data types

       //structures

       //classes

       //free functions
  )pbdoc";

  // python module definitions
}
