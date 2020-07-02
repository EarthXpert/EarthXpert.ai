/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main_gdal.h"
#include "testconfig.h"

#include "epipolarmodel/epipolarmodel.h"
#include "epipolarmodel/epipolarmodelfactory.h"
#include "marshaller/rationalmodelmarshaller.h"

#include <iostream>

using namespace earthxpert;

static void TestRationalEpipolarModel(const std::string& leftMdlFile, 
  const std::string& rightMdlFile);

TEST(epipolarmodelfactory, 052522862010_01_P001_PAN) {
  std::string leftMdlFile = "D:/home/workspace/TestData/Imagery/Satellite/052522862010_01_P001_PAN/09JAN15034657-P2AS-052522862010_01_P001.RPB";
  std::string rightMdlFile = "D:/home/workspace/TestData/Imagery/Satellite/052522862010_01_P001_PAN/09JAN15034801-P2AS-052522862010_01_P001.RPB";
  TestRationalEpipolarModel(leftMdlFile, rightMdlFile);
}

TEST(epipolarmodelfactory, po_162796_pan) {
  std::string leftMdlFile = "D:/home/workspace/TestData/Imagery/Satellite/po_162796_pan/po_162796_pan_0000000_rpc.txt";
  std::string rightMdlFile = "D:/home/workspace/TestData/Imagery/Satellite/po_162796_pan/po_162796_pan_0010000_rpc.txt";
  TestRationalEpipolarModel(leftMdlFile, rightMdlFile);
}

void TestRationalEpipolarModel(const std::string& leftMdlFile,
  const std::string& rightMdlFile) {
  RationalModelMarshaller rmMarshaller;
  std::shared_ptr<ISensorModel> leftModel = rmMarshaller.Deserialize(leftMdlFile);
  ASSERT_TRUE(SensorType::eRATIONAL == leftModel->GetModelType());
  std::shared_ptr<ISensorModel> rightModel = rmMarshaller.Deserialize(rightMdlFile);
  ASSERT_TRUE(SensorType::eRATIONAL == rightModel->GetModelType());

  EpipolarModelFactory epiModelFac(FacEpiOpt::eFULL);
  auto [leftEpiModel, rightEpiModel] = epiModelFac.CreateEpipolarModel(leftModel, rightModel);
  ASSERT_TRUE(IEpipolarModel::ePLANERECT == leftEpiModel->GetEpipolarAlgorithm());
  ASSERT_TRUE(IEpipolarModel::ePLANERECT == rightEpiModel->GetEpipolarAlgorithm());

  auto lEpiMdl = dynamic_cast<GeoEpipolarModel*>(leftEpiModel.get());
  auto lEpiRect = lEpiMdl->GetEpiRect();
  auto rEpiMdl = dynamic_cast<GeoEpipolarModel*>(rightEpiModel.get());
  auto rEpiRect = rEpiMdl->GetEpiRect();

  auto lRot = lEpiRect->GetRotation();
  auto rRot = rEpiRect->GetRotation();
  ASSERT_DOUBLE_EQ(lRot, rRot) << "Epipolar rotation";
  std::cout << "Epipolar Rotation: " << Rad2Deg(lRot) << std::endl;

  auto lNumRows = lEpiRect->GetNumRows();
  auto rNumRows = rEpiRect->GetNumRows();
  ASSERT_DOUBLE_EQ(lNumRows, rNumRows) << "Epipolar rows";
  std::cout << "Epipolar Rows: " << lNumRows << std::endl;
}