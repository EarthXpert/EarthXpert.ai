/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main_gdal.h"
#include "testconfig.h"

#include "epipolarmodel/epipolarmodel.h"
#include "epipolarmodel/epipolarmodelfactory.h"
#include "marshaller/rationalmodelmarshaller.h"
#include "sensormodel/stereomodel.h"

#include <algorithm>
#include <iostream>

using namespace earthxpert;

static constexpr double kTol = 1.0e-8; // [mm]
static constexpr double kTolH = 1.0e-3; // [mm]
static constexpr int kNum = 5;
static constexpr double kFactor = 0.75;

static void CreateRationalEpipolarModel(const std::string& leftMdlFile,
  const std::string& rightMdlFile, EpiModelPair& epiModels);
static void TestImageToGround_Cube(const IEpipolarModel* leftEpiModel, 
  const IEpipolarModel* rightEpiModel, const double tol = kTol, 
  const double tolH = kTolH);

TEST(epipolarmodelfactory, 052522862010_01_P001_PAN) {
  std::string leftMdlFile = TestConfig::DataPath("Imagery/Satellite/052522862010_01_P001_PAN/09JAN15034657-P2AS-052522862010_01_P001.RPB");
  std::string rightMdlFile = TestConfig::DataPath("Imagery/Satellite/052522862010_01_P001_PAN/09JAN15034801-P2AS-052522862010_01_P001.RPB");
  
  EpiModelPair epiModels;
  CreateRationalEpipolarModel(leftMdlFile, rightMdlFile, epiModels);

  auto leftEpiModel{ epiModels.first.get() };
  auto rightEpiModel{ epiModels.second.get() };

  auto [lEpiNumCols, lEpiNumRows] = leftEpiModel->GetImageSize();
  ASSERT_EQ(24266, lEpiNumCols);
  ASSERT_EQ(21829, lEpiNumRows);
  auto [lRawNumCols, lRawNumRows] = leftEpiModel->GetRawImageSize();
  ASSERT_EQ(19274, lRawNumCols);
  ASSERT_EQ(23048, lRawNumRows);

  auto [rEpiNumCols, rEpiNumRows] = rightEpiModel->GetImageSize();
  ASSERT_EQ(24266, rEpiNumCols);
  ASSERT_EQ(21829, rEpiNumRows);
  auto [rRawNumCols, rRawNumRows] = rightEpiModel->GetRawImageSize();
  ASSERT_EQ(19274, rRawNumCols);
  ASSERT_EQ(23048, rRawNumRows);

  TestImageToGround_Cube(leftEpiModel, rightEpiModel);
}

TEST(epipolarmodelfactory, po_162796_pan) {
  std::string leftMdlFile = TestConfig::DataPath("Imagery/Satellite/po_162796_pan/po_162796_pan_0000000_rpc.txt");
  std::string rightMdlFile = TestConfig::DataPath("Imagery/Satellite/po_162796_pan/po_162796_pan_0010000_rpc.txt");

  EpiModelPair epiModels;
  CreateRationalEpipolarModel(leftMdlFile, rightMdlFile, epiModels);

  auto leftEpiModel{ epiModels.first.get() };
  auto rightEpiModel{ epiModels.second.get() };

  auto [lEpiNumCols, lEpiNumRows] = leftEpiModel->GetImageSize();
  ASSERT_EQ(12947, lEpiNumCols);
  ASSERT_EQ(12295, lEpiNumRows);
  auto [lRawNumCols, lRawNumRows] = leftEpiModel->GetRawImageSize();
  ASSERT_EQ(12122, lRawNumCols);
  ASSERT_EQ(13148, lRawNumRows);

  auto [rEpiNumCols, rEpiNumRows] = rightEpiModel->GetImageSize();
  ASSERT_EQ(12947, rEpiNumCols);
  ASSERT_EQ(12295, rEpiNumRows);
  auto [rRawNumCols, rRawNumRows] = rightEpiModel->GetRawImageSize();
  ASSERT_EQ(12122, rRawNumCols);
  ASSERT_EQ(13148, rRawNumRows);

  TestImageToGround_Cube(epiModels.first.get(), epiModels.second.get(), kTol, 3.0e-3);
}

TEST(epipolarmodelfactory, zy302a_006911_258169_20170827) {
  std::string leftMdlFile = TestConfig::DataPath("Imagery/Satellite/zy302a_006911_258169_20170827/zy302a_fwd_006911_258169_20170827181359_01_sec_0001_1709018015_rpc.txt");
  std::string rightMdlFile = TestConfig::DataPath("Imagery/Satellite/zy302a_006911_258169_20170827/zy302a_bwd_006911_258169_20170827181359_01_sec_0001_1709011888_rpc.txt");

  EpiModelPair epiModels;
  CreateRationalEpipolarModel(leftMdlFile, rightMdlFile, epiModels);

  auto leftEpiModel{ epiModels.first.get() };
  auto rightEpiModel{ epiModels.second.get() };

  auto [lEpiNumCols, lEpiNumRows] = leftEpiModel->GetImageSize();
  ASSERT_EQ(20715, lEpiNumCols);
  ASSERT_EQ(22898, lEpiNumRows);
  auto [lRawNumCols, lRawNumRows] = leftEpiModel->GetRawImageSize();
  ASSERT_EQ(24512, lRawNumCols);
  ASSERT_EQ(19996, lRawNumRows);

  auto [rEpiNumCols, rEpiNumRows] = rightEpiModel->GetImageSize();
  ASSERT_EQ(20715, rEpiNumCols);
  ASSERT_EQ(22898, rEpiNumRows);
  auto [rRawNumCols, rRawNumRows] = rightEpiModel->GetRawImageSize();
  ASSERT_EQ(24512, rRawNumCols);
  ASSERT_EQ(19996, rRawNumRows);

  TestImageToGround_Cube(epiModels.first.get(), epiModels.second.get(), 3.0e-8, 2.0e-2);
}

void CreateRationalEpipolarModel(const std::string& leftMdlFile,
  const std::string& rightMdlFile, EpiModelPair& epiModels) {
  RationalModelMarshaller rmMarshaller;
  std::shared_ptr<RationalModel> leftModel = rmMarshaller.Deserialize(leftMdlFile);
  std::shared_ptr<RationalModel> rightModel = rmMarshaller.Deserialize(rightMdlFile);

  EpipolarModelFactory epiModelFac(FacEpiOpt::eFULL);
  auto [leftEpiModel, rightEpiModel] = epiModelFac.CreateRationalEpiModel(leftModel, rightModel);
  ASSERT_TRUE(IEpipolarModel::ePLANERECT == leftEpiModel->GetEpipolarAlgorithm());
  ASSERT_TRUE(IEpipolarModel::ePLANERECT == rightEpiModel->GetEpipolarAlgorithm());

  auto lEpiMdl = dynamic_cast<GeoEpipolarModel*>(leftEpiModel.get());
  auto lEpiRect = lEpiMdl->GetEpiRect();
  auto rEpiMdl = dynamic_cast<GeoEpipolarModel*>(rightEpiModel.get());
  auto rEpiRect = rEpiMdl->GetEpiRect();

  auto lRot = lEpiRect->GetRotation();
  auto rRot = rEpiRect->GetRotation();
  ASSERT_DOUBLE_EQ(lRot, rRot) << "Epipolar rotation";
  std::cout << "Epipolar Rotation [deg]: " << Rad2Deg(lRot) << std::endl;

  auto lNumRows = lEpiRect->GetNumRows();
  auto rNumRows = rEpiRect->GetNumRows();
  ASSERT_DOUBLE_EQ(lNumRows, rNumRows) << "Epipolar rows";
  //std::cout << "Epipolar Rows: " << lNumRows << std::endl;

  epiModels.first = std::move(leftEpiModel);
  epiModels.second = std::move(rightEpiModel);
}

void TestImageToGround_Cube(const IEpipolarModel* leftEpiModel,
  const IEpipolarModel* rightEpiModel, const double tol, const double tolH) {
  auto leftRM = dynamic_cast<const RationalModel*>(leftEpiModel->GetSensorModel());
  auto rightRM = dynamic_cast<const RationalModel*>(rightEpiModel->GetSensorModel());

  auto nfcL = leftRM->GetNormalFators();
  const auto minLonL = nfcL[rnf::eOLONG] - nfcL[rnf::eSLONG] * kFactor;
  const auto maxLonL = nfcL[rnf::eOLONG] + nfcL[rnf::eSLONG] * kFactor;
  const auto minLatL = nfcL[rnf::eOLAT] - nfcL[rnf::eSLAT] * kFactor;
  const auto maxLatL = nfcL[rnf::eOLAT] + nfcL[rnf::eSLAT] * kFactor;
  const auto minHgtL = nfcL[rnf::eOHEIGHT] - nfcL[rnf::eSHEIGHT] * kFactor;
  const auto maxHgtL = nfcL[rnf::eOHEIGHT] + nfcL[rnf::eSHEIGHT] * kFactor;

  auto nfcR = rightRM->GetNormalFators();
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

  auto leftModel = dynamic_cast<const EpipolarSensorModel*>(leftEpiModel);
  auto rightModel = dynamic_cast<const EpipolarSensorModel*>(rightEpiModel);
  StereoModel smN(leftModel, rightModel);    // NR 
  smN.SetForwardAlgorithm(IStereoModel::eSM_NR);

  auto [leftEpiNumCols, leftEpiNumRows] = leftEpiModel->GetImageSize();
  auto [rightEpiNumCols, rightEpiNumRows] = rightEpiModel->GetImageSize();

  auto [leftRawNumCols, leftRawNumRows] = leftEpiModel->GetRawImageSize();
  auto [rightRawNumCols, rightRawNumRows] = rightEpiModel->GetRawImageSize();

  int n{};  // # of test points
  int nN{}; // # of NR.

  bool ret;
  double diffLon{}, diffLat{}, diffHgt{};

  for (auto lon = minLon; lon <= maxLon; lon += stepLon) {
    for (auto lat = minLat; lat <= maxLat; lat += stepLat) {
      for (auto hgt = minHgt; hgt <= maxHgt; hgt += stepHgt) {
        Point3d ground{ lon, lat, hgt };

        Point2d imageL;
        leftRM->GroundToImage(ground, imageL);
        if (0 > imageL.x || imageL.x > leftRawNumCols || 0 > imageL.y || imageL.y > leftRawNumRows)
          continue;
        leftModel->GroundToImage(ground, imageL);
        if (0 > imageL.x || imageL.x > leftEpiNumCols || 0 > imageL.y || imageL.y > leftEpiNumRows)
          continue;

        Point2d imageR;
        rightRM->GroundToImage(ground, imageR);
        if (0 > imageR.x || imageR.x > rightRawNumCols || 0 > imageR.y || imageR.y > rightRawNumRows)
          continue;
        rightModel->GroundToImage(ground, imageR);
        if (0 > imageR.x || imageR.x > rightEpiNumCols || 0 > imageR.y || imageR.y > rightEpiNumRows)
          continue;

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

  ASSERT_LT(0, n) << "Number of test points: " << n;
  ASSERT_EQ(nN, n) << "Number of NR points: " << nN;

  ASSERT_LE(maxDiffLonN, tol) << "Max. longitude difference > 1 mm: " << maxDiffLonN;
  ASSERT_LE(maxDiffLatN, tol) << "Max.  latitude difference > 1 mm: " << maxDiffLatN;
  ASSERT_LE(maxDiffHgtN, tolH) << "Max.    height difference > 1 mm: " << maxDiffHgtN;
}
