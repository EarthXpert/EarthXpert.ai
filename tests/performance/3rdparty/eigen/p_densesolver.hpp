/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/eigen.hpp>
#include <Eigen/Dense>

#include <iostream>

namespace bmp = boost::multiprecision;
using namespace Eigen;

static constexpr int kNumIters = 100;

template<typename Ty>
using MatTy = Matrix<Ty, Dynamic, Dynamic, RowMajor>;

template<typename Ty>
using VecTy = Matrix<Ty, Dynamic, 1>;

/**
 *  \brief fill A and b
 *
 *  \param [in] n the dimension of square matrix
 */
template<typename Ty> void 
DenseFill(MatTy<Ty>& A, VecTy<Ty>& b, const int n) {
  A = MatTy<Ty>::Random(n, n);
  b = VecTy<Ty>::Random(n);

  for (int i = 0; i < n; ++i) {
    A(i, i) += Ty(i + 1);
    b[i] += Ty(sqrt(i + 1));
  }

  //std::cout << "A:\n" << A << std::endl;
  //std::cout << "rhs b:\n" << b << std::endl;
}

/**
 *  \tparam Ty bmp::number with a backend
 */
template<typename Ty> void Dense_Fill(const int& n) {
  MatTy<Ty> A;
  VecTy<Ty> b;
  DenseFill<Ty>(A, b, n);
}

/**
 *  \brief solve Ax = b for x
 *
 *  \tparam Ty number type
 */
template<typename Ty> void Dense_Solve(const int n) {
  MatTy<Ty> A;
  VecTy<Ty> b;
  DenseFill<Ty>(A, b, n);

  // A.fullPivHouseholderQr().solve(b)
  VecTy<Ty> x = (A.transpose() * A).inverse() * (A.transpose() * b);

  //const Ty err = (A * x - b).norm() / b.norm(); // relative error
  //std::cout << "relative error: " << err << std::endl;
}

/**
 *  TestCase 1: double
 */
void DenseMatF16_Fill(const int& n) {
  Dense_Fill<double>(n);
}

void DenseMatF16_Solve(const int& n) {
  Dense_Solve<double>(n);
}

/**
 *  TestCase 2: bmp::cpp_dec_float_50
 */
void DenseMatF50_Fill(const int& n) {
  Dense_Fill<bmp::cpp_dec_float_50>(n);
}

void DenseMatF50_Solve(const int& n) {
  Dense_Solve<bmp::cpp_dec_float_50>(n);
}

/**
 *  TestCase 3: bmp::cpp_dec_float_100
 */
void DenseMatF100_Fill(const int& n) {
  Dense_Fill<bmp::cpp_dec_float_100>(n);
}

void DenseMatF100_Solve(const int& n) {
  Dense_Solve<bmp::cpp_dec_float_100>(n);
}

/**
 *  TestCase 4: mpf_float_50
 */
void DenseMatF50_Fill_GMP(const int& n) {
  Dense_Fill<bmp::mpf_float_50>(n);
}

void DenseMatF50_Solve_GMP(const int& n) {
  Dense_Solve<bmp::mpf_float_50>(n);
}

/**
 *  TestCase 5: mpf_float_100
 */
void DenseMatF100_Fill_GMP(const int& n) {
  Dense_Fill<bmp::mpf_float_100>(n);
}

void DenseMatF100_Solve_GMP(const int& n) {
  Dense_Solve<bmp::mpf_float_100>(n);
}

static const std::vector<int> args_ds{ 10, 100 };

SLTBENCH_FUNCTION_WITH_ARGS(DenseMatF16_Fill, args_ds);
SLTBENCH_FUNCTION_WITH_ARGS(DenseMatF16_Solve, args_ds);

SLTBENCH_FUNCTION_WITH_ARGS(DenseMatF50_Fill, args_ds);
SLTBENCH_FUNCTION_WITH_ARGS(DenseMatF50_Solve, args_ds);
SLTBENCH_FUNCTION_WITH_ARGS(DenseMatF100_Fill, args_ds);
SLTBENCH_FUNCTION_WITH_ARGS(DenseMatF100_Solve, args_ds);

SLTBENCH_FUNCTION_WITH_ARGS(DenseMatF50_Fill_GMP, args_ds);
SLTBENCH_FUNCTION_WITH_ARGS(DenseMatF50_Solve_GMP, args_ds);
SLTBENCH_FUNCTION_WITH_ARGS(DenseMatF100_Fill_GMP, args_ds);
SLTBENCH_FUNCTION_WITH_ARGS(DenseMatF100_Solve_GMP, args_ds);
