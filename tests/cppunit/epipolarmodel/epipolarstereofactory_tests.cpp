/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main_gdal.h"
#include "testconfig.h"

#include "core/math/constants.h"
#include "core/math/mutils.h"
#include "epipolarmodel/epipolarmodel.h"
#include "epipolarmodel/epipolarstereofactory.h"
#include "marshaller/rationalmodelmarshaller.h"
#include "sensormodel/utils/stereomodel.h"

#include <algorithm>
#include <iostream>

using namespace earthxpert;

static constexpr int kNum = 5;
static constexpr double kFactor = 0.75;

static void CreateRationalEpipolarModel(const std::string& leftMdlFile,
  const std::string& rightMdlFile, EpiModelPair& epiModels);
static void TestImageToGround_Cube(const IEpipolarModel* leftEpiModel, 
  const IEpipolarModel* rightEpiModel, const double tol = kTolDEG, 
  const double tolH = kTolMM);

TEST(epipolarmodelfactory, 052522862010_01_P001_PAN) {
  std::string leftMdlFile = TestConfig::DataPath("Imagery/Satellite/052522862010_01_P001_PAN/09JAN15034657-P2AS-052522862010_01_P001.RPB");
  std::string rightMdlFile = TestConfig::DataPath("Imagery/Satellite/052522862010_01_P001_PAN/09JAN15034801-P2AS-052522862010_01_P001.RPB");
  
  EpiModelPair epiModels;
  CreateRationalEpipolarModel(leftMdlFile, rightMdlFile, epiModels);

  auto leftEpiModel{ epiModels.first.get() };
  auto rightEpiModel{ epiModels.second.get() };

  auto [lEpiNumCols, lEpiNumRows] = leftEpiModel->GetImageSize();
  ASSERT_EQ(24272, lEpiNumCols);
  ASSERT_EQ(21856, lEpiNumRows);
  auto [lRawNumCols, lRawNumRows] = leftEpiModel->GetRawImageSize();
  ASSERT_EQ(19274, lRawNumCols);
  ASSERT_EQ(23048, lRawNumRows);

  auto [rEpiNumCols, rEpiNumRows] = rightEpiModel->GetImageSize();
  ASSERT_EQ(24272, rEpiNumCols);
  ASSERT_EQ(21856, rEpiNumRows);
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
  ASSERT_EQ(12976, lEpiNumCols);
  ASSERT_EQ(12304, lEpiNumRows);
  auto [lRawNumCols, lRawNumRows] = leftEpiModel->GetRawImageSize();
  ASSERT_EQ(12122, lRawNumCols);
  ASSERT_EQ(13148, lRawNumRows);

  auto [rEpiNumCols, rEpiNumRows] = rightEpiModel->GetImageSize();
  ASSERT_EQ(12976, rEpiNumCols);
  ASSERT_EQ(12304, rEpiNumRows);
  auto [rRawNumCols, rRawNumRows] = rightEpiModel->GetRawImageSize();
  ASSERT_EQ(12122, rRawNumCols);
  ASSERT_EQ(13148, rRawNumRows);

  TestImageToGround_Cube(epiModels.first.get(), epiModels.second.get(), kTolDEG, 3.0e-3);
}

TEST(epipolarmodelfactory, zy302a_006911_258169_20170827) {
  std::string leftMdlFile = TestConfig::DataPath("Imagery/Satellite/zy302a_006911_258169_20170827/zy302a_fwd_006911_258169_20170827181359_01_sec_0001_1709018015_rpc.txt");
  std::string rightMdlFile = TestConfig::DataPath("Imagery/Satellite/zy302a_006911_258169_20170827/zy302a_bwd_006911_258169_20170827181359_01_sec_0001_1709011888_rpc.txt");

  EpiModelPair epiModels;
  CreateRationalEpipolarModel(leftMdlFile, rightMdlFile, epiModels);

  auto leftEpiModel{ epiModels.first.get() };
  auto rightEpiModel{ epiModels.second.get() };

  auto [lEpiNumCols, lEpiNumRows] = leftEpiModel->GetImageSize();
  ASSERT_EQ(20736, lEpiNumCols);
  ASSERT_EQ(22912, lEpiNumRows);
  auto [lRawNumCols, lRawNumRows] = leftEpiModel->GetRawImageSize();
  ASSERT_EQ(24512, lRawNumCols);
  ASSERT_EQ(19996, lRawNumRows);

  auto [rEpiNumCols, rEpiNumRows] = rightEpiModel->GetImageSize();
  ASSERT_EQ(20736, rEpiNumCols);
  ASSERT_EQ(22912, rEpiNumRows);
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

  EpipolarStereoFactory epiModelFac(FacEpiOpt::eFULL);
  auto [leftEpiModel, rightEpiModel] = epiModelFac.CreateRationalEpiStereo(leftModel, rightModel);
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

  auto nfcL = leftRM->GetNormalFactors();
  const auto minLonL = nfcL[rnf::eOLON] - nfcL[rnf::eSLON] * kFactor;
  const auto maxLonL = nfcL[rnf::eOLON] + nfcL[rnf::eSLON] * kFactor;
  const auto minLatL = nfcL[rnf::eOLAT] - nfcL[rnf::eSLAT] * kFactor;
  const auto maxLatL = nfcL[rnf::eOLAT] + nfcL[rnf::eSLAT] * kFactor;
  const auto minHgtL = nfcL[rnf::eOHGT] - nfcL[rnf::eSHGT] * kFactor;
  const auto maxHgtL = nfcL[rnf::eOHGT] + nfcL[rnf::eSHGT] * kFactor;

  auto nfcR = rightRM->GetNormalFactors();
  const auto minLonR = nfcR[rnf::eOLON] - nfcR[rnf::eSLON] * kFactor;
  const auto maxLonR = nfcR[rnf::eOLON] + nfcR[rnf::eSLON] * kFactor;
  const auto minLatR = nfcR[rnf::eOLAT] - nfcR[rnf::eSLAT] * kFactor;
  const auto maxLatR = nfcR[rnf::eOLAT] + nfcR[rnf::eSLAT] * kFactor;
  const auto minHgtR = nfcR[rnf::eOHGT] - nfcR[rnf::eSHGT] * kFactor;
  const auto maxHgtR = nfcR[rnf::eOHGT] + nfcR[rnf::eSHGT] * kFactor;

  const auto minLon = std::max(minLonL, minLonR);
  const auto maxLon = std::min(maxLonL, maxLonR);
  const auto minLat = std::max(minLatL, minLatR);
  const auto maxLat = std::min(maxLatL, maxLatR);
  const auto minHgt = std::max(minHgtL, minHgtR);
  const auto maxHgt = std::min(maxHgtL, maxHgtR);

  const auto stepLon = (maxLon - minLon) / kNum;
  const auto stepLat = (maxLat - minLat) / kNum;
  const auto stepHgt = (maxHgt - minHgt) / kNum;

  Point3d maxDiffN(-1.0, -1.0, -1.0);

  auto leftModel = dynamic_cast<const SensorEpipolarModel*>(leftEpiModel);
  auto rightModel = dynamic_cast<const SensorEpipolarModel*>(rightEpiModel);
  StereoModel smN(leftModel, rightModel);    // NR 
  smN.SetForwardAlgorithm(IStereoModel::eSM_NR);

  auto [leftEpiNumCols, leftEpiNumRows] = leftEpiModel->GetImageSize();
  auto [rightEpiNumCols, rightEpiNumRows] = rightEpiModel->GetImageSize();

  auto [leftRawNumCols, leftRawNumRows] = leftEpiModel->GetRawImageSize();
  auto [rightRawNumCols, rightRawNumRows] = rightEpiModel->GetRawImageSize();

  int n{};  // # of test points
  int nN{}; // # of NR.

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
        const bool ret = smN.ImagesToGround(imageL, imageR, grdN);
        if (ret) {
          Larger(maxDiffN, xabs(grdN - ground));
          ++nN;
        }

        ++n;
      }
    }
  }

  ASSERT_LT(0, n) << "Number of test points: " << n;
  ASSERT_EQ(nN, n) << "Number of NR points: " << nN;

  ASSERT_LE(maxDiffN.x, tol) << "Max. longitude difference > 1 mm: " << maxDiffN.x;
  ASSERT_LE(maxDiffN.y, tol) << "Max.  latitude difference > 1 mm: " << maxDiffN.y;
  ASSERT_LE(maxDiffN.z, tolH) << "Max.    height difference > 1 mm: " << maxDiffN.z;
}
