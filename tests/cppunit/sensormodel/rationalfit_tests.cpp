/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/utils/constants.h"
#include "testconfig.h"
#include "testutils.h"

#include "marshaller/sensormodel/rationalmodelmarshaller.h"
#include "sensormodel/rationalfit.h"
#include "sensormodel/utils/sensorcube.h"

using namespace earthxpert;

TEST(rationalfit, CalcCoefs) {
  std::string mdlFile = TestConfig::DataPath("Imagery/Satellite/Khalifasat/KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_rpc.txt");
  RationalModelMarshaller rmMarshaller;
  auto ratModel = rmMarshaller.Deserialize(mdlFile);

  std::vector<Point2f> images;
  std::vector<Point3d> grounds;

  SensorCube sensorCube(11, 11, 11);
  sensorCube.CreateCube(ratModel.get(), images, grounds);

  RationalFit ratFit;
  ratFit.CreateRationalModel(images, grounds);

  std::array<double, NumFact> normalFactors;
  ratFit.GetNormalFactors(normalFactors);
}
