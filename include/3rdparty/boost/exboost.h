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

#include <boost/locale.hpp>

#include <boost/units/absolute.hpp>
#include <boost/units/cmath.hpp>
#include <boost/units/io.hpp>
#include <boost/units/pow.hpp>
#include <boost/units/systems/si.hpp>

namespace bfs = boost::filesystem;

namespace bg = boost::geometry;
namespace bgc = bg::concepts;
namespace bgm = bg::model;

namespace bl = boost::locale;

namespace bu = boost::units;
namespace bsi = boost::units::si;

#include <string>
#include <type_traits>

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

/**
 *  \brief set the i18n/l10n environment
 */
void EX3RDPARTY_EXPORTS ConfigureI18N_BL();
}
} // namespace earthxpert
