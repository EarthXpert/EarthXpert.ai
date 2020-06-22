/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/sltbench/sltbench_main_gdal.h"
#include "testconfig.h"

#include "core/utils/unitutils.h"
#include "sensormodel/rationalmodel.h"
#include "sensormodel/stereomodel.h"

#include <omp.h>

using namespace earthxpert;
using rfm = earthxpert::RationalModel;

static constexpr int num = 15;
static constexpr double factor = 0.75;

static void I2G_Cube(const StereoModel& sm, const std::vector<Point3d>& cube);
static void I2G_CubeOMP(const StereoModel& sm, const std::vector<Point3d>& cube);
static std::vector<Point3d> CreateCube(const RationalModel* leftModel, const RationalModel* rightModel);
static std::unique_ptr<RationalModel> CreateLeftRationalModel();
static std::unique_ptr<RationalModel> CreateRightRationalModel();

/**
 *  TestCase 0: timing cube creation
 */
void StereoModel_Cube() {
  const auto leftModel = CreateLeftRationalModel();
  const auto rightModel = CreateRightRationalModel();
  auto cube = CreateCube(leftModel.get(), rightModel.get());
}

/**
 *  TestCase 1: stereo via intersection
 */
void StereoModel_I2G_Intersect() {
  const auto leftModel = CreateLeftRationalModel();
  const auto rightModel = CreateRightRationalModel();
  auto cube = CreateCube(leftModel.get(), rightModel.get());

  auto nfcL = leftModel->GetNormalFators();
  Point3d origin(nfcL[rfm::eOLONG], nfcL[rfm::eOLAT], nfcL[rfm::eOHEIGHT]);
  auto ee = std::make_shared<EarthEllipsoid>();
  ee->SetLsrOrigin(Deg2Rad(origin));
 
  StereoModel smI(leftModel.get(), rightModel.get());    // intersect
  smI.SetEarthEllipsoid(std::move(ee));
  smI.SetForwardAlgorithm(StereoModel::eCA_INTERSECT);
  I2G_Cube(smI, cube);
}

/**
 *  TestCase 2: stereo via NR
 */
void StereoModel_I2G_NR() {
  const auto leftModel = CreateLeftRationalModel();
  const auto rightModel = CreateRightRationalModel();
  auto cube = CreateCube(leftModel.get(), rightModel.get());

  StereoModel smN(leftModel.get(), rightModel.get());    // NR
  smN.SetForwardAlgorithm(StereoModel::eSM_NR);
  I2G_Cube(smN, cube);
}

/**
 *  TestCase 3: OMP stereo via intersection
 */
void StereoModel_I2G_IntersectOMP() {
  const auto leftModel = CreateLeftRationalModel();
  const auto rightModel = CreateRightRationalModel();
  auto cube = CreateCube(leftModel.get(), rightModel.get());

  auto nfcL = leftModel->GetNormalFators();
  Point3d origin(nfcL[rfm::eOLONG], nfcL[rfm::eOLAT], nfcL[rfm::eOHEIGHT]);
  auto ee = std::make_shared<EarthEllipsoid>();
  ee->SetLsrOrigin(Deg2Rad(origin));

  StereoModel smI(leftModel.get(), rightModel.get());    // intersect
  smI.SetEarthEllipsoid(std::move(ee));
  smI.SetForwardAlgorithm(StereoModel::eCA_INTERSECT);
  I2G_CubeOMP(smI, cube);
}

/**
 *  TestCase 4: OMP stereo via NR
 */
void StereoModel_I2G_NROMP() {
  const auto leftModel = CreateLeftRationalModel();
  const auto rightModel = CreateRightRationalModel();
  auto cube = CreateCube(leftModel.get(), rightModel.get());

  StereoModel smN(leftModel.get(), rightModel.get());    // NR
  smN.SetForwardAlgorithm(StereoModel::eSM_NR);
  I2G_CubeOMP(smN, cube);
}

// single thread
void I2G_Cube(const StereoModel& sm, const std::vector<Point3d>& cube) {
  Point2d imageL;
  Point2d imageR;
  Point3d grd;

  auto& models = sm.GetSensorModels();

  for (auto& ground : cube) {
    models[0]->GroundToImage(ground, imageL);
    models[1]->GroundToImage(ground, imageR);

    sm.ImageToGround(imageL, imageR, grd);
  }
}

// OMP
void I2G_CubeOMP(const StereoModel& sm, const std::vector<Point3d>& cube) {
  auto& models = sm.GetSensorModels();
  const int64 numPts = cube.size();

#pragma omp parallel 
  {
  Point2d imageL;
  Point2d imageR;
  Point3d grd;

#pragma omp for
  for (int64 i = 0; i < numPts; ++i) {
    models[0]->GroundToImage(cube[i], imageL);
    models[1]->GroundToImage(cube[i], imageR);

    sm.ImageToGround(imageL, imageR, grd);
  }
  }
}

std::vector<Point3d> CreateCube(const RationalModel* leftModel, const RationalModel* rightModel) {
  auto nfcL = leftModel->GetNormalFators();
  const auto minLonL = nfcL[rfm::eOLONG] - nfcL[rfm::eSLONG] * factor;
  const auto maxLonL = nfcL[rfm::eOLONG] + nfcL[rfm::eSLONG] * factor;
  const auto minLatL = nfcL[rfm::eOLAT] - nfcL[rfm::eSLAT] * factor;
  const auto maxLatL = nfcL[rfm::eOLAT] + nfcL[rfm::eSLAT] * factor;
  const auto minHgtL = nfcL[rfm::eOHEIGHT] - nfcL[rfm::eSHEIGHT] * factor;
  const auto maxHgtL = nfcL[rfm::eOHEIGHT] + nfcL[rfm::eSHEIGHT] * factor;

  auto nfcR = rightModel->GetNormalFators();
  const auto minLonR = nfcR[rfm::eOLONG] - nfcR[rfm::eSLONG] * factor;
  const auto maxLonR = nfcR[rfm::eOLONG] + nfcR[rfm::eSLONG] * factor;
  const auto minLatR = nfcR[rfm::eOLAT] - nfcR[rfm::eSLAT] * factor;
  const auto maxLatR = nfcR[rfm::eOLAT] + nfcR[rfm::eSLAT] * factor;
  const auto minHgtR = nfcR[rfm::eOHEIGHT] - nfcR[rfm::eSHEIGHT] * factor;
  const auto maxHgtR = nfcR[rfm::eOHEIGHT] + nfcR[rfm::eSHEIGHT] * factor;

  const auto minLon = std::max(minLonL, minLonR);
  const auto maxLon = std::min(maxLonL, maxLonR);
  const auto minLat = std::max(minLatL, minLatR);
  const auto maxLat = std::min(maxLatL, maxLatR);
  const auto minHgt = std::max(minHgtL, minHgtR);
  const auto maxHgt = std::min(maxHgtL, maxHgtR);

  const auto stepLon = (maxLon - minLon) / num;
  const auto stepLat = (maxLat - minLat) / num;
  const auto stepHgt = (maxHgt - minHgt) / num;

  std::vector<Point3d> cube;
  cube.reserve(pow(num+1, 3));

  for (auto lon = minLon; lon <= maxLon; lon += stepLon) {
    for (auto lat = minLat; lat <= maxLat; lat += stepLat) {
      for (auto hgt = minHgt; hgt <= maxHgt; hgt += stepHgt) {
        cube.emplace_back(lon, lat, hgt);
      }
    }
  }

  return cube;
}

std::unique_ptr<RationalModel> CreateLeftRationalModel() {
  std::array<double, rfm::NumFact> normalFators{
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

  auto wgs84Geog = std::make_shared<SpatialReferenceSystem>("WGS84");
  auto rm = std::make_unique<RationalModel>(normalFators,
    lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef, wgs84Geog);
  rm->SetErrors(36.87, 0.13);

  return rm;
}

std::unique_ptr<RationalModel> CreateRightRationalModel() {
  std::array<double, rfm::NumFact> normalFators{
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

  auto wgs84Geog = std::make_shared<SpatialReferenceSystem>("WGS84");
  auto rm = std::make_unique<RationalModel>(normalFators,
    lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef, wgs84Geog);
  rm->SetErrors(51.53, 0.13);

  return rm;
}

SLTBENCH_FUNCTION(StereoModel_Cube);
SLTBENCH_FUNCTION(StereoModel_I2G_Intersect);
SLTBENCH_FUNCTION(StereoModel_I2G_NR);
SLTBENCH_FUNCTION(StereoModel_I2G_IntersectOMP);
SLTBENCH_FUNCTION(StereoModel_I2G_NROMP);
