/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/utils/constants.h"
#include "testconfig.h"
#include "testutils.h"

#include "core/utils/fsutils.h"
#include "marshaller/sensormodel/rationalmodelmarshaller.h"
#include "sensormodel/rationalfit.h"
#include "sensormodel/utils/sensorcube.h"
#include "sensormodel/utils/smutils.h"

#include <filesystem>

using namespace earthxpert;
using namespace earthxpert::utils;
namespace fs = std::filesystem;

UPRationalModel CreateRationalModel(const ISensorModel* sensorModel, int dim, bool cc=true);
void TestRationalFit(const std::string& inRpc, const std::string& outRpc, int dim, bool cc=true);

TEST(rationalfit, Khalifasat5) {
  std::string inRpc{ "Imagery/Satellite/Khalifasat/KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_rpc.txt" };
  std::string outRpc{ "KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P0_5_rpc.txt" };
  TestRationalFit(inRpc, outRpc, 5, false);

  inRpc = "Imagery/Satellite/Khalifasat/KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_rpc.txt";
  outRpc = "KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_5_rpc.txt";
  TestRationalFit(inRpc, outRpc, 5);
}

TEST(rationalfit, Khalifasat10) {
  std::string inRpc{ "Imagery/Satellite/Khalifasat/KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_rpc.txt" };
  std::string outRpc{ "KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P0_10_rpc.txt" };
  TestRationalFit(inRpc, outRpc, 10, false);

  inRpc = "Imagery/Satellite/Khalifasat/KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_rpc.txt";
  outRpc = "KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_10_rpc.txt";
  TestRationalFit(inRpc, outRpc, 10);
}

TEST(rationalfit, Khalifasat20) {
  std::string inRpc{ "Imagery/Satellite/Khalifasat/KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_rpc.txt" };
  std::string outRpc{ "KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P0_20_rpc.txt" };
  TestRationalFit(inRpc, outRpc, 20, false);

  inRpc = "Imagery/Satellite/Khalifasat/KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_rpc.txt";
  outRpc = "KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_20_rpc.txt";
  TestRationalFit(inRpc, outRpc, 20);
}

TEST(rationalfit, WV2_Stereo1B_XJ5) {
  std::string inRpc{ "Imagery/Satellite/WV2_Stereo1B_XJ/15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN_rpc.txt" };
  std::string outRpc{ "15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN0_5_rpc.txt" };
  TestRationalFit(inRpc, outRpc, 5, false);

  inRpc = "Imagery/Satellite/WV2_Stereo1B_XJ/15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN_rpc.txt";
  outRpc = "15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN_5_rpc.txt";
  TestRationalFit(inRpc, outRpc, 5);
}

TEST(rationalfit, WV2_Stereo1B_XJ10) {
  std::string inRpc{ "Imagery/Satellite/WV2_Stereo1B_XJ/15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN_rpc.txt" };
  std::string outRpc{ "15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN0_10_rpc.txt" };
  TestRationalFit(inRpc, outRpc, 10, false);

  inRpc = "Imagery/Satellite/WV2_Stereo1B_XJ/15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN_rpc.txt";
  outRpc = "15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN_10_rpc.txt";
  TestRationalFit(inRpc, outRpc, 10);
}

TEST(rationalfit, WV2_Stereo1B_XJ20) {
  std::string inRpc{ "Imagery/Satellite/WV2_Stereo1B_XJ/15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN_rpc.txt" };
  std::string outRpc{ "15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN0_20_rpc.txt" };
  TestRationalFit(inRpc, outRpc, 20, false);

  inRpc = "Imagery/Satellite/WV2_Stereo1B_XJ/15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN_rpc.txt";
  outRpc = "15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN_20_rpc.txt";
  TestRationalFit(inRpc, outRpc, 20);
}

UPRationalModel CreateRationalModel(const ISensorModel* sensorModel, 
  const int dim, const bool cc) {
  std::vector<Point2f> images;
  std::vector<Point3d> grounds;

  SensorCube sensorCube(dim, dim, dim);
  sensorCube.CreateCube(sensorModel, images, grounds);

  RationalFit ratFit;
  if (!cc)
    ratFit.SetCrossCorelation(0, 0.0);
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

void TestRationalFit(const std::string& inRpc,
  const std::string& outRpc, const int dim, const bool cc) {
  std::string mdlFile = TestConfig::DataPath(inRpc);
  RationalModelMarshaller rmMarshaller;
  auto ratModel = rmMarshaller.Deserialize(mdlFile);
  ASSERT_TRUE(ratModel);

  if (cc)
    std::cout << "*** cross-correlation method ***\n";
  else
    std::cout << "*** standard method ***\n";

  auto ratModelF = CreateRationalModel(ratModel.get(), dim, cc); // 1st fit
  //CreateRationalModel(ratModelF.get());   // 2nd fit

  fs::path fitPath(TestConfig::OutputPath(outRpc));
  RemoveFile(fitPath);
  rmMarshaller.Serialize(fitPath.string(), ratModelF.get());
}
