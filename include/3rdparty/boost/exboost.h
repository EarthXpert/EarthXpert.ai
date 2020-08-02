/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"

#include <boost/filesystem.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/comparable_distance.hpp>
#include <boost/geometry/geometries/segment.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <string>
#include <type_traits>

namespace bfs = boost::filesystem;
namespace bg = boost::geometry;
namespace bgc = boost::geometry::concepts;
namespace bgm = boost::geometry::model;

namespace earthxpert {
template <typename Ty, typename = std::enable_if_t<std::is_arithmetic_v<Ty>, void>>
using PointXY = bgm::d2::point_xy<Ty>;

// defaults: clock-wise, closed, std::vector 
template <typename Ty, typename = std::enable_if_t<std::is_arithmetic_v<Ty>, void>>
using Polygon2 = bgm::polygon<PointXY<Ty>>;

using PointXYf = PointXY<float>;
using PointXYd = PointXY<double>;

using Polygon2f = Polygon2<float>;
using Polygon2d = Polygon2<double>;

namespace thirdparty{
/**
 *  \brief insert prefix for filename component
 *
 *  \param [in] fileName input full path filename
 *  \param [in] prefix string to be inserted
 *  \return Returns prefix + filename
 */
std::string EX3RDPARTY_EXPORTS PathPrefixFilename(
  const std::string& fileName, const std::string& prefix);
}
} // namespace earthxpert
