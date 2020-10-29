/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"

#include "core/math/constants.h"
#include "core/math/mutils.h"
#include "core/utils/unitutils.h"
#include "marshaller/rationalmodelmarshaller.h"
#include "sensormodel/rationalmodel.h"
#include "sensormodel/rationalstereomodel.h"
#include "sensormodel/utils/nullstereomodel.h"
#include "sensormodel/utils/stereomodel.h"

//#include <iostream>

using namespace earthxpert;
using rfm = earthxpert::RationalModel;

static constexpr double kTolP = 1.0e-2; // [0.01 pixel]
static constexpr int kNum = 5;
static constexpr double kFactor = 0.75;

static void TestImageToGround_Cube(const RationalModel* leftModel, const RationalModel* rightModel);

static std::unique_ptr<RationalModel> CreateRationalModelALeft();
static std::unique_ptr<RationalModel> CreateRationalModelARight();
static std::unique_ptr<RationalModel> CreateRationalModelBLeft();
static std::unique_ptr<RationalModel> CreateRationalModelBRight();

/**
 * Test 1: center at top and bottom
 * \todo put algorithms in an array
 */
TEST(stereomodel, ImageToGround) {
  const auto leftModel = CreateRationalModelALeft();
  const auto rightModel = CreateRationalModelARight();

  StereoModel smC(leftModel.get(), rightModel.get());    // Ceres
  smC.SetForwardAlgorithm(IStereoModel::eSM_CERES);

  StereoModel smI(leftModel.get(), rightModel.get());    // intersect
  Point3d origin(109.2114, 34.6557, 396);
  auto ee = std::make_shared<EarthEllipsoid>();
  ee->SetLsrOrigin(Deg2Rad(origin));
  smI.SetEarthEllipsoid(ee);
  smI.SetForwardAlgorithm(IStereoModel::eCA_INTERSECT);

  RationalStereoModel smE(leftModel.get(), rightModel.get()); // EXAI

  StereoModel smL(leftModel.get(), rightModel.get());    // LS 
  smL.SetForwardAlgorithm(IStereoModel::eSM_LS);

  StereoModel smN(leftModel.get(), rightModel.get());    // NR 
  smN.SetForwardAlgorithm(IStereoModel::eSM_NR);

  Point2d imageL, imageR;
  Point3d grdC, grdI, grdE, grdL, grdN;
  std::vector<Point2d> imgC, imgI, imgE, imgL, imgN;

  // low and high positions
  std::array<Point3d, 2> grd{Point3d(109.2114, 34.6557, 396.0 - 375.0), 
                             Point3d(109.2114, 34.6557, 396.0 + 375.0) };
  
  for (int i = 0; i < 2; ++i) {
    leftModel->GroundToImage(grd[i], imageL);
    rightModel->GroundToImage(grd[i], imageR);

    // forward intersection
    smC.ImagesToGround(imageL, imageR, grdC);
    smI.ImagesToGround(imageL, imageR, grdI);
    smE.ImagesToGround(imageL, imageR, grdE);
    smL.ImagesToGround(imageL, imageR, grdL);
    smN.ImagesToGround(imageL, imageR, grdN);

    ASSERT_LE(fabs(grdC.x - grd[i].x), kTolDEG) << "Max. longitude difference > 1 mm";
    ASSERT_LE(fabs(grdC.y - grd[i].y), kTolDEG) << "Max.  latitude difference > 1 mm";
    ASSERT_LE(fabs(grdC.z - grd[i].z), kTolMM) << "Max.    height difference > 1 mm";
    ASSERT_LE(fabs(grdI.x - grd[i].x), kTolDEG) << "Max. longitude difference > 1 mm";
    ASSERT_LE(fabs(grdI.y - grd[i].y), kTolDEG) << "Max.  latitude difference > 1 mm";
    ASSERT_LE(fabs(grdI.z - grd[i].z), kTolMM) << "Max.    height difference > 1 mm";
    ASSERT_LE(fabs(grdE.x - grd[i].x), kTolDEG) << "Max. longitude difference > 1 mm";
    ASSERT_LE(fabs(grdE.y - grd[i].y), kTolDEG) << "Max.  latitude difference > 1 mm";
    ASSERT_LE(fabs(grdE.z - grd[i].z), kTolMM) << "Max.    height difference > 1 mm";
    ASSERT_LE(fabs(grdL.x - grd[i].x), kTolDEG) << "Max. longitude difference > 1 mm";
    ASSERT_LE(fabs(grdL.y - grd[i].y), kTolDEG) << "Max.  latitude difference > 1 mm";
    ASSERT_LE(fabs(grdL.z - grd[i].z), kTolMM) << "Max.    height difference > 1 mm";
    ASSERT_LE(fabs(grdN.x - grd[i].x), kTolDEG) << "Max. longitude difference > 1 mm";
    ASSERT_LE(fabs(grdN.y - grd[i].y), kTolDEG) << "Max.  latitude difference > 1 mm";
    ASSERT_LE(fabs(grdN.z - grd[i].z), kTolMM) << "Max.    height difference > 1 mm";

    // reprojection for consistency check
    smC.GroundToImages(grdC, imgC);
    smI.GroundToImages(grdI, imgI);
    smE.GroundToImages(grdE, imgE);
    smL.GroundToImages(grdL, imgL);
    smN.GroundToImages(grdN, imgN);

    ASSERT_NEAR(imageL.x, imgC[0].x, kTolP);
    ASSERT_NEAR(imageL.y, imgC[0].y, kTolP);
    ASSERT_NEAR(imageR.x, imgC[1].x, kTolP);
    ASSERT_NEAR(imageR.y, imgC[1].y, kTolP);
    ASSERT_NEAR(imageL.x, imgI[0].x, kTolP);
    ASSERT_NEAR(imageL.y, imgI[0].y, kTolP);
    ASSERT_NEAR(imageR.x, imgI[1].x, kTolP);
    ASSERT_NEAR(imageR.y, imgI[1].y, kTolP);
    ASSERT_NEAR(imageL.x, imgE[0].x, kTolP);
    ASSERT_NEAR(imageL.y, imgE[0].y, kTolP);
    ASSERT_NEAR(imageR.x, imgE[1].x, kTolP);
    ASSERT_NEAR(imageR.y, imgE[1].y, kTolP);
    ASSERT_NEAR(imageL.x, imgL[0].x, kTolP);
    ASSERT_NEAR(imageL.y, imgL[0].y, kTolP);
    ASSERT_NEAR(imageR.x, imgL[1].x, kTolP);
    ASSERT_NEAR(imageR.y, imgL[1].y, kTolP);
    ASSERT_NEAR(imageL.x, imgN[0].x, kTolP);
    ASSERT_NEAR(imageL.y, imgN[0].y, kTolP);
    ASSERT_NEAR(imageR.x, imgN[1].x, kTolP);
    ASSERT_NEAR(imageR.y, imgN[1].y, kTolP);
  }
}

/**
 * Test 2: cube
 */
TEST(stereomodel, ImageToGround_Cube) {
  const auto leftModelA = CreateRationalModelALeft();
  const auto rightModelA = CreateRationalModelARight();
  TestImageToGround_Cube(leftModelA.get(), rightModelA.get());

  //const auto leftModelB = CreateRationalModelBLeft();
  //const auto rightModelB = CreateRationalModelBRight();
  //TestImageToGround_Cube(leftModelB.get(), rightModelB.get());

  std::string leftMdlFileC = TestConfig::DataPath("Imagery/Satellite/zy302a_006911_258169_20170827/zy302a_fwd_006911_258169_20170827181359_01_sec_0001_1709018015_rpc.txt");
  std::string rightMdlFileC = TestConfig::DataPath("Imagery/Satellite/zy302a_006911_258169_20170827/zy302a_bwd_006911_258169_20170827181359_01_sec_0001_1709011888_rpc.txt");
  RationalModelMarshaller rmMarshaller;
  auto leftModelC = rmMarshaller.Deserialize(leftMdlFileC);
  auto rightModelC = rmMarshaller.Deserialize(rightMdlFileC);
  TestImageToGround_Cube(leftModelC.get(), rightModelC.get());
}

void TestImageToGround_Cube(const RationalModel* leftModel, const RationalModel* rightModel) {
  auto nfcL = leftModel->GetNormalFactors();
  const auto minLonL = nfcL[rnf::eOLONG] - nfcL[rnf::eSLONG] * kFactor;
  const auto maxLonL = nfcL[rnf::eOLONG] + nfcL[rnf::eSLONG] * kFactor;
  const auto minLatL = nfcL[rnf::eOLAT] - nfcL[rnf::eSLAT] * kFactor;
  const auto maxLatL = nfcL[rnf::eOLAT] + nfcL[rnf::eSLAT] * kFactor;
  const auto minHgtL = nfcL[rnf::eOHEIGHT] - nfcL[rnf::eSHEIGHT] * kFactor;
  const auto maxHgtL = nfcL[rnf::eOHEIGHT] + nfcL[rnf::eSHEIGHT] * kFactor;

  auto nfcR = rightModel->GetNormalFactors();
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

  Point3d maxDiffC(-1.0, -1.0, -1.0);
  Point3d maxDiffI(-1.0, -1.0, -1.0);
  Point3d maxDiffE(-1.0, -1.0, -1.0);
  Point3d maxDiffL(-1.0, -1.0, -1.0);
  Point3d maxDiffN(-1.0, -1.0, -1.0);

  StereoModel smC(leftModel, rightModel);    // Ceres 
  smC.SetForwardAlgorithm(IStereoModel::eSM_CERES);

  Point3d origin((minLon + maxLon) / 2, (minLat + maxLat) / 2, (minHgt + maxHgt) / 2);
  auto ee = std::make_shared<EarthEllipsoid>();
  ee->SetLsrOrigin(Deg2Rad(origin));
  StereoModel smI(leftModel, rightModel);    // intersect
  smI.SetEarthEllipsoid(ee);
  smI.SetForwardAlgorithm(IStereoModel::eCA_INTERSECT);

  StereoModel smE(leftModel, rightModel);    // EXAI 

  StereoModel smL(leftModel, rightModel);    // LS 
  smL.SetForwardAlgorithm(IStereoModel::eSM_LS);

  StereoModel smN(leftModel, rightModel);    // NR 
  smN.SetForwardAlgorithm(IStereoModel::eSM_NR);

  auto [leftNumCols, leftNumRows] = leftModel->GetImageSize();
  auto [rightNumCols, rightNumRows] = rightModel->GetImageSize();

  int n{};  // # of test points
  int nC{}; // # of successful Ceres
  int nI{}; // # of successful interactions
  int nE{}; // # of successful EXAI
  int nL{}; // # of LS.
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
        ret = smC.ImagesToGround(imageL, imageR, grdC);
        if (ret) {
          Larger(maxDiffC, xabs(grdC - ground));
          ++nC;
        }

        Point3d grdI{ ground };
        ret = smI.ImagesToGround(imageL, imageR, grdI);
        if (ret) {
          Larger(maxDiffI, xabs(grdI - ground));
          ++nI;
        }

        Point3d grdE{ ground };
        ret = smE.ImagesToGround(imageL, imageR, grdE);
        if (ret) {
          Larger(maxDiffE, xabs(grdE - ground));
          ++nE;
        }

        Point3d grdL{ ground };
        ret = smL.ImagesToGround(imageL, imageR, grdL);
        if (ret) {
          Larger(maxDiffL, xabs(grdL - ground));
          ++nL;
        }

        Point3d grdN{ ground };
        ret = smN.ImagesToGround(imageL, imageR, grdN);
        if (ret) {
          Larger(maxDiffN, xabs(grdN - ground));
          ++nN;
        }
        ++n;
      }
    }
  }

  ASSERT_LT(0, n)  << "Number of test points: " << n;
  ASSERT_EQ(nC, n) << "Number of Ceres points: " << nC;
  ASSERT_EQ(nI, n) << "Number of interaction points: " << nI;
  ASSERT_EQ(nE, n) << "Number of Ceres points: " << nE;
  ASSERT_EQ(nL, n) << "Number of NR points: " << nL;
  ASSERT_EQ(nN, n) << "Number of NR points: " << nN;

  ASSERT_LE(maxDiffC.x, kTolDEG) << "Max. longitude difference > 1 mm: " << maxDiffC.x;
  ASSERT_LE(maxDiffC.y, kTolDEG) << "Max.  latitude difference > 1 mm: " << maxDiffC.y;
  ASSERT_LE(maxDiffC.z, kTolMM) << "Max.    height difference > 1 mm: " << maxDiffC.z;
  ASSERT_LE(maxDiffI.x, kTolDEG) << "Max. longitude difference > 1 mm: " << maxDiffI.x;
  ASSERT_LE(maxDiffI.y, kTolDEG) << "Max.  latitude difference > 1 mm: " << maxDiffI.y;
  ASSERT_LE(maxDiffI.z, kTolMM) << "Max.    height difference > 1 mm: " << maxDiffI.z;
  ASSERT_LE(maxDiffE.x, kTolDEG) << "Max. longitude difference > 1 mm: " << maxDiffE.x;
  ASSERT_LE(maxDiffE.y, kTolDEG) << "Max.  latitude difference > 1 mm: " << maxDiffE.y;
  ASSERT_LE(maxDiffE.z, kTolMM) << "Max.    height difference > 1 mm: " << maxDiffE.z;
  ASSERT_LE(maxDiffL.x, kTolDEG) << "Max. longitude difference > 1 mm: " << maxDiffL.x;
  ASSERT_LE(maxDiffL.y, kTolDEG) << "Max.  latitude difference > 1 mm: " << maxDiffL.y;
  ASSERT_LE(maxDiffL.z, kTolMM) << "Max.    height difference > 1 mm: " << maxDiffL.z;
  ASSERT_LE(maxDiffN.x, kTolDEG) << "Max. longitude difference > 1 mm: " << maxDiffN.x;
  ASSERT_LE(maxDiffN.y, kTolDEG) << "Max.  latitude difference > 1 mm: " << maxDiffN.y;
  ASSERT_LE(maxDiffN.z, kTolMM) << "Max.    height difference > 1 mm: " << maxDiffN.z;
}

/**
 * Create an instance of rational model of left image
 */
std::unique_ptr<RationalModel> CreateRationalModelALeft() {
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
std::unique_ptr<RationalModel> CreateRationalModelARight() {
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
std::unique_ptr<RationalModel> CreateRationalModelBLeft() {
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
std::unique_ptr<RationalModel> CreateRationalModelBRight() {
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
