/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <Eigen/Dense>

#include <complex>
#include <iostream>

using namespace Eigen;

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
 *  \brief solve Ax = b for x
 *
 *  \tparam Ty number type
 */
TEST(eigen, DenseSolve_dyn) {
  MatTy<double> A;
  VecTy<double> b;
  DenseFill<double>(A, b, 10);

  // singular value decomposition
  VecTy<double> x_svd = A.bdcSvd(ComputeThinU | ComputeThinV).solve(b);  // not stable?
  //std::cout << "SVD solution:\n" << x_svd << std::endl;

  const double err_svd = (A * x_svd - b).norm() / b.norm(); // relative error
  std::cout << "SVD relative error: " << err_svd << std::endl;

  // QR factorization

  // leaset squares
  VecTy<double> x_ls = (A.transpose() * A).ldlt().solve(A.transpose() * b);
  //std::cout << "LS solution:\n" << x_ls << std::endl;

  const double err_ls = (A * x_ls - b).norm() / b.norm(); // relative error
  std::cout << "LS relative error: " << err_ls << std::endl;
}