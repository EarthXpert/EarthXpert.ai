/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rd_party/gtest/gtest_main.h"
#include "testconfig.h"

#include "sensormodel/rationalmodel.h"

#include <cmath>

using namespace watxtract;
using rfm = watxtract::RationalModel;

static std::unique_ptr<RationalModel> CreateRationalModel();

/**
 * Test creation of rational model.
 */
TEST(rationalmodel, CreateRationalModel) {
  const auto rationalModel = CreateRationalModel();

  ASSERT_TRUE(nullptr != rationalModel) << "returned nullptr";
}

/**
 * Test consistency between G2I & I2G projections at corners.
 */
TEST(rationalmodel, CornerI2G) {
  constexpr auto tol = 1.0e-8; // [mm]

  const auto rationalModel = CreateRationalModel();

  std::array<Point3d, 4> grdCorners{
      Point3d{8.225197491999999e+01, 4.511445409000000e+01,
              7.939600000000000e+02},
      Point3d{8.248208859000000e+01, 4.509750527000000e+01,
              5.558400000000000e+02},
      Point3d{8.248323748000000e+01, 4.497118496000000e+01,
              4.622700000000000e+02},
      Point3d{8.225262825000000e+01, 4.499020055000000e+01,
              4.967700000000000e+02} };

  for (const auto& ground : grdCorners) {
    Point2d image;
    rationalModel->GroundToImage(ground, image);

    Point3d grdI2G{ 0, 0, ground.z };
    rationalModel->ImageToGround(image, grdI2G);

    ASSERT_NEAR(ground.x, grdI2G.x, tol) << "Longitude difference > 1 mm";
    ASSERT_NEAR(ground.y, grdI2G.y, tol) << "Latitude difference > 1 mm";
  }
}

TEST(rationalmodel, CornerI2G_Functor) {
  constexpr auto tol = 1.0e-8; // [mm]

  const auto rationalModel = CreateRationalModel();

  std::array<Point3d, 4> grdCorners{
      Point3d{8.225197491999999e+01, 4.511445409000000e+01,
              7.939600000000000e+02},
      Point3d{8.248208859000000e+01, 4.509750527000000e+01,
              5.558400000000000e+02},
      Point3d{8.248323748000000e+01, 4.497118496000000e+01,
              4.622700000000000e+02},
      Point3d{8.225262825000000e+01, 4.499020055000000e+01,
              4.967700000000000e+02} };

  for (const auto& ground : grdCorners) {
    Point2d image;
    (*rationalModel)(ground, image);

    Point3d grdI2G{ 0, 0, ground.z };
    (*rationalModel)(image, grdI2G);

    ASSERT_NEAR(ground.x, grdI2G.x, tol) << "Longitude difference > 1 mm";
    ASSERT_NEAR(ground.y, grdI2G.y, tol) << "Latitude difference > 1 mm";
  }
}

/**
 * Test consistency between G2I & I2G projections in dense cube.
 */
TEST(rationalmodel, CubeI2G) {
  constexpr auto num{ 10 };
  const auto rationalModel = CreateRationalModel();

  auto nfc = rationalModel->GetNormalFators();
  const auto minLon = nfc[rfm::eOLONG] - nfc[rfm::eSLONG];
  const auto maxLon = nfc[rfm::eOLONG] + nfc[rfm::eSLONG];
  const auto minLat = nfc[rfm::eOLAT] - nfc[rfm::eSLAT];
  const auto maxLat = nfc[rfm::eOLAT] + nfc[rfm::eSLAT];
  const auto minHgt = nfc[rfm::eOHEIGHT] - nfc[rfm::eSHEIGHT];
  const auto maxHgt = nfc[rfm::eOHEIGHT] + nfc[rfm::eSHEIGHT];

  const auto stepLon = (maxLon - minLon) / num;
  const auto stepLat = (maxLat - minLat) / num;
  const auto stepHgt = (maxHgt - minHgt) / num;

  double maxDiffLon{};
  double maxDiffLat{};

  for (auto lon = minLon; lon <= maxLon; lon += stepLon) {
    for (auto lat = minLat; lat <= maxLat; lat += stepLat) {
      for (auto hgt = minHgt; hgt <= maxHgt; hgt += stepHgt) {
        Point3d ground{ lon, lat, hgt };

        Point2d image;
        rationalModel->GroundToImage(ground, image);

        Point3d grdI2G{ 0, 0, ground.z };
        rationalModel->ImageToGround(image, grdI2G);

        const auto diffLon = fabs(grdI2G.x - ground.x);
        if (maxDiffLon < diffLon)
          maxDiffLon = diffLon;

        const auto diffLat = fabs(grdI2G.y - ground.y);
        if (maxDiffLat < diffLat)
          maxDiffLat = diffLat;
      }
    }
  }

  constexpr auto tol = 1.0e-8; // [mm]

  ASSERT_LE(maxDiffLon, tol) << "Max. longitude difference > 1 mm";
  ASSERT_LE(maxDiffLat, tol) << "Max. latitude difference > 1 mm";
}

TEST(rationalmodel, CubeI2G_Functor) {
  constexpr auto num{ 10 };
  const auto rationalModel = CreateRationalModel();

  auto nfc = rationalModel->GetNormalFators();
  const auto minLon = nfc[rfm::eOLONG] - nfc[rfm::eSLONG];
  const auto maxLon = nfc[rfm::eOLONG] + nfc[rfm::eSLONG];
  const auto minLat = nfc[rfm::eOLAT] - nfc[rfm::eSLAT];
  const auto maxLat = nfc[rfm::eOLAT] + nfc[rfm::eSLAT];
  const auto minHgt = nfc[rfm::eOHEIGHT] - nfc[rfm::eSHEIGHT];
  const auto maxHgt = nfc[rfm::eOHEIGHT] + nfc[rfm::eSHEIGHT];

  const auto stepLon = (maxLon - minLon) / num;
  const auto stepLat = (maxLat - minLat) / num;
  const auto stepHgt = (maxHgt - minHgt) / num;

  double maxDiffLon{};
  double maxDiffLat{};

  for (auto lon = minLon; lon <= maxLon; lon += stepLon) {
    for (auto lat = minLat; lat <= maxLat; lat += stepLat) {
      for (auto hgt = minHgt; hgt <= maxHgt; hgt += stepHgt) {
        Point3d ground{ lon, lat, hgt };

        Point2d image;
        (*rationalModel)(ground, image);

        Point3d grdI2G{ 0, 0, ground.z };
        (*rationalModel)(image, grdI2G);

        const auto diffLon = fabs(grdI2G.x - ground.x);
        if (maxDiffLon < diffLon)
          maxDiffLon = diffLon;

        const auto diffLat = fabs(grdI2G.y - ground.y);
        if (maxDiffLat < diffLat)
          maxDiffLat = diffLat;
      }
    }
  }

  constexpr auto tol = 1.0e-8; // [mm]

  ASSERT_LE(maxDiffLon, tol) << "Max. longitude difference > 1 mm";
  ASSERT_LE(maxDiffLat, tol) << "Max. latitude difference > 1 mm";
}

/**
 * Create an instance of rational model
 */
std::unique_ptr<RationalModel> CreateRationalModel() {
  std::array<double, rfm::NumFact> normalFators{
      14521, 18751, 45.0429, 82.3670, 686, 14522, 18752, 0.0727, 0.1175, 501};
  std::array<double, rfm::NumCoef> lineNumCoef{
      +2.048296E-02, -1.444756E-01, -1.166505E+00, +2.179594E-02,
      +2.399269E-03, -1.701633E-04, -4.619032E-04, -7.510283E-04,
      -1.567463E-02, +1.107633E-05, -2.934684E-06, +1.082874E-05,
      +2.324952E-04, +1.012335E-05, +6.271587E-05, +1.971394E-05,
      +8.277023E-05, -1.254797E-06, -2.400344E-05, -1.540895E-06};
  std::array<double, rfm::NumCoef> lineDenCoef{
      +1.000000E+00, +5.656984E-03, +6.020129E-03, -1.011134E-03,
      +2.039104E-04, +3.110480E-06, -2.311865E-05, -3.463555E-05,
      +2.872835E-04, -7.137507E-05, +4.382423E-06, -6.956753E-07,
      -8.039794E-05, -6.994943E-07, -1.289141E-05, -2.214561E-04,
      +2.095757E-07, +3.218667E-07, +1.374648E-05, +1.189815E-07};
  std::array<double, rfm::NumCoef> sampNumCoef{
      +4.526993E-03, +1.023303E+00, +2.768776E-04, +1.985262E-02,
      +1.732794E-03, +2.556939E-04, +3.437755E-04, -4.321477E-03,
      -3.073514E-05, -1.103175E-06, +7.163050E-06, -2.597877E-05,
      -1.828961E-04, -2.722353E-05, -1.031462E-04, -5.073534E-05,
      +6.366313E-07, -3.450508E-07, +3.389686E-06, -5.443458E-07};
  std::array<double, rfm::NumCoef> sampDenCoef{
      +1.000000E+00, -2.146783E-04, -1.781214E-03, -3.517961E-04,
      +8.108283E-05, +6.804021E-07, -6.465760E-06, -2.220632E-05,
      +1.701262E-04, -2.672432E-05, +6.994983E-07, -1.545366E-07,
      -3.961882E-06, -1.172739E-07, -1.775734E-06, +6.411183E-06,
      +5.534508E-08, +8.539990E-08, +9.395855E-07, +1.757215E-08};

  auto wgs84Geog = std::make_shared<SpatialReferenceSystem>("WGS84");

  return std::make_unique<RationalModel>(normalFators, 
    lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef, wgs84Geog);
}
