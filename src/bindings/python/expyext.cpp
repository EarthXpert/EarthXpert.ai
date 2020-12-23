/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
// EXAI APIs
#include "3rdparty/gdal/exgdal.h"
#include "blockadjust/adjustrational.h"
#include "core/spatialref/earthellipsoid.h"
#include "marshaller/sensormodel/rationalmodelmarshaller.h"
#include "project/rationalprj.h"
#include "rectification/epipolar/epipolar.h"
#include "sensormodel/rationalmodel.h"

// python glue
//#include <boost/python.hpp>
#include <boost/python/numpy/ndarray.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <xtensor-python/pyarray.hpp>
#include <xtensor-python/pytensor.hpp>
#include <xtensor-python/pyvectorize.hpp>

namespace pb = pybind11;
using namespace earthxpert;

// expyext python APIs
#include "utils.h"

// python wrapper definitions
#include "blockadjust.h"
#include "core.h"
#include "epipolar.h"
#include "imageproc.h"
#include "io.h"
#include "marshaller.h"
#include "pcproc.h"
#include "sensormodel.h"
#include "sfm.h"

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

  #include "detail/utils_m.hpp"

  // python module definitions
  #include "detail/blockadjust_m.hpp"
  #include "detail/core_m.hpp"
  #include "detail/epipolar_m.hpp"
  #include "detail/imageproc_m.hpp"
  #include "detail/io_m.hpp"
  #include "detail/marshaller_m.hpp"
  #include "detail/pcproc_m.hpp"
  #include "detail/sensormodel_m.hpp"
  #include "detail/sfm_m.hpp"

  // python module attributes
  m.attr("__version__") = "1.0.0";

  GDALInitialize();   // register GDAL drivers
}
