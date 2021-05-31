/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/sltbench/sltbench_main_gdal.h"
#include "testconfig.h"

#include "sensormodel/rationalmodel.h"

#include <omp.h>

using namespace earthxpert;
using rfm = earthxpert::RationalModel;

constexpr int kNum = 15;
constexpr double kFactor = 0.75;

std::unique_ptr<RationalModel> CreateRationalModelLeft();
std::unique_ptr<RationalModel> CreateRationalModelRight();

void RationalModel_I2G() {

}

void RationalModel_I2GOMP() {

}

void RationalModel_CubeS2G() {
  //EnableCubeS2G(true);
}

void RationalModel_CubeS2GOMP() {
  //EnableCubeS2G(true);
}

std::unique_ptr<RationalModel> CreateRationalModelLeft() {
  std::array<double, rnf::NumFact> normalFators{
    11086, 9371, 34.6557, 109.2114, 396, 11524, 9637, 0.0508, 0.0522, 501 };
  std::array<double, rfm::NumCoef> lineNumCoef{
    +1.521921E-03, +1.474600E-02, -9.778159E-01, +3.780369E-02,
    +2.073335E-05, +1.215385E-04, +2.246249E-05, -2.171145E-04,
    -7.450344E-04, -2.110344E-07, +1.052829E-06, -4.701088E-06,
    -5.376786E-07, -8.545029E-07, -1.429913E-06, -6.277007E-07,
    -4.789538E-07, +1.425948E-06, +5.539230E-07, +0.000000E+00 };
  std::array<double, rfm::NumCoef> lineDenCoef{
    +1.000000E+00, -4.609758E-06, +7.588218E-04, -3.811571E-05,
    +1.859987E-07, +2.329609E-07, +9.682897E-08, -8.820424E-07,
    +1.003652E-06, -1.571331E-07, -1.319185E-06, +7.435641E-06,
    +2.544131E-07, +1.255983E-06, -1.196886E-06, -4.123720E-08,
    +2.781511E-07, +2.117748E-06, +1.241980E-06, -2.008707E-07 };
  std::array<double, rfm::NumCoef> sampNumCoef{
    -3.565944E-03, +9.928353E-01, +2.076299E-02, +2.823089E-02,
    -2.454540E-04, +6.150688E-04, -2.913558E-04, +1.762260E-03,
    +1.992950E-05, +2.542609E-05, -1.136942E-06, +4.152868E-06,
    -4.447290E-08, +8.203124E-07, -5.281411E-07, -9.666461E-08,
    +2.533985E-08, +1.945002E-06, -1.966671E-07, +2.214201E-08 };
  std::array<double, rfm::NumCoef> sampDenCoef{
    +1.000000E+00, +1.778571E-03, +3.249266E-04, -5.057361E-04,
    -9.937933E-08, +6.123450E-08, -2.404779E-08, +2.749872E-06,
    +4.336915E-07, -1.272603E-07, +6.887453E-07, +1.775895E-06,
    -3.854645E-07, -1.514237E-06, +2.438273E-06, +1.138310E-07,
    +3.018330E-07, +3.725428E-06, -3.713386E-07, -4.588691E-07 };

  auto wgs84Geog = std::make_shared<OGRSpatialRef>("WGS84");
  auto rm = std::make_unique<RationalModel>(normalFators,
    lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef, wgs84Geog);
  rm->SetErrors(36.87, 0.13);

  return rm;
}

std::unique_ptr<RationalModel> CreateRationalModelRight() {
  std::array<double, rnf::NumFact> normalFators{
      11064, 9370, 34.6557, 109.2114, 396, 11596, 9879, 0.0508, 0.0522, 501 };
  std::array<double, rfm::NumCoef> lineNumCoef{
    +3.735476E-03, +1.464924E-02, -9.717309E-01, -4.376065E-02,
    +4.157628E-05, +1.557648E-04, -1.178083E-03, -2.182991E-04,
    -1.789994E-03, -3.987463E-05, +2.657597E-06, +1.730955E-06,
    +1.216805E-06, +6.431261E-07, -1.588821E-06, -3.063611E-06,
    -1.323720E-06, -2.873439E-06, -9.298532E-07, +5.976351E-07 };
  std::array<double, rfm::NumCoef> lineDenCoef{
    +1.000000E+00, -3.953033E-06, +1.838174E-03, -9.851028E-04,
    +4.930497E-07, +1.323503E-06, +2.042529E-06, -7.187613E-07,
    +3.713023E-06, -8.623449E-07, +1.275056E-07, -1.311881E-06,
    -1.956620E-08, -1.072836E-06, -5.709434E-07, -2.697302E-07,
    +1.135140E-06, +9.726938E-07, -6.131941E-07, -2.103627E-06 };
  std::array<double, rfm::NumCoef> sampNumCoef{
    -3.474512E-03, +9.685162E-01, +2.025189E-02, +5.203661E-02,
    -2.618544E-04, +6.665277E-04, +3.603243E-04, +1.663155E-03,
    +1.917508E-05, +4.086770E-05, +5.996848E-07, +2.312352E-06,
    -6.698834E-07, -1.219678E-07, -5.556591E-07, -1.574238E-07,
    +9.081171E-07, +2.975967E-06, -9.154268E-07, -1.935041E-07 };
  std::array<double, rfm::NumCoef> sampDenCoef{
    +1.000000E+00, +1.727239E-03, +3.050319E-04, -4.741330E-04,
    -1.779630E-07, -3.323697E-07, +4.075508E-07, +2.951014E-06,
    +4.266388E-07, -6.648271E-07, +4.980653E-07, -5.128758E-06,
    -1.768367E-07, -3.220970E-06, -7.220123E-07, +0.000000E+00,
    -4.376586E-07, +5.630521E-07, +5.859119E-08, -2.803118E-07 };

  auto wgs84Geog = std::make_shared<OGRSpatialRef>("WGS84");
  auto rm = std::make_unique<RationalModel>(normalFators,
    lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef, wgs84Geog);
  rm->SetErrors(51.53, 0.13);

  return rm;
}

SLTBENCH_FUNCTION(RationalModel_I2G);
SLTBENCH_FUNCTION(RationalModel_I2GOMP);
SLTBENCH_FUNCTION(RationalModel_CubeS2G);
SLTBENCH_FUNCTION(RationalModel_CubeS2GOMP);
