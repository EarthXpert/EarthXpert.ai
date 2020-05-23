/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rd_party/gdal/wxgdal.h"
#include "core/spatialref/earthellipsoid.h"
#include "rectification/epipolar/epipolar.h"
#include "sensormodel/rationalmodel.h"

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

namespace pb = pybind11;
using namespace earthxpert;
using namespace earthxpert::epipolar;

// \todo all pure virtual
template <class Base = ISensorModel> class PyISensorModel : public Base {
public:
  using Base::Base;
  bool ImageToGround(const Point2d& image, Point3d& ground) const override {
    PYBIND11_OVERLOAD_PURE(bool, Base, ImageToGround, image, ground);
  }
  bool GroundToImage(const Point3d& ground, Point2d& image) const override {
    PYBIND11_OVERLOAD_PURE(bool, Base, GroundToImage, ground, image);
  }
};

template <class Base = SensorModel> class PySensorModel : public PyISensorModel<Base> {
public:
  using PyISensorModel<Base>::PyISensorModel;
  bool ImageToGround(const Point2d& image, Point3d& ground) const override {
    PYBIND11_OVERLOAD_PURE(bool, Base, ImageToGround, image, ground);
  }
  bool GroundToImage(const Point3d& ground, Point2d& image) const override {
    PYBIND11_OVERLOAD_PURE(bool, Base, GroundToImage, ground, image);
  }
};

template <class Base = RationalModel> class PyRationalModel : public PySensorModel<Base> {
public:
  using PySensorModel<Base>::PySensorModel;
  bool ImageToGround(const Point2d& image, Point3d& ground) const override {
    PYBIND11_OVERLOAD_PURE(bool, Base, ImageToGround, image, ground);
  }
  bool GroundToImage(const Point3d& ground, Point2d& image) const override {
    PYBIND11_OVERLOAD_PURE(bool, Base, GroundToImage, ground, image);
  }
};

PYBIND11_MODULE(wxpyext, m) {
  m.doc() = R"pbdoc(
    WatXtract package for python 
    ----------------------------

       // data types

       //structures
       Point3d

       //free functions
       generateFrameEpipolar
       generateRationalEpipolar

       //classes
       EarthEllipsoid
    )pbdoc";

  // data types

  //structures
  pb::class_<Point2d>(m, "Point2d")
    .def_readwrite("x", &Point2d::x)
    .def_readwrite("y", &Point2d::y);
  pb::class_<Point3d>(m, "Point3d")
    .def_readwrite("x", &Point3d::x)
    .def_readwrite("y", &Point3d::y)
    .def_readwrite("z", &Point3d::z);

  // free functions
  m.def("generateFrameEpipolar", &GenerateFrameEpipolar, R"pbdoc(
        Generate epipolar images for frame model)pbdoc");
  m.def("generateRationalEpipolar", &GenerateRationalEpipolar, R"pbdoc(
        Generate epipolar images for rational model)pbdoc");

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

  //pb::class_<ISensorModel, PyISensorModel<>> isensorModel(m, "ISensorModel");
  //pb::enum_<ISensorModel::ModelType>(isensorModel, "ModelType")
  //  .value("eUNDEFINED", ISensorModel::ModelType::eUNDEFINED)
  //  .value("eADS", ISensorModel::ModelType::eADS)
  //  .value("eFRAME", ISensorModel::ModelType::eFRAME)
  //  .value("ePUSHBROOM", ISensorModel::ModelType::ePUSHBROOM)
  //  .value("eROLLING", ISensorModel::ModelType::eROLLING)
  //  .value("eAFFINE", ISensorModel::ModelType::eAFFINE)
  //  .value("eNETWORK", ISensorModel::ModelType::eNETWORK)
  //  .value("eRATIONAL", ISensorModel::ModelType::eRATIONAL)
  //  .value("NumSensorTypes", ISensorModel::ModelType::NumSensorTypes);
  //isensorModel.def(pb::init<>())
  //  .def("imageToGround", pb::overload_cast<const Point2d&, Point3d&>(&ISensorModel::ImageToGround, pb::const_))
  //  .def("groundToImage", pb::overload_cast<const Point3d&, Point2d&>(&ISensorModel::GroundToImage, pb::const_));

  // module attributes
  m.attr("__version__") = "1.0.0";

  GDALInitialize();   // register GDAL drivers
}
