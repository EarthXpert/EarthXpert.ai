//#include "sensormodel/rationalmodel.h"

// data types

//structures

// classes
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

// free functions
