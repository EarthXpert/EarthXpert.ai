/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/rmutils.h"

namespace earthxpert {
 /**
  *  \brief Compute the combinations of normalized longitude, latitude,
  *  and height that go along with the rational function.
  *
  *  \param [in] u normalized longitude
  *  \param [in] v normalized latitude
  *  \param [in] w normalized height
  *  \param [out] uvw array of combined terms.
  */
void CalcTerms(const double u, const double v, const double w, double* uvw) {
  uvw[rpc::eC] = 1.0;
  uvw[rpc::eL] = u;
  uvw[rpc::eP] = v;
  uvw[rpc::eH] = w;
  uvw[rpc::eLP] = u * v;
  uvw[rpc::eLH] = u * w;
  uvw[rpc::ePH] = v * w;
  uvw[rpc::eLL] = u * u;
  uvw[rpc::ePP] = v * v;
  uvw[rpc::eHH] = w * w;
  uvw[rpc::ePLH] = uvw[rpc::eLP] * w; // L * P * H
  uvw[rpc::eLLL] = uvw[rpc::eLL] * u; // L * L * L
  uvw[rpc::eLPP] = u * uvw[rpc::ePP]; // L * P * P
  uvw[rpc::eLHH] = u * uvw[rpc::eHH]; // L * H * H
  uvw[rpc::eLLP] = uvw[rpc::eLL] * v; // L * L * P
  uvw[rpc::ePPP] = uvw[rpc::ePP] * v; // P * P * P
  uvw[rpc::ePHH] = v * uvw[rpc::eHH]; // P * H * H
  uvw[rpc::eLLH] = uvw[rpc::eLL] * w; // L * L * H
  uvw[rpc::ePPH] = uvw[rpc::ePP] * w; // P * P * H
  uvw[rpc::eHHH] = uvw[rpc::eHH] * w; // H * H * H
}

void CalcTerms(const Point3d& uvw, double* const LPH,
  const std::bitset<rpc::NumCoef>& coefFlags) {
  CalcTerms(uvw.x, uvw.y, uvw.z, LPH);

  for (int i = 0; i < rpc::NumCoef; ++i) {
    if (!coefFlags.test(i))
      LPH[i] = 0.0;
  }
}
}
