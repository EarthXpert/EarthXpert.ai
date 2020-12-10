/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/sltbench/sltbench_main.h"
#include "testconfig.h"

#include <memory_resource>
#include <string>
#include <vector>

static constexpr int kNumIters = 100;
static constexpr int kNumStrs = 1000;

/**
 *  TestCase 0:
 */
void STD_Vector() {
  std::vector<std::string> strs;

  for (int i = 0; i < kNumIters; ++i) {
    strs.clear();
    for (int j = 0; j < kNumStrs; ++j) {
      strs.push_back("test vector of strings by STD");
    }
  }
}

void PMR_Vector() {
  std::pmr::unsynchronized_pool_resource upr;
  std::pmr::vector<std::pmr::string> strs(&upr);

  for (int i = 0; i < kNumIters; ++i) {
    strs.clear();
    for (int j = 0; j < kNumStrs; ++j) {
      strs.push_back("test vector of strings by PMR");
    }
  }
}

SLTBENCH_FUNCTION(STD_Vector);
SLTBENCH_FUNCTION(PMR_Vector);
