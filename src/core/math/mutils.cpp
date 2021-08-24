/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/math/mutils.h"

#include <algorithm>
#include <numeric>

namespace earthxpert {
double TruncateOffsetDigits(const double off, const int nunDigits) {
  if (0 > nunDigits) return off;
  const double pd = pow(10.0, nunDigits);
  return std::round(off * pd) / pd;
}

double TruncateScaleDigits(const double scl, const int nunDigits) {
  if (0 > nunDigits) return scl;
  const double pd = pow(10.0, nunDigits);
  const double sclN = std::round(scl * pd) / pd;
  return (0.0 != sclN) ? sclN : pow(10.0, -nunDigits);
}

std::pair<double, double> CalcOffsetScale(const std::vector<double>& arr,
  const int offDigits, const int sclDigits) {
  const auto sum = std::accumulate(arr.cbegin(), arr.cend(), 0.0);
  auto off = TruncateOffsetDigits(sum / double(arr.size()), offDigits);

  auto [min, max] = std::minmax_element(arr.cbegin(), arr.cend());
  auto scl = TruncateScaleDigits(std::max(off - *min, *max - off), sclDigits);

  return std::make_pair(off, scl);
}
}
