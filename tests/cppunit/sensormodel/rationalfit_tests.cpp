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
#include "sensormodel/sensormodelfactory.h"
#include "sensormodel/utils/sensorcube.h"
#include "sensormodel/utils/smutils.h"

#include <filesystem>

using namespace earthxpert;
using namespace earthxpert::utils;
namespace fs = std::filesystem;

void TestRationalFit(const std::string& inRpc, const std::string& outRpc, int dim, bool cc=false);

/*
TEST(rationalfit, Khalifasat3) {
  std::string inRpc{ "Imagery/Satellite/Khalifasat/KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_rpc.txt" };
  std::string outRpc{ "KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P0_3_rpc.txt" };
  TestRationalFit(inRpc, outRpc, 3, false);

  inRpc = "Imagery/Satellite/Khalifasat/KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_rpc.txt";
  outRpc = "KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_3_rpc.txt";
  TestRationalFit(inRpc, outRpc, 3, true);
}

TEST(rationalfit, Khalifasat4) {
  std::string inRpc{ "Imagery/Satellite/Khalifasat/KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_rpc.txt" };
  std::string outRpc{ "KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P0_4_rpc.txt" };
  TestRationalFit(inRpc, outRpc, 4, false);

  inRpc = "Imagery/Satellite/Khalifasat/KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_rpc.txt";
  outRpc = "KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_4_rpc.txt";
  TestRationalFit(inRpc, outRpc, 4);
}

TEST(rationalfit, Khalifasat5) {
  std::string inRpc{ "Imagery/Satellite/Khalifasat/KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_rpc.txt" };
  std::string outRpc{ "KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P0_5_rpc.txt" };
  TestRationalFit(inRpc, outRpc, 5, false);

  inRpc = "Imagery/Satellite/Khalifasat/KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_rpc.txt";
  outRpc = "KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_5_rpc.txt";
  TestRationalFit(inRpc, outRpc, 5, true);
}

TEST(rationalfit, Khalifasat10) {
  std::string inRpc{ "Imagery/Satellite/Khalifasat/KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_rpc.txt" };
  std::string outRpc{ "KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P0_10_rpc.txt" };
  TestRationalFit(inRpc, outRpc, 10, false);

  inRpc = "Imagery/Satellite/Khalifasat/KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_rpc.txt";
  outRpc = "KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_10_rpc.txt";
  TestRationalFit(inRpc, outRpc, 10, true);
}
*/
TEST(rationalfit, Khalifasat20) {
  std::string inRpc{ "Imagery/Satellite/Khalifasat/KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_rpc.txt" };
  std::string outRpc{ "KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P0_20_rpc.txt" };
  TestRationalFit(inRpc, outRpc, 20, false);

  inRpc = "Imagery/Satellite/Khalifasat/KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_rpc.txt";
  outRpc = "KhalifaSat_20190704093359_03703_UAE 15_11121_Radio_P_20_rpc.txt";
  TestRationalFit(inRpc, outRpc, 20, true);
}
/*
TEST(rationalfit, WV2_Stereo1B_XJ5) {
  std::string inRpc{ "Imagery/Satellite/WV2_Stereo1B_XJ/15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN_rpc.txt" };
  std::string outRpc{ "15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN0_5_rpc.txt" };
  TestRationalFit(inRpc, outRpc, 5, false);

  inRpc = "Imagery/Satellite/WV2_Stereo1B_XJ/15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN_rpc.txt";
  outRpc = "15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN_5_rpc.txt";
  TestRationalFit(inRpc, outRpc, 5, true);
}

TEST(rationalfit, WV2_Stereo1B_XJ10) {
  std::string inRpc{ "Imagery/Satellite/WV2_Stereo1B_XJ/15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN_rpc.txt" };
  std::string outRpc{ "15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN0_10_rpc.txt" };
  TestRationalFit(inRpc, outRpc, 10, false);

  inRpc = "Imagery/Satellite/WV2_Stereo1B_XJ/15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN_rpc.txt";
  outRpc = "15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN_10_rpc.txt";
  TestRationalFit(inRpc, outRpc, 10, true);
}
TEST(rationalfit, WV2_Stereo1B_XJ20) {
  std::string inRpc{ "Imagery/Satellite/WV2_Stereo1B_XJ/15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN_rpc.txt" };
  std::string outRpc{ "15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN0_20_rpc.txt" };
  TestRationalFit(inRpc, outRpc, 20, false);

  inRpc = "Imagery/Satellite/WV2_Stereo1B_XJ/15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN_rpc.txt";
  outRpc = "15AUG02052758-1BS_R1C1-054355223040_05_P003_RAW_PAN_20_rpc.txt";
  TestRationalFit(inRpc, outRpc, 20, true);
}
*/

void TestRationalFit(const std::string& inRpc,
  const std::string& outRpc, const int dim, const bool cc) {
  std::string mdlFile = TestConfig::DataPath(inRpc);
  RationalModelMarshaller rmMarshaller;
  auto ratModel = rmMarshaller.Deserialize(mdlFile);
  ASSERT_TRUE(ratModel);

  if (cc)
    std::cout << "\n*** cross-correlation method ***\n";
  else
    std::cout << "\n*** standard method ***\n";

  auto ratModelF = SensorModelFactory::CreateRationalModel(ratModel.get(), dim, dim, 0, cc); // 1st fit
  //CreateRationalModel(ratModelF.get());   // 2nd fit

  if (outRpc.empty()) return;
  fs::path fitPath(TestConfig::OutputPath(outRpc));
  RemoveFile(fitPath);
  rmMarshaller.Serialize(fitPath.string(), ratModelF.get());
}
