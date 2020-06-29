/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"

#include "core/utils/unitutils.h"
#include "sensormodel/rationalmodel.h"
#include "sensormodel/rationalstereomodel.h"
#include "sensormodel/stereomodel.h"

//#include <iostream>

using namespace earthxpert;
using rfm = earthxpert::RationalModel;

static constexpr double kTol = 1.0e-8; // [mm]
static constexpr double kTolH = 1.0e-3; // [mm]
static constexpr int kNum = 5;
static constexpr double kFactor = 0.75;

static void TestImageToGround_Cube(const RationalModel* leftModel, const RationalModel* rightModel);

static std::unique_ptr<RationalModel> CreateLeftRationalModelA();
static std::unique_ptr<RationalModel> CreateRightRationalModelA();
static std::unique_ptr<RationalModel> CreateLeftRationalModelB();
static std::unique_ptr<RationalModel> CreateRightRationalModelB();

/**
 * Test 1: center at top and bottom
 */
TEST(stereomodel, ImageToGround) {
  const auto leftModel = CreateLeftRationalModelA();
  const auto rightModel = CreateRightRationalModelA();

  StereoModel smC(leftModel.get(), rightModel.get());    // Ceres
  smC.SetForwardAlgorithm(IStereoModel::eSM_CERES);

  RationalStereoModel smE(leftModel.get(), rightModel.get()); // EXAI

  StereoModel smI(leftModel.get(), rightModel.get());    // intersect
  Point3d origin(109.2114, 34.6557, 396);
  auto ee = std::make_shared<EarthEllipsoid>();
  ee->SetLsrOrigin(Deg2Rad(origin));
  smI.SetEarthEllipsoid(ee);
  smI.SetForwardAlgorithm(IStereoModel::eCA_INTERSECT);

  StereoModel smN(leftModel.get(), rightModel.get());    // NR 
  smN.SetForwardAlgorithm(IStereoModel::eSM_NR);

  Point2d imageL, imageR;
  Point3d grdC, grdE, grdI, grdN;

  // low position
  Point3d grdL(109.2114, 34.6557, 396.0 - 375.0);
  leftModel->GroundToImage(grdL, imageL);
  rightModel->GroundToImage(grdL, imageR);

  smC.ImageToGround(imageL, imageR, grdC);
  smE.ImageToGround(imageL, imageR, grdE);
  smI.ImageToGround(imageL, imageR, grdI);
  smN.ImageToGround(imageL, imageR, grdN);

  ASSERT_LE(fabs(grdC.x - grdL.x), kTol) << "Max. longitude difference > 1 mm";
  ASSERT_LE(fabs(grdC.y - grdL.y), kTol) << "Max.  latitude difference > 1 mm";
  ASSERT_LE(fabs(grdC.z - grdL.z), kTolH) << "Max.    height difference > 1 mm";
  ASSERT_LE(fabs(grdE.x - grdL.x), kTol) << "Max. longitude difference > 1 mm";
  ASSERT_LE(fabs(grdE.y - grdL.y), kTol) << "Max.  latitude difference > 1 mm";
  ASSERT_LE(fabs(grdE.z - grdL.z), kTolH) << "Max.    height difference > 1 mm";
  ASSERT_LE(fabs(grdI.x - grdL.x), kTol) << "Max. longitude difference > 1 mm";
  ASSERT_LE(fabs(grdI.y - grdL.y), kTol) << "Max.  latitude difference > 1 mm";
  ASSERT_LE(fabs(grdI.z - grdL.z), kTolH) << "Max.    height difference > 1 mm";
  ASSERT_LE(fabs(grdN.x - grdL.x), kTol) << "Max. longitude difference > 1 mm";
  ASSERT_LE(fabs(grdN.y - grdL.y), kTol) << "Max.  latitude difference > 1 mm";
  ASSERT_LE(fabs(grdN.z - grdL.z), kTolH)  << "Max.    height difference > 1 mm";

  // hight position
  Point3d grdH(109.2114, 34.6557, 396.0 + 375.0);
  leftModel->GroundToImage(grdH, imageL);
  rightModel->GroundToImage(grdH, imageR);

  smC.ImageToGround(imageL, imageR, grdC);
  smE.ImageToGround(imageL, imageR, grdE);
  smI.ImageToGround(imageL, imageR, grdI);
  smN.ImageToGround(imageL, imageR, grdN);

  ASSERT_LE(fabs(grdC.x - grdH.x), kTol) << "Max. longitude difference > 1 mm";
  ASSERT_LE(fabs(grdC.y - grdH.y), kTol) << "Max.  latitude difference > 1 mm";
  ASSERT_LE(fabs(grdC.z - grdH.z), kTolH) << "Max.    height difference > 1 mm";
  ASSERT_LE(fabs(grdE.x - grdH.x), kTol) << "Max. longitude difference > 1 mm";
  ASSERT_LE(fabs(grdE.y - grdH.y), kTol) << "Max.  latitude difference > 1 mm";
  ASSERT_LE(fabs(grdE.z - grdH.z), kTolH) << "Max.    height difference > 1 mm";
  ASSERT_LE(fabs(grdI.x - grdH.x), kTol) << "Max. longitude difference > 1 mm";
  ASSERT_LE(fabs(grdI.y - grdH.y), kTol) << "Max.  latitude difference > 1 mm";
  ASSERT_LE(fabs(grdI.z - grdH.z), kTolH) << "Max.    height difference > 1 mm";
  ASSERT_LE(fabs(grdN.x - grdH.x), kTol) << "Max. longitude difference > 1 mm";
  ASSERT_LE(fabs(grdN.y - grdH.y), kTol) << "Max.  latitude difference > 1 mm";
  ASSERT_LE(fabs(grdN.z - grdH.z), kTolH) << "Max.    height difference > 1 mm";
}

/**
 * Test 2: cube
 */
TEST(stereomodel, ImageToGround_Cube) {
  const auto leftModelA = CreateLeftRationalModelA();
  const auto rightModelA = CreateRightRationalModelA();
  TestImageToGround_Cube(leftModelA.get(), rightModelA.get());

  //const auto leftModelB = CreateLeftRationalModelB();
  //const auto rightModelB = CreateRightRationalModelB();
  //TestImageToGround_Cube(leftModelB.get(), rightModelB.get());
}

void TestImageToGround_Cube(const RationalModel* leftModel, const RationalModel* rightModel) {
  auto nfcL = leftModel->GetNormalFators();
  const auto minLonL = nfcL[rnf::eOLONG] - nfcL[rnf::eSLONG] * kFactor;
  const auto maxLonL = nfcL[rnf::eOLONG] + nfcL[rnf::eSLONG] * kFactor;
  const auto minLatL = nfcL[rnf::eOLAT] - nfcL[rnf::eSLAT] * kFactor;
  const auto maxLatL = nfcL[rnf::eOLAT] + nfcL[rnf::eSLAT] * kFactor;
  const auto minHgtL = nfcL[rnf::eOHEIGHT] - nfcL[rnf::eSHEIGHT] * kFactor;
  const auto maxHgtL = nfcL[rnf::eOHEIGHT] + nfcL[rnf::eSHEIGHT] * kFactor;

  auto nfcR = rightModel->GetNormalFators();
  const auto minLonR = nfcR[rnf::eOLONG] - nfcR[rnf::eSLONG] * kFactor;
  const auto maxLonR = nfcR[rnf::eOLONG] + nfcR[rnf::eSLONG] * kFactor;
  const auto minLatR = nfcR[rnf::eOLAT] - nfcR[rnf::eSLAT] * kFactor;
  const auto maxLatR = nfcR[rnf::eOLAT] + nfcR[rnf::eSLAT] * kFactor;
  const auto minHgtR = nfcR[rnf::eOHEIGHT] - nfcR[rnf::eSHEIGHT] * kFactor;
  const auto maxHgtR = nfcR[rnf::eOHEIGHT] + nfcR[rnf::eSHEIGHT] * kFactor;

  const auto minLon = std::max(minLonL, minLonR);
  const auto maxLon = std::min(maxLonL, maxLonR);
  const auto minLat = std::max(minLatL, minLatR);
  const auto maxLat = std::min(maxLatL, maxLatR);
  const auto minHgt = std::max(minHgtL, minHgtR);
  const auto maxHgt = std::min(maxHgtL, maxHgtR);

  const auto stepLon = (maxLon - minLon) / kNum;
  const auto stepLat = (maxLat - minLat) / kNum;
  const auto stepHgt = (maxHgt - minHgt) / kNum;

  double maxDiffLonC{ -1.0 }, maxDiffLatC{ -1.0 }, maxDiffHgtC{ -1.0 };
  double maxDiffLonE{ -1.0 }, maxDiffLatE{ -1.0 }, maxDiffHgtE{ -1.0 };
  double maxDiffLonI{ -1.0 }, maxDiffLatI{ -1.0 }, maxDiffHgtI{ -1.0 };
  double maxDiffLonN{ -1.0 }, maxDiffLatN{ -1.0 }, maxDiffHgtN{ -1.0 };

  StereoModel smC(leftModel, rightModel);    // Ceres 
  smC.SetForwardAlgorithm(IStereoModel::eSM_CERES);

  StereoModel smE(leftModel, rightModel);    // EXAI 

  Point3d origin((minLon + maxLon) / 2, (minLat + maxLat) / 2, (minHgt + maxHgt) / 2);
  auto ee = std::make_shared<EarthEllipsoid>();
  ee->SetLsrOrigin(Deg2Rad(origin));
  StereoModel smI(leftModel, rightModel);    // intersect
  smI.SetEarthEllipsoid(ee);
  smI.SetForwardAlgorithm(IStereoModel::eCA_INTERSECT);

  StereoModel smN(leftModel, rightModel);    // NR 
  smN.SetForwardAlgorithm(IStereoModel::eSM_NR);

  auto [leftNumCols, leftNumRows] = leftModel->GetImageSize();
  auto [rightNumCols, rightNumRows] = rightModel->GetImageSize();

  int n{};  // # of test points
  int nC{}; // # of successful Ceres
  int nE{}; // # of successful EXAI
  int nI{}; // # of successful interactions
  int nN{}; // # of NR.

  bool ret;
  double diffLon{}, diffLat{}, diffHgt{};

  for (auto lon = minLon; lon <= maxLon; lon += stepLon) {
    for (auto lat = minLat; lat <= maxLat; lat += stepLat) {
      for (auto hgt = minHgt; hgt <= maxHgt; hgt += stepHgt) {
        Point3d ground{ lon, lat, hgt };

        Point2d imageL;
        leftModel->GroundToImage(ground, imageL);
        if (0 > imageL.x || imageL.x > leftNumCols || 0 > imageL.y || imageL.y > leftNumRows)
          continue;

        Point2d imageR;
        rightModel->GroundToImage(ground, imageR);
        if (0 > imageR.x || imageR.x > rightNumCols || 0 > imageR.y || imageR.y > rightNumRows)
          continue;

        Point3d grdC{ ground };
        ret = smC.ImageToGround(imageL, imageR, grdC);
        if (ret) {
          diffLon = fabs(grdC.x - ground.x);
          if (maxDiffLonC < diffLon)
            maxDiffLonC = diffLon;

          diffLat = fabs(grdC.y - ground.y);
          if (maxDiffLatC < diffLat)
            maxDiffLatC = diffLat;

          diffHgt = fabs(grdC.z - ground.z);
          if (maxDiffHgtC < diffHgt)
            maxDiffHgtC = diffHgt;

          ++nC;
        }

        Point3d grdE{ ground };
        ret = smE.ImageToGround(imageL, imageR, grdE);
        if (ret) {
          diffLon = fabs(grdE.x - ground.x);
          if (maxDiffLonE < diffLon)
            maxDiffLonE = diffLon;

          diffLat = fabs(grdE.y - ground.y);
          if (maxDiffLatE < diffLat)
            maxDiffLatE = diffLat;

          diffHgt = fabs(grdE.z - ground.z);
          if (maxDiffHgtE < diffHgt)
            maxDiffHgtE = diffHgt;

          ++nE;
        }

        Point3d grdI{ ground };
        ret = smI.ImageToGround(imageL, imageR, grdI);
        if (ret) {
          diffLon = fabs(grdI.x - ground.x);
          if (maxDiffLonI < diffLon)
            maxDiffLonI = diffLon;

          diffLat = fabs(grdI.y - ground.y);
          if (maxDiffLatI < diffLat)
            maxDiffLatI = diffLat;

          diffHgt = fabs(grdI.z - ground.z);
          if (maxDiffHgtI < diffHgt)
            maxDiffHgtI = diffHgt;

          ++nI;
        }

        Point3d grdN{ ground };
        ret = smN.ImageToGround(imageL, imageR, grdN);
        if (ret) {
          diffLon = fabs(grdN.x - ground.x);
          if (maxDiffLonN < diffLon)
            maxDiffLonN = diffLon;

          diffLat = fabs(grdN.y - ground.y);
          if (maxDiffLatN < diffLat)
            maxDiffLatN = diffLat;

          diffHgt = fabs(grdN.z - ground.z);
          if (maxDiffHgtN < diffHgt)
            maxDiffHgtN = diffHgt;

          ++nN;
        }
        ++n;
      }
    }
  }

  ASSERT_LT(0, n)  << "Number of test points: " << n;
  ASSERT_EQ(nC, n) << "Number of Ceres points: " << nC;
  ASSERT_EQ(nE, n) << "Number of Ceres points: " << nE;
  ASSERT_EQ(nI, n) << "Number of interaction points: " << nI;
  ASSERT_EQ(nN, n) << "Number of NR points: " << nN;

  ASSERT_LE(maxDiffLonC, kTol) << "Max. longitude difference > 1 mm: " << maxDiffLonC;
  ASSERT_LE(maxDiffLatC, kTol) << "Max.  latitude difference > 1 mm: " << maxDiffLatC;
  ASSERT_LE(maxDiffHgtC, kTolH) << "Max.    height difference > 1 mm: " << maxDiffHgtC;
  ASSERT_LE(maxDiffLonE, kTol) << "Max. longitude difference > 1 mm: " << maxDiffLonE;
  ASSERT_LE(maxDiffLatE, kTol) << "Max.  latitude difference > 1 mm: " << maxDiffLatE;
  ASSERT_LE(maxDiffHgtE, kTolH) << "Max.    height difference > 1 mm: " << maxDiffHgtE;
  ASSERT_LE(maxDiffLonI, kTol) << "Max. longitude difference > 1 mm: " << maxDiffLonI;
  ASSERT_LE(maxDiffLatI, kTol) << "Max.  latitude difference > 1 mm: " << maxDiffLatI;
  ASSERT_LE(maxDiffHgtI, kTolH) << "Max.    height difference > 1 mm: " << maxDiffHgtI;
  ASSERT_LE(maxDiffLonN, kTol)   << "Max. longitude difference > 1 mm: " << maxDiffLonN;
  ASSERT_LE(maxDiffLatN, kTol)   << "Max.  latitude difference > 1 mm: " << maxDiffLatN;
  ASSERT_LE(maxDiffHgtN, kTolH)  << "Max.    height difference > 1 mm: " << maxDiffHgtN;
}

/**
 * Create an instance of rational model of left image
 */
std::unique_ptr<RationalModel> CreateLeftRationalModelA() {
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

  auto wgs84Geog = std::make_shared<SpatialReferenceSystem>("WGS84");
  auto rm = std::make_unique<RationalModel>(normalFators,
    lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef, wgs84Geog);
  rm->SetErrors(36.87, 0.13);

  return rm;
}

/**
 * Create an instance of rational model of right image
 */
std::unique_ptr<RationalModel> CreateRightRationalModelA() {
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

  auto wgs84Geog = std::make_shared<SpatialReferenceSystem>("WGS84");
  auto rm = std::make_unique<RationalModel>(normalFators,
    lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef, wgs84Geog);
  rm->SetErrors(51.53, 0.13);

  return rm;
}

// \todo exam models
/**
 * Create an instance of rational model of left image
 */
std::unique_ptr<RationalModel> CreateLeftRationalModelB() {
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
std::unique_ptr<RationalModel> CreateRightRationalModelB() {
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
