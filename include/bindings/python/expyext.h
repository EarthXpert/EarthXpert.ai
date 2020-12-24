/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

//#include <boost/python.hpp>
#include <boost/python/numpy/ndarray.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <xtensor-python/pyarray.hpp>
#include <xtensor-python/pytensor.hpp>
#include <xtensor-python/pyvectorize.hpp>

#include <functional>
#include <list>

using InitFunctor = std::function<void(pybind11::module&)>;
using PyModuleList = std::list<InitFunctor>;

PyModuleList& ExpyModules();

class ExpyModule {
  using InitFunc = void (*)(pybind11::module&);

public:
  ExpyModule(InitFunc init) { ExpyModules().push_back(init); }

  ExpyModule(const char* submodule_name, InitFunc init) {
    ExpyModules().push_back([=](pybind11::module& parent) {
      auto m = parent.def_submodule(submodule_name);
      init(m); });
  }
};

#define EXPY_SUBMODULE(name, m) \
  void ExpyInit##name(pybind11::module &); \
  ExpyModule name(#name, ExpyInit##name);  \
  void ExpyInit##name(pybind11::module& m)
