/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "detail/datasource.hpp"
#include "detail/exceptions.hpp"

  // data types

  //structures
  pb::class_<Point2d>(m, "Point2d")
    .def_readwrite("x", &Point2d::x)
    .def_readwrite("y", &Point2d::y);
  pb::class_<Point3d>(m, "Point3d")
    .def_readwrite("x", &Point3d::x)
    .def_readwrite("y", &Point3d::y)
    .def_readwrite("z", &Point3d::z);

  // classes

  // free functions
