#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"

#include "3rdparty/ceres/exceres.h"
#include "bundle_adjuster.hpp"

#include <iostream>

TEST(ceres, DUMMY) {
  std::cout << "[   EXAI   ]" << std::endl;

  int argc{ 3 };
  char** argv = new char* [3];
  argv[0] = "bundle_adjuster";
  argv[1] = "--input=D:/home/workspace/TestData/bal/problem-1723-156502-pre.txt";
  argv[2] = "--num_iterations=80";

  GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);
  //google::InitGoogleLogging(argv[0]);
  
  ASSERT_FALSE(FLAGS_input.empty());
  SolveProblem(FLAGS_input.c_str());

  delete[] argv;
}
