/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <boost/geometry/algorithms/for_each.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/multi_point.hpp>

using point_t = bg::model::point<double, 3, bg::cs::cartesian>;
using points_t = bg::model::multi_point<point_t>;
using linestring_t = bg::model::linestring<point_t>;

struct ZeroZ {
  inline void operator()(point_t& p) const { p.set<2>(0); }
  template <typename G> inline void operator()(G& g) const { bg::for_each_point(g, *this); }
} static constexpr zeroz{};

static inline constexpr auto io = [](auto& g) { return bg::dsv(g); }; // switch to bg::wkt easily

/**
 *  boost.geometry basically works in 2d for 3d geometries.
 */
TEST(boost, geometry_intersection) {
  const bool cond[9] = { false, false, true,
                         false, true, true,
                         true, false, false };
  const linestring_t lss[] = { {{0, 0, 1}, {1, 1, 1}},
                               {{0, 1, 0}, {1, 0, 0}},
                               {{0, 1, 0}, {1, 0, 1}} };

  //std::cout << std::fixed << std::setprecision(2);

  int i{};
  for (auto a : lss) {
    auto az{ a };
    zeroz(az);

    for (auto b : lss) {
      points_t c, d;
      bg::intersection(a, b, c);
      //std::cout << "Normal:  " << io(a) << " with " << io(b) << " ->" << io(c) << "\n";

      zeroz(b);
      bg::intersection(az, b, d);

      const auto eq{ bg::equals(c, d) };
      ASSERT_TRUE(cond[i] == eq);

      //if (!eq)
      //  std::cout << " - but:  " << io(az) << " with " << io(b) << " ->" << io(d) << "\n";
      ++i;
    }
  }
}
