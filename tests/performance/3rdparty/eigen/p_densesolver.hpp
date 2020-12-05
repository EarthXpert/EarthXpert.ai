/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <boost/multiprecision/cpp_dec_float.hpp>
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

// fill A and b
template<typename Ty> void 
DenseFill(MatTy<Ty>& A, VecTy<Ty>& b, const int count) {
  A = MatTy<Ty>::Random(count, count);
  b = VecTy<Ty>::Random(count);

  for (int i = 0; i < count; ++i) {
    A(i, i) += Ty(i + 1);
    b[i] += Ty(sqrt(i + 1));
  }

  //std::cout << "A:\n" << A << std::endl;
  //std::cout << "rhs b:\n" << b << std::endl;
}

/**
 *  \brief solve Ax = b for x
 *
 *  \tparam Ty number type
 */
template<typename Ty> void DenseSolve(const int count) {
  MatTy<Ty> A;
  VecTy<Ty> b;
  DenseFill<Ty>(A, b, count);

  // A.fullPivHouseholderQr().solve(b)
  VecTy<Ty> x = (A.transpose() * A).inverse() * (A.transpose() * b);

  //const Ty err = (A * x - b).norm() / b.norm(); // relative error
  //std::cout << "relative error: " << err << std::endl;
}

/**
 *  TestCase 1: double
 */
void DenseMatF16_Fill(const int& count) {
  MatTy<double> A;
  VecTy<double> b;
  DenseFill<double>(A, b, count);
}

void DenseMatF16_Solve(const int& count) {
  DenseSolve<double>(count);
}

/**
 *  TestCase 2: bmp::cpp_dec_float_50
 */
void DenseMatF50_Fill(const int& count) {
  MatTy<bmp::cpp_dec_float_50> A;
  VecTy<bmp::cpp_dec_float_50> b;
  DenseFill<bmp::cpp_dec_float_50>(A, b, count);
}

void DenseMatF50_Solve(const int& count) {
  DenseSolve<bmp::cpp_dec_float_50>(count);
}

/**
 *  TestCase 3: bmp::cpp_dec_float_100
 */
void DenseMatF100_Fill(const int& count) {
  MatTy<bmp::cpp_dec_float_100> A;
  VecTy<bmp::cpp_dec_float_100> b;
  DenseFill<bmp::cpp_dec_float_100>(A, b, count);
}

void DenseMatF100_Solve(const int& count) {
  DenseSolve<bmp::cpp_dec_float_100>(count);
}

static const std::vector<int> args_ds{ 10, 100 };

SLTBENCH_FUNCTION_WITH_ARGS(DenseMatF16_Fill, args_ds);
SLTBENCH_FUNCTION_WITH_ARGS(DenseMatF16_Solve, args_ds);
SLTBENCH_FUNCTION_WITH_ARGS(DenseMatF50_Fill, args_ds);
SLTBENCH_FUNCTION_WITH_ARGS(DenseMatF50_Solve, args_ds);
SLTBENCH_FUNCTION_WITH_ARGS(DenseMatF100_Fill, args_ds);
SLTBENCH_FUNCTION_WITH_ARGS(DenseMatF100_Solve, args_ds);
