/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXPYEXT_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXPYEXT_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXPYEXT_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXPYEXT_EXPORTS
#define EXPYEXT_EXPORTS
#endif

// python extension
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/numpy/ndarray.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <xtensor/xmath.hpp>
#include <xtensor/xarray.hpp>

//#define FORCE_IMPORT_ARRAY
#include <xtensor-python/pyarray.hpp>
#include <xtensor-python/pytensor.hpp>
#include <xtensor-python/pyvectorize.hpp>

#include <functional>
#include <list>

namespace bp = boost::python;
namespace bpn = boost::python::numpy;

/**
 *  \brief macro adds submodels to package expyext
 */
using InitFunctor = std::function<void(pybind11::module&)>;
using PyModuleList = std::list<InitFunctor>;

PyModuleList& ExpyModules();

class ExpyModule {
  using InitFunc = void (*)(pybind11::module&);

public:
  ExpyModule(InitFunc init) { ExpyModules().push_back(init); }

  ExpyModule(const char* submodule_name, InitFunc init) {
    ExpyModules().push_back([=](pybind11::module& m) {
      auto sm = m.def_submodule(submodule_name);
      init(sm); });
  }
};

#define EXPY_SUBMODULE(name, sm) \
  void ExpyInit##name(pybind11::module &); \
  ExpyModule name(#name, ExpyInit##name);  \
  void ExpyInit##name(pybind11::module& sm)
