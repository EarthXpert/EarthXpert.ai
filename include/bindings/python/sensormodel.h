#include "sensormodel/rationalmodel.h"

// data types

//structures

// classes
// \todo all pure virtual
template <class Base = earthxpert::ISensorModel> 
class PyISensorModel : public Base {
public:
  using Base::Base;
  bool ImageToGround(const earthxpert::Point2d& image, 
    earthxpert::Point3d& ground) const override {
    PYBIND11_OVERLOAD_PURE(bool, Base, ImageToGround, image, ground);
  }
  bool GroundToImage(const earthxpert::Point3d& ground, 
    earthxpert::Point2d& image) const override {
    PYBIND11_OVERLOAD_PURE(bool, Base, GroundToImage, ground, image);
  }
};

template <class Base = earthxpert::SensorModel> 
class PySensorModel : public PyISensorModel<Base> {
public:
  using PyISensorModel<Base>::PyISensorModel;
  bool ImageToGround(const earthxpert::Point2d& image, 
    earthxpert::Point3d& ground) const override {
    PYBIND11_OVERLOAD_PURE(bool, Base, ImageToGround, image, ground);
  }
  bool GroundToImage(const earthxpert::Point3d& ground, 
    earthxpert::Point2d& image) const override {
    PYBIND11_OVERLOAD_PURE(bool, Base, GroundToImage, ground, image);
  }
};

template <class Base = earthxpert::RationalModel> 
class PyRationalModel : public PySensorModel<Base> {
public:
  using PySensorModel<Base>::PySensorModel;
  PyRationalModel() {}

  bool ImageToGround(const earthxpert::Point2d& image, 
    earthxpert::Point3d& ground) const override {
    PYBIND11_OVERLOAD_PURE(bool, Base, ImageToGround, image, ground);
  }
  bool GroundToImage(const earthxpert::Point3d& ground, 
    earthxpert::Point2d& image) const override {
    PYBIND11_OVERLOAD_PURE(bool, Base, GroundToImage, ground, image);
  }
};

// free functions
