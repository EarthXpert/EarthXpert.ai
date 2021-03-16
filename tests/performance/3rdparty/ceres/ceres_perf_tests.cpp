/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/sltbench/sltbench_main.h"
#include "testconfig.h"

#include "3rdparty/ceres/exceres.h"
#include "p_tinysolver.hpp"

#include <iostream>
#include <tuple>

using namespace earthxpert;
using namespace ceres;

/**
 *  TestCase 1: dynamic memory
 */
void AnalyticDiff_(const int64_t& count) {
}

void AutoDiff_(const int64_t& count) {
}

void NumericDiff_(const int64_t& count) {
}

/**
 *  TestCase 2: pre-allocated memory
 */
void AnalyticDiff_Alloc_(const int64_t& count) {
}

void AutoDiff_Alloc_(const int64_t& count) {
}

void NumericDiff_Alloc_(const int64_t& count) {
}

const std::vector<int64_t> args{ 1 };

SLTBENCH_FUNCTION_WITH_ARGS(AnalyticDiff_, args);
SLTBENCH_FUNCTION_WITH_ARGS(AutoDiff_, args);
SLTBENCH_FUNCTION_WITH_ARGS(NumericDiff_, args);
