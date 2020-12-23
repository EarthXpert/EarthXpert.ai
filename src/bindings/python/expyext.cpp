/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gdal/exgdal.h"
#include "core/spatialref/earthellipsoid.h"
#include "marshaller/sensormodel/rationalmodelmarshaller.h"
#include "rectification/epipolar/epipolar.h"
#include "sensormodel/rationalmodel.h"

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

namespace pb = pybind11;
using namespace earthxpert;

// wrapper definitions
#include "core.hpp"
#include "epipolar.hpp"
#include "io.hpp"
#include "marshaller.hpp"
#include "sensormodel.hpp"

PYBIND11_MODULE(expyext, m) {
  m.doc() = R"pbdoc(
    WatXtract package for python
    ----------------------------

       // data types

       //structures
       Point3d

       //classes
       EarthEllipsoid

       //free functions
       generateFrameEpipolar
       generateRationalEpipolar
    )pbdoc";

  // module definitions
  #include "core_m.hpp"
  #include "epipolar_m.hpp"
  #include "io_m.hpp"
  #include "marshaller_m.hpp"
  #include "sensormodel_m.hpp"

  // module attributes
  m.attr("__version__") = "1.0.0";

  GDALInitialize();   // register GDAL drivers
}
