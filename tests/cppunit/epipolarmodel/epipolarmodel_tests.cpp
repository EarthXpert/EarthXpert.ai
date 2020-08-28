/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main_gdal.h"
#include "testconfig.h"

#include "core/math/mutils.h"
#include "epipolarmodel/epipolarmodel.h"
#include "epipolarmodel/epipolarstereofactory.h"
#include "epipolarmodel/nullepipolarmodel.h"
#include "marshaller/rationalmodelmarshaller.h"
#include "sensormodel/utils/smutils.h"

#include <iostream>

using namespace earthxpert;

static constexpr int kGridStride = 128;
static constexpr double kTol = 1.0e-3; // [mm]

static void CreateRationalEpipolarModel(const std::string& leftMdlFile,
  const std::string& rightMdlFile, EpiModelPair& epiModels);
static void TestRaw2Epi_Epi2Raw_Grid(const IEpipolarModel* epiModel, double tol=kTol);
static void TestStride(const IEpipolarModel* epiModel, int stride, double tol = kTol);

static void TestCubeI2G(const ISensorModel* epiSensorModel);
static void TestImageToGround_Cube(const ISensorModel* leftEpiSensorModel,
  const ISensorModel* rightSensorModel);

/**
 *  Test 1: WV01
 */
TEST(epipolarmodel, 052522862010_01_P001_PAN) {
  std::string leftMdlFile = TestConfig::DataPath("Imagery/Satellite/052522862010_01_P001_PAN/09JAN15034657-P2AS-052522862010_01_P001.RPB");
  std::string rightMdlFile = TestConfig::DataPath("Imagery/Satellite/052522862010_01_P001_PAN/09JAN15034801-P2AS-052522862010_01_P001.RPB");

  EpiModelPair epiModels;
  CreateRationalEpipolarModel(leftMdlFile, rightMdlFile, epiModels);

  TestRaw2Epi_Epi2Raw_Grid(epiModels.first.get());
  TestRaw2Epi_Epi2Raw_Grid(epiModels.second.get());
}

/**
 *  Test 2: ZY3
 */
TEST(epipolarmodel, zy302a_006911_258169_20170827) {
  std::string leftMdlFile = TestConfig::DataPath("Imagery/Satellite/zy302a_006911_258169_20170827/zy302a_fwd_006911_258169_20170827181359_01_sec_0001_1709018015_rpc.txt");
  std::string rightMdlFile = TestConfig::DataPath("Imagery/Satellite/zy302a_006911_258169_20170827/zy302a_bwd_006911_258169_20170827181359_01_sec_0001_1709011888_rpc.txt");

  EpiModelPair epiModels;
  CreateRationalEpipolarModel(leftMdlFile, rightMdlFile, epiModels);

  TestRaw2Epi_Epi2Raw_Grid(epiModels.first.get(), 0.001);  // [1cm]
  TestRaw2Epi_Epi2Raw_Grid(epiModels.second.get(), 0.001);
}

// ref epipolarmodelfactory_tests
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

void TestRaw2Epi_Epi2Raw_Grid(const IEpipolarModel* epiModel, const double tol) {
  auto [rawNumCols, rawNumRows] = epiModel->GetRawImageSize();

  Point2d maxDiffCR(-1.0, -1.0);

  Point2d epi, rawN;
  for (int col = 0; col < rawNumCols; col+=kGridStride)
    for (int row = 0; row < rawNumRows; row+=kGridStride) {
      Point2d raw(col, row);
      epiModel->RawToEpi(raw, epi);
      epiModel->EpiToRaw(epi, rawN);

      Larger(maxDiffCR, xabs(rawN - raw));
    }

  auto sensorModel = epiModel->GetSensorModel();
  auto gsd = CalcGsd(sensorModel, sensorModel->GetMeanHeight());

  auto maxDiff = maxDiffCR * gsd;
  ASSERT_LT(maxDiff.x, tol) << "max diiference in x < 1mm";
  ASSERT_LT(maxDiff.y, tol) << "max diiference in y < 1mm";
}

/**
 *  \brief first, do Epi2Raw at 1:1; 
 *         second, do bi-linear interpolation at given stride;
 *         third, calculate the maximum differences between them.
 */
void TestStride(const IEpipolarModel* epiModel, int stride, double tol) {

}

/**
 *  \breif sensor model of epipolar image via the chain between
 *  epipolar image space, rectification space, and raw image space.
 */
void TestCubeI2G(const ISensorModel* epiSensorModel) {

}

/**
 *  \breif stereo model composed of epipolar images as OrthoReady product
 */
void TestImageToGround_Cube(const ISensorModel* leftEpiSensorModel,
  const ISensorModel* rightSensorModel) {

}
