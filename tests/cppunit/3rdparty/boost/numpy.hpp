#include "3rdparty/stl/exstl.h"

#include <boost/python/numpy.hpp>
#include <iostream>
#include <vector>

using namespace earthxpert;
namespace bp = boost::python;
namespace bpn = boost::python::numpy;

TEST(bpn, DUMMY) {
  Py_Initialize();
  bpn::initialize();

  std::vector<int> arr{ 1,2,3,4,5 };
  auto pyarr = bpn::from_data(arr.data(), bpn::dtype::get_builtin<int>(),
    bp::make_tuple(5), bp::make_tuple(sizeof(int)), bp::object());

  std::cout << "C++ array:      " << arr << std::endl;
  std::cout << "Python ndarray: " << 
    bp::extract<char const*>(bp::str(pyarr)) << std::endl;

  pyarr[1] = 5;
  std::cout << "vector updated by ndarray: " << arr << std::endl;

  arr[2] = 8;
  std::cout << "ndarray updated by vector: " << 
    bp::extract<char const*>(bp::str(pyarr)) << std::endl;
}
