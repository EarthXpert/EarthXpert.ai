#include "3rd_party/gtest/gtest_main.h"
#include "testconfig.h"

#include "io/marshaller/epipolarmodelmarshaller.h"
#include "io/marshaller/framemodelmarshaller.h"
#include "io/marshaller/rationalmodelmarshaller.h"

#include <iostream>

using namespace watxtract;

TEST(marshaller, epipolarmodel) {
  RationalModelMarshaller rmarshaller;
  auto rationalmodel =
    rmarshaller.Deserialize(TestConfig::DataPath("test.RPB"));

  auto rationalmodel2 =
    rmarshaller.Deserialize(TestConfig::DataPath("test.RPB"));

  EpipolarModelFactory epfactory(EpipolarModelFactory::EpipolarOption::eFULL);
  const std::pair<int, int> modelpair = { 5000, 5000 };

  auto epimodel =
    epfactory.CreateEpipolarModel(std::move(rationalmodel), modelpair,
      std::move(rationalmodel2), modelpair, 3000);

  EpipolarModelMarshaller epiMarshaller;
  epiMarshaller.Serialize(TestConfig::DataPath("epimodel.json"),
    epimodel.get());
}

TEST(marshaller, framemodel) {
  FrameModelMarshaller fmarshaller;
  auto nameList = fmarshaller.Deserialize(TestConfig::DataPath("geoin.dat"),
    TestConfig::DataPath("itera.dat"));
  cout << nameList.size() << endl;
  for (auto const &name : nameList) {
    auto model = fmarshaller.GetModel(name);
  }

  //checks need get method on framemodel
}

TEST(marshaller, rationalmodel) {
  RationalModelMarshaller rmarshaller;
  auto rationalmodel =
      rmarshaller.Deserialize(TestConfig::DataPath("test.RPB"));

  double errBias, errRand;
  rationalmodel->GetErrors(errBias, errRand);
  EXPECT_EQ(errBias, 1.89);
  EXPECT_EQ(errRand, 0.26);

  double* normalFactor = rationalmodel->GetNormalFators();
  EXPECT_EQ(normalFactor[0], 12661);
  EXPECT_EQ(normalFactor[1], 17589);
  EXPECT_EQ(normalFactor[2], 45.0456);
  EXPECT_EQ(normalFactor[3], 82.1361);
  EXPECT_EQ(normalFactor[4], 719);
  EXPECT_EQ(normalFactor[5], 12662);
  EXPECT_EQ(normalFactor[6], 17590);
  EXPECT_EQ(normalFactor[7], 0.0744);
  EXPECT_EQ(normalFactor[8], 0.1239);
  EXPECT_EQ(normalFactor[9], 501);
}
