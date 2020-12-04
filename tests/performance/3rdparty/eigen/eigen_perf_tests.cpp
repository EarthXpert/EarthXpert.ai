/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/sltbench/sltbench_main.h"
#include "testconfig.h"

#include "3rdparty/eigen/exeigen.h"

#include <Eigen/IterativeLinearSolvers>
#include <iostream>

using namespace earthxpert;
using namespace::Eigen;
using SpMatDR = SparseMatrix<double, RowMajor>;

static constexpr int64_t kN = 1000;

inline void Insert(SpMatDR& A, const int64_t count) {
  for (int64_t i = 0; i < count; ++i) {
    const int64_t r = i / kN;
    const int64_t c = i % kN;
    A.insert(r, c) = 0.1;
  }
}

inline void SetTriplets(SpMatDR& A, const int64_t count) {
  std::vector<EGTri> egTri;
  egTri.reserve(count);

  for (int64_t i = 0; i < count; ++i) {
    const int r = i / kN;
    const int c = i % kN;
    egTri.emplace_back(r, c, 0.1);
  }

  A.setFromTriplets(egTri.begin(), egTri.end());
}

/**
 *  TestCase 1: Insert()
 */
void SpMat_Insert(const int64_t& count) {
  SpMatDR A(kN, kN);
  Insert(A, count);
}

/**
 *  TestCase 2: ReserveInsert()
 */
void SpMat_ReserveInsert(const int64_t& count) {
  SpMatDR A(kN, kN);
  A.reserve(count);

  Insert(A, count);
}

/**
 *  TestCase 3: SetTriplets()
 */
void SpMat_SetTriplets(const int64_t& count) {
  SpMatDR A(kN, kN);
  SetTriplets(A, count);
}

/**
 *  TestCase 4: ReserveSetTriplets()
 */
void SpMat_ReserveSetTriplets(const int64_t& count) {
  SpMatDR A(kN, kN);
  A.reserve(count);

  SetTriplets(A, count);
}

static const std::vector<int64_t> args{ kN, kN*4, kN*4+3,
  kN*10, kN*10+3, kN*22, kN*22+3 }; // kN*46+3, kN*94+3

SLTBENCH_FUNCTION_WITH_ARGS(SpMat_Insert, args);
SLTBENCH_FUNCTION_WITH_ARGS(SpMat_ReserveInsert, args);
//SLTBENCH_FUNCTION_WITH_ARGS(SpMat_SetTriplets, args);
//SLTBENCH_FUNCTION_WITH_ARGS(SpMat_ReserveSetTriplets, args);
