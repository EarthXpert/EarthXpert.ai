/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"

#include "sensormodel/rationalmodel.h"
#include <cmath>

using namespace earthxpert;
using rfm = earthxpert::RationalModel;

static constexpr int kNum{ 10 };
static constexpr double kTol = 1.0e-8; // [mm]

static std::unique_ptr<RationalModel> CreateRationalModelA();
static std::unique_ptr<RationalModel> CreateRationalModelB();
static std::unique_ptr<RationalModel> CreateRationalModelC();
static void TestCubeI2G(const RationalModel* rationalModel);
static void TestCubeI2G_Functor(const RationalModel* rationalModel);

/**
 * Test creation of rational model.
 */
TEST(rationalmodel, CreateRationalModel) {
  const auto rationalModelA = CreateRationalModelA();
  ASSERT_TRUE(nullptr != rationalModelA) << "returned nullptr";

  const auto rationalModelB = CreateRationalModelB();
  ASSERT_TRUE(nullptr != rationalModelB) << "returned nullptr";

  const auto rationalModelC = CreateRationalModelC();
  ASSERT_TRUE(nullptr != rationalModelC) << "returned nullptr";
}

/**
 * Test consistency between G2I & I2G projections at corners.
 */
TEST(rationalmodel, CornerI2G) {
  const auto rationalModel = CreateRationalModelA();

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

    ASSERT_NEAR(ground.x, grdI2G.x, kTol) << "Longitude difference > 1 mm";
    ASSERT_NEAR(ground.y, grdI2G.y, kTol) << "Latitude difference > 1 mm";
  }
}

TEST(rationalmodel, CornerI2G_Functor) {
  const auto rationalModel = CreateRationalModelA();

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

    ASSERT_NEAR(ground.x, grdI2G.x, kTol) << "Longitude difference > 1 mm";
    ASSERT_NEAR(ground.y, grdI2G.y, kTol) << "Latitude difference > 1 mm";
  }
}

/**
 * Test consistency between G2I & I2G projections in dense cube.
 */
TEST(rationalmodel, CubeI2G) {
  const auto rationalModelA = CreateRationalModelA();
  TestCubeI2G(rationalModelA.get());

  const auto rationalModelB = CreateRationalModelB();
  TestCubeI2G(rationalModelB.get());

  const auto rationalModelC = CreateRationalModelC();
  TestCubeI2G(rationalModelC.get());
}

TEST(rationalmodel, CubeI2G_Functor) {
  const auto rationalModelA = CreateRationalModelA();
  TestCubeI2G_Functor(rationalModelA.get());

  const auto rationalModelB = CreateRationalModelB();
  TestCubeI2G_Functor(rationalModelB.get());

  const auto rationalModelC = CreateRationalModelC();
  TestCubeI2G_Functor(rationalModelC.get());
}

/**
 * Create an instance of rational model
 */
std::unique_ptr<RationalModel> CreateRationalModelA() {
  std::array<double, rnf::NumFact> normalFators{
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

/**
 * Create an instance of rational model of left image
 */
std::unique_ptr<RationalModel> CreateRationalModelB() {
  std::array<double, rnf::NumFact> normalFators{
    15069, 13790, 43.8664, -79.5300, 196, 15210, 13930, 0.0900, 0.1109, 500 };
  std::array<double, rfm::NumCoef> lineNumCoef{
    -2.283325E-03, -3.637293E-02, -1.028037E+00, +8.541168E-03,
    -1.087663E-03, +2.212122E-05, +3.198352E-04, -7.637938E-04,
    +2.038123E-03, -3.354134E-06, +1.433719E-07, -5.131378E-07,
    +5.072271E-05, +8.926054E-07, +1.807736E-05, +8.696844E-05,
    +2.565157E-05, +1.725727E-07, -2.467292E-06, -2.304789E-07 };
  std::array<double, rfm::NumCoef> lineDenCoef{
    +1.000000E+00, -1.228360E-03, -2.240329E-04, +3.473562E-04,
    +3.488909E-05, +4.910593E-07, -1.658898E-06, -2.266468E-05,
    +6.880573E-05, -2.521642E-05, +5.548128E-08, +4.066673E-07,
    -2.603538E-05, +7.047010E-08, -1.145117E-07, -2.627126E-04,
    -9.257284E-08, -6.076670E-08, +2.447826E-06, -1.121040E-08 };
  std::array<double, rfm::NumCoef> sampNumCoef{
    +3.104176E-03, +9.992945E-01, -1.032982E-02, +1.065050E-02,
    -3.057999E-04, +5.246950E-04, -4.638530E-04, -3.417709E-03,
    -6.456779E-04, +9.257974E-06, +4.782383E-06, -1.203111E-05,
    -8.542644E-05, -1.550251E-05, +9.560577E-05, +2.702477E-05,
    +2.592021E-07, -4.713430E-06, +1.373349E-06, -1.653996E-07 };
  std::array<double, rfm::NumCoef> sampDenCoef{
    +1.000000E+00, +3.042363E-04, +3.351404E-04, -5.622164E-04,
    -5.543118E-05, +2.766370E-06, -2.086463E-06, -1.633373E-05,
    +6.760431E-05, -1.601660E-05, -1.110994E-07, -1.948456E-07,
    +9.289130E-07, -5.578129E-08, +7.709145E-07, -1.884327E-06,
    -1.749326E-08, +1.599896E-08, -9.276634E-08, +1.988493E-08 };

  auto wgs84Geog = std::make_shared<SpatialReferenceSystem>("WGS84");
  auto rm = std::make_unique<RationalModel>(normalFators,
    lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef, wgs84Geog);
  rm->SetErrors(227.56, 0.13);

  return rm;
}

/**
 * Create an instance of rational model of right image
 */
std::unique_ptr<RationalModel> CreateRationalModelC() {
  std::array<double, rnf::NumFact> normalFators{
      15073, 13790, 43.7319, -79.5313, 110, 15198, 13940, 0.0893, 0.1106, 500 };
  std::array<double, rfm::NumCoef> lineNumCoef{
    -1.736510E-03, -3.255581E-02, -1.024674E+00, +7.518102E-03,
    -1.226802E-03, +2.042748E-05, +3.287586E-04, -7.485382E-04,
    +1.342133E-03, -3.026986E-06, +3.175060E-07, -8.070179E-07,
    +4.829815E-05, +7.835400E-07, +1.568980E-05, +8.209472E-05,
    +2.345234E-05, +1.756302E-07, -1.047164E-06, -1.646642E-07 };
  std::array<double, rfm::NumCoef> lineDenCoef{
    +1.000000E+00, -1.339828E-03, -4.880515E-04, +3.520250E-04,
    +3.248944E-05, +6.424253E-07, -9.058295E-07, -2.154700E-05,
    +6.224909E-05, -2.321114E-05, -1.469679E-07, -2.780368E-07,
    +1.868181E-05, +1.012657E-07, +5.782585E-07, +1.542782E-04,
    +3.152974E-08, -1.565110E-08, -2.824003E-06, +2.985498E-08 };
  std::array<double, rfm::NumCoef> sampNumCoef{
    +3.430236E-03, +9.963798E-01, -8.301724E-03, +1.133237E-02,
    -7.259747E-05, +5.164309E-04, -4.618378E-04, -3.682025E-03,
    -7.410386E-04, +9.105926E-06, +4.599130E-06, -1.044922E-05,
    -7.870863E-05, -1.534520E-05, +1.046738E-04, +1.922050E-04,
    +2.340716E-07, -5.273178E-06, -2.988280E-06, -1.750077E-07 };
  std::array<double, rfm::NumCoef> sampDenCoef{
    +1.000000E+00, +2.441650E-04, +1.009248E-04, -5.573238E-04,
    -4.782700E-05, +2.485334E-06, -1.799464E-06, -1.649388E-05,
    +6.248947E-05, -1.589834E-05, -1.116968E-07, -1.787185E-07,
    +4.066869E-07, -6.278089E-08, +8.102032E-07, -2.620842E-06,
    +0.000000E+00, +0.000000E+00, -5.450526E-08, +1.890781E-08 };

  auto wgs84Geog = std::make_shared<SpatialReferenceSystem>("WGS84");
  auto rm = std::make_unique<RationalModel>(normalFators,
    lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef, wgs84Geog);
  rm->SetErrors(227.34, 0.13);

  return rm;
}

void TestCubeI2G(const RationalModel* rationalModel) {
  auto nfc = rationalModel->GetNormalFators();
  const auto minLon = nfc[rnf::eOLONG] - nfc[rnf::eSLONG];
  const auto maxLon = nfc[rnf::eOLONG] + nfc[rnf::eSLONG];
  const auto minLat = nfc[rnf::eOLAT] - nfc[rnf::eSLAT];
  const auto maxLat = nfc[rnf::eOLAT] + nfc[rnf::eSLAT];
  const auto minHgt = nfc[rnf::eOHEIGHT] - nfc[rnf::eSHEIGHT];
  const auto maxHgt = nfc[rnf::eOHEIGHT] + nfc[rnf::eSHEIGHT];

  const auto stepLon = (maxLon - minLon) / kNum;
  const auto stepLat = (maxLat - minLat) / kNum;
  const auto stepHgt = (maxHgt - minHgt) / kNum;

  double maxDiffLon{-1.0};
  double maxDiffLat{-1.0};

  int n{};
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

        ++n;
      }
    }
  }

  ASSERT_LT(0, n) << "Number of test points";
  ASSERT_LE(maxDiffLon, kTol) << "Max. longitude difference > 1 mm";
  ASSERT_LE(maxDiffLat, kTol) << "Max. latitude difference > 1 mm";
}

void TestCubeI2G_Functor(const RationalModel* rationalModel) {
  auto nfc = rationalModel->GetNormalFators();
  const auto minLon = nfc[rnf::eOLONG] - nfc[rnf::eSLONG];
  const auto maxLon = nfc[rnf::eOLONG] + nfc[rnf::eSLONG];
  const auto minLat = nfc[rnf::eOLAT] - nfc[rnf::eSLAT];
  const auto maxLat = nfc[rnf::eOLAT] + nfc[rnf::eSLAT];
  const auto minHgt = nfc[rnf::eOHEIGHT] - nfc[rnf::eSHEIGHT];
  const auto maxHgt = nfc[rnf::eOHEIGHT] + nfc[rnf::eSHEIGHT];

  const auto stepLon = (maxLon - minLon) / kNum;
  const auto stepLat = (maxLat - minLat) / kNum;
  const auto stepHgt = (maxHgt - minHgt) / kNum;

  double maxDiffLon{-1.0};
  double maxDiffLat{-1.0};

  int n{};
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

        ++n;
      }
    }
  }

  ASSERT_LT(0, n) << "Number of test points";
  ASSERT_LE(maxDiffLon, kTol) << "Max. longitude difference > 1 mm";
  ASSERT_LE(maxDiffLat, kTol) << "Max. latitude difference > 1 mm";
}
