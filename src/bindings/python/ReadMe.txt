perhaps one package exai.pyd that wraps all C++ modules.

1) *.hpp to be included before statement PYBIND11_MODULE(expyext, m)

2) *_m.hpp to be included within statement PYBIND11_MODULE(expyext, m)