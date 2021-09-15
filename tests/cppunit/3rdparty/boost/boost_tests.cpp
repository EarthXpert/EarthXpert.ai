/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/boost/exboost.h"

//#include "algo.hpp"
#include "asio.hpp"
#include "fusion.hpp"
#include "datetime.hpp"
#include "geometry.hpp"
#include "graph.hpp"
#include "hana.hpp"
#include "math.hpp"
#include "mp11.hpp"
#include "mpl.hpp"
#include "multiarray.hpp"
//#include "multiprecision.hpp"
#include "numpy.hpp"
#include "process.hpp"
#include "python.hpp"
#include "spirit.hpp"
#include "thread.hpp"
#include "timer.hpp"

#include <boost/pool/object_pool.hpp>
#include <boost/type_traits.hpp>

#include <algorithm>
#include <complex>
#include <iostream>

using namespace std::complex_literals;

TEST(bfs, path) {
  bfs::path filePath("D:/Workspace/Results/output.txt");

  ASSERT_FALSE(filePath.filename_is_dot());
  ASSERT_TRUE(filePath.has_extension());
}

template<typename Ty, template<typename> class Compare>
struct BinaryCompare {
  bool BinayOp(const Ty lhs, const Ty rhs) const {
    if constexpr (boost::is_complex<Ty>::value) {
      return Compare<Ty::value_type>{}(std::norm(lhs), std::norm(rhs));
    }
    else {
      return Compare<Ty>{}(lhs, rhs);
    }
  }

  template<typename Ty2 = Ty>
  bool BinayOp2(const Ty2 lhs, const Ty2 rhs) const {
    static_assert(std::is_arithmetic_v<Ty2>, "scalar");
    return Compare<Ty2>{}(lhs, rhs);
  }

  template<typename Ty2>
  bool BinayOp2(const std::complex<Ty2> lhs, const std::complex<Ty2> rhs) const {
    static_assert(std::is_same_v<std::complex<Ty2>, Ty>, "complex");
    static_assert(std::is_same_v<Ty2, Ty::value_type>, "complex::value_type");
    return Compare<Ty2>{}(std::norm(lhs), std::norm(rhs));
  }

  bool operator()(const Ty lhs, const Ty rhs) const {
    return BinayOp(lhs, rhs);
  }
};

TEST(btt, cmp) {
  // scalar
  std::cout << "scalar<int>:     " << BinaryCompare<int, std::less>{}(1, 2) << std::endl;
  std::cout << "scalar<float>:   " << BinaryCompare<float, std::greater>{}(1.f, 2.f) << std::endl;

  BinaryCompare<int, std::less> bOpint;
  std::cout << "scalar<int>:     " << bOpint.BinayOp2(1, 2) << std::endl;
  BinaryCompare<float, std::greater> bOpflt;
  std::cout << "scalar<float>:   " << bOpflt.BinayOp2(1.f, 2.f) << std::endl;

  // complex
  std::complex<float> zf1{ 1.f + 1if }, zf2{ 2.f + 2if };
  std::cout << "complex<float>:  " << BinaryCompare<std::complex<float>, std::less>{}(zf1, zf2) << std::endl;
  std::complex<double> zd1{ 1. + 1i }, zd2{ 2. + 2i };
  std::cout << "complex<double>: " << BinaryCompare<std::complex<double>, std::greater>{}(zd1, zd2) << std::endl;

  BinaryCompare<std::complex<float>, std::less> bOpcflt;
  std::cout << "complex<double>: " << bOpcflt.BinayOp2(zf1, zf2) << std::endl;
  BinaryCompare<std::complex<double>, std::greater> bOpcdbl;
  std::cout << "complex<double>: " << bOpcdbl.BinayOp2(zd1, zd2) << std::endl;
}
