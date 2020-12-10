/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/eigen.hpp>
#include <Eigen/IterativeLinearSolvers>

#include <random>

namespace bmp = boost::multiprecision;
using namespace::Eigen;

//  \tparam bmp::number<backend>
template<typename Ty>
using SpMatR = SparseMatrix<Ty, RowMajor>;

template<typename Ty>
using DenseVec = Matrix<Ty, Dynamic, 1>;

static constexpr auto kScale = 1.0 / static_cast<double>(std::mt19937::max());
static constexpr int64_t kNs = 10'000'000;

/**
 *  \brief fill A and b
 *
 *  \param [in] count the number of fill-ins on each row of sparse matrix
 */
template<typename Ty = double>
void LSCG_NonDiagonalFill(SpMatR<Ty>& A, DenseVec<Ty>& b, const int count) {
  std::mt19937 rng;
  A.resize(kNs, kNs);
  b.resize(kNs);

  std::vector<int> reserveSizes(kNs, count);
  A.reserve(reserveSizes);

  // fill A and b
  for (int i = 0; i < kNs; ++i) {
    A.insert(i, i) = Ty(i + rng() * kScale);  // i + [0, 1]
    b[i] = Ty(i + (rng() * kScale - 0.5) * 0.1);  // [-0.05, 0.05]

    // add count-1 elements [-0.5, 0.5] to each row
    for (int j = 1; j < count; ++j)
      A.insert(i, rng() % kNs) += Ty(rng() * kScale - 0.5);
  }
}

template<typename Ty = double>
void LSCG_NonDiagonal_Fill(const int count) {
  SpMatR<Ty> A;
  DenseVec<Ty> b;
  LSCG_NonDiagonalFill<Ty>(A, b, count);
}

template<typename Ty = double>
void LSCG_NonDiagonal_Solve(const int& count) {
  SpMatR<Ty> A;
  DenseVec<Ty> b;
  LSCG_NonDiagonalFill<Ty>(A, b, count);

  LeastSquaresConjugateGradient<SpMatR<Ty>> lscg;
  lscg.compute(A);
  DenseVec<Ty> x = lscg.solve(b);

  //std::cout << "LSCG error: " << lscg.error() << std::endl;

  //double err = (A * x - b).norm() / b.norm();
  //std::cout << "relative error: " << err << std::endl;
}

/**
 *  TestCase 1: double
 */
void LSCGF16_NonDiagonal_Fill(const int& count) {
  LSCG_NonDiagonal_Fill<double>(count);
}

void LSCGF16_NonDiagonal_Solve(const int& count) {
  LSCG_NonDiagonal_Solve<double>(count);
}

/**
 *  TestCase 2: bmp::cpp_dec_float_50
 */
void LSCGF50_NonDiagonal_Fill(const int& count) {
  LSCG_NonDiagonal_Fill<bmp::cpp_dec_float_50>(count);
}

void LSCGF50_NonDiagonal_Solve(const int& count) {
  LSCG_NonDiagonal_Solve<bmp::cpp_dec_float_50>(count);
}

/**
 *  TestCase 3: bmp::cpp_dec_float_100
 */
void LSCGF100_NonDiagonal_Fill(const int& count) {
  LSCG_NonDiagonal_Fill<bmp::cpp_dec_float_100>(count);
}

void LSCGF100_NonDiagonal_Solve(const int& count) {
  LSCG_NonDiagonal_Solve<bmp::cpp_dec_float_100>(count);
}

/**
 *  TestCase 4: mpf_float_50
 */
void LSCGF50_NonDiagonal_Fill_GMP(const int& count) {
  LSCG_NonDiagonal_Fill<bmp::mpf_float_50>(count);
}

void LSCGF50_NonDiagonal_Solve_GMP(const int& count) {
  LSCG_NonDiagonal_Solve<bmp::mpf_float_50>(count);
}

/**
 *  TestCase 5: mpf_float_100
 */
void LSCGF100_NonDiagonal_Fill_GMP(const int& count) {
  LSCG_NonDiagonal_Fill<bmp::mpf_float_100>(count);
}

void LSCGF100_NonDiagonal_Solve_GMP(const int& count) {
  LSCG_NonDiagonal_Solve<bmp::mpf_float_100>(count);
}

static const std::vector<int> args_cg{ 15 };   // 3

/**
 *  \remark test 100 digits on computer with 32GB+ memory
 */
SLTBENCH_FUNCTION_WITH_ARGS(LSCGF16_NonDiagonal_Fill, args_cg);
SLTBENCH_FUNCTION_WITH_ARGS(LSCGF16_NonDiagonal_Solve, args_cg);

SLTBENCH_FUNCTION_WITH_ARGS(LSCGF50_NonDiagonal_Fill, args_cg);
SLTBENCH_FUNCTION_WITH_ARGS(LSCGF50_NonDiagonal_Solve, args_cg);
//SLTBENCH_FUNCTION_WITH_ARGS(LSCGF100_NonDiagonal_Fill, args_cg);
//SLTBENCH_FUNCTION_WITH_ARGS(LSCGF100_NonDiagonal_Solve, args_cg);

SLTBENCH_FUNCTION_WITH_ARGS(LSCGF50_NonDiagonal_Fill_GMP, args_cg);
SLTBENCH_FUNCTION_WITH_ARGS(LSCGF50_NonDiagonal_Solve_GMP, args_cg);
//SLTBENCH_FUNCTION_WITH_ARGS(LSCGF100_NonDiagonal_Fill_GMP, args_cg);
//SLTBENCH_FUNCTION_WITH_ARGS(LSCGF100_NonDiagonal_Solve_GMP, args_cg);
