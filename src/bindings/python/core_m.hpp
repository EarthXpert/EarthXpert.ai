/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
//#include "core/spatialref/earthellipsoid.h"
#include "detail/datasource_m.hpp"
#include "detail/exceptions_m.hpp"

  // data types

  //structures

  // classes
  pb::class_<EarthEllipsoid>(m, "EarthEllipsoid")
  .def(pb::init<double, double>(), pb::arg("semiMajor") = kWGS84_A, pb::arg("eccSq") = kWGS84_ECCSQ)
  .def("clone", &EarthEllipsoid::Clone)
  .def("lphToXYZ", pb::overload_cast<const Point3d&>(&EarthEllipsoid::LPHToXYZ, pb::const_))
  .def("lphToXYZ", pb::overload_cast<double, double, double>(&EarthEllipsoid::LPHToXYZ, pb::const_))
  .def("xyzToLPH", pb::overload_cast<const Point3d&>(&EarthEllipsoid::XYZToLPH, pb::const_))
  .def("xyzToLPH", pb::overload_cast<double, double, double>(&EarthEllipsoid::XYZToLPH, pb::const_))
  .def("lsrToXYZ", pb::overload_cast<const Point3d&>(&EarthEllipsoid::LSRToXYZ, pb::const_))
  .def("lsrToXYZ", pb::overload_cast<double, double, double>(&EarthEllipsoid::LSRToXYZ, pb::const_))
  .def("xyzToLSR", pb::overload_cast<const Point3d&>(&EarthEllipsoid::XYZToLSR, pb::const_))
  .def("xyzToLSR", pb::overload_cast<double, double, double>(&EarthEllipsoid::XYZToLSR, pb::const_))
  .def("lsrToLPH", pb::overload_cast<const Point3d&>(&EarthEllipsoid::LSRToLPH, pb::const_))
  .def("lsrToLPH", pb::overload_cast<double, double, double>(&EarthEllipsoid::LSRToLPH, pb::const_))
  .def("lphToLSR", pb::overload_cast<const Point3d&>(&EarthEllipsoid::LPHToLSR, pb::const_))
  .def("lphToLSR", pb::overload_cast<double, double, double>(&EarthEllipsoid::LPHToLSR, pb::const_))
  .def("setLsrOrigin", &EarthEllipsoid::SetLsrOrigin)
  .def("getLsrOrigin", &EarthEllipsoid::GetLsrOrigin);

  // free functions
