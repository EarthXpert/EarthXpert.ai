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
#include "sensormodel/utils/smutils.h"

using namespace earthxpert;

UPRationalModel CreateRationalModel(const ISensorModel* sensorModel);

TEST(rationalfit, CalcCoefs) {
  std::string mdlFile = TestConfig::DataPath("Imagery/Satellite/Khalifasat/KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_rpc.txt");
  RationalModelMarshaller rmMarshaller;
  auto ratModel = rmMarshaller.Deserialize(mdlFile);
  ASSERT_TRUE(ratModel);

  auto ratModelF = CreateRationalModel(ratModel.get()); // 1st fit

  //CreateRationalModel(ratModelF.get());   // 2nd fit
}

UPRationalModel CreateRationalModel(const ISensorModel* sensorModel) {
  std::vector<Point2f> images;
  std::vector<Point3d> grounds;

  SensorCube sensorCube(12, 12, 12);
  sensorCube.CreateCube(sensorModel, images, grounds);

  RationalFit ratFit;
  auto ratModelF = ratFit.CreateRationalModel(images, grounds);

  auto print = [](const std::array<double, 4>& stats, const std::string& name) {
    std::cout << name << " mean: " << stats[0] << " rms: " << stats[1]
      << " min: " << stats[2] << " max: " << stats[3] << std::endl;
  };

  // original accuray
  std::array<double, 4> sstats{}, lstats{};
  CalcStatitics(sensorModel, images, grounds, sstats, lstats);
  print(lstats, "original model line -");
  print(sstats, "original model samp -");

  // fit accuray
  std::array<double, 4> sstatsF{}, lstatsF{};
  CalcStatitics(ratModelF.get(), images, grounds, sstatsF, lstatsF);
  print(lstatsF, "fitted model line -");
  print(sstatsF, "fitted model samp -");

  // QC cube
  std::vector<Point2f> imagesQC;
  std::vector<Point3d> groundsQC;
  sensorCube.CreateCubeQC(sensorModel, imagesQC, groundsQC);

  // original accuracy QC
  std::array<double, 4> sstatsQC{}, lstatsQC{};
  CalcStatitics(sensorModel, imagesQC, groundsQC, sstatsQC, lstatsQC);
  print(lstatsQC, "original model line QC -");
  print(sstatsQC, "original model samp QC -");

  // Fit accuracy QC
  std::array<double, 4> sstatsFQC{}, lstatsFQC{};
  CalcStatitics(ratModelF.get(), imagesQC, groundsQC, sstatsFQC, lstatsFQC);
  print(lstatsFQC, "fitted model line QC -");
  print(sstatsFQC, "fitted model samp QC -");

  return ratModelF;
}
