/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/vld/exvld.h"

#include <cstdlib>
#include <iostream>
#include <string>

 /**
  *  TestCase 0: new memory leak
  */
TEST(vld, ML_new) {
  auto dbl = new double;        //delete dbl;
  auto str = new std::string;   delete str;

  auto intArr = new int[10];    delete[] intArr;
  auto fltArr = new float[10];  delete[] fltArr;
}

/**
 *  TestCase 0: malloc memory leak
 */
TEST(vld, ML_malloc) {
  auto dbl = static_cast<double*>(malloc(sizeof(double)));    free(dbl);

  auto intArr = static_cast<int*>(malloc(sizeof(int) * 10));  //free(intArr);
}
