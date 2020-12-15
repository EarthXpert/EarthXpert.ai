/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/excore.h"
#include "3rdparty/interpolate/exinterpolate.h"

#include <boost/math/interpolators/barycentric_rational.hpp>
#include <boost/math/interpolators/cardinal_cubic_b_spline.hpp>

namespace bm = boost::math;
namespace bmi = bm::interpolators;
