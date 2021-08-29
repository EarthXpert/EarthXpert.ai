/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"

#include <cassert>
#include <tuple>
#include <Eigen/Eigen>

//template<typename Ty>
//using EigMatX = Eigen::Matrix<Ty, Eigen::Dynamic, Eigen::Dynamic>;

template<typename Ty>
using EigMatXR = Eigen::Matrix<Ty, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

template<typename Ty>
using EigVecX = Eigen::Matrix<Ty, Eigen::Dynamic, 1>;

template<typename Ty>
using EigVecXR = Eigen::Matrix<Ty, 1, Eigen::Dynamic>;

using EigTri = Eigen::Triplet<double>;

namespace earthxpert {
template<typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
using MatView = Eigen::Map<Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>>;

template<typename _Scalar, int _Rows, int _Options, int _MaxRows, int _MaxCols>
using VecView = MatView<_Scalar, _Rows, 1, _Options, _MaxRows, _MaxCols>;

/**
 *  \param [in] A a matrix from small to median size.
 *  \return Return reciprocal of condition number by SVD
 *
 *  \except bad_allocate if the matrix size is large.
 */
template<typename MatTy> inline double RCondSvd(const MatTy& A) {
  Eigen::JacobiSVD<Eigen::MatrixXd> svd(A);
  const auto& sv = svd.singularValues();
  const double rcn = sv(sv.size() - 1) / sv(0);
  return rcn;
}

/**
 *  \param [in] A a matrix from small to median size.
 *  \return Return condition number by SVD
 *
 *  \except bad_allocate if the matrix size is large.
 */
template<typename MatTy> inline double CondSvd(const MatTy& A) {
  const double cn = 1.0 / RCondSvd(A);
  return cn;
}

template<typename MatTy, typename VecTy1, typename VecTy2> 
inline auto RelativeErr(const MatTy& A, const VecTy1& x, 
  const VecTy2& b) -> typename MatTy::Scalar {
  return (A * x - b).norm() / b.norm();
}

/**
 *  \param [in] N a symetric square matrix
 * 
 *  \return (max_correlation_coef, row_loc, col_loc)
 */
template<typename MatTy>
inline std::tuple<double, int, int> LocateMaxCorrelation(const MatTy& N) {
  const int dim = N.rows();
  assert(dim == N.cols());
  EigMatXR<double> cf = N.inverse().eval();   // cofactor
  const EigVecX<double> cfd = cf.diagonal().array().sqrt();

  int rl{}, cl{};
  double mcc{};
  for (int i = 0; i < dim; ++i) {
    for (int j = i + 1; j < dim; ++j) { // strict upper triangle
      cf(i, j) /= cfd[i] * cfd[j];
      if (mcc >= cf(i, j)) continue;
      mcc = cf(i, j);
      rl = i;
      cl = j;
    }
  }
  //std::cout << cf << std::endl;
  return std::make_tuple(mcc, rl, cl);
}

/**
 *  \brief insert multiple fill-ins on a row
 *
 *  \tparam InputIterators iterator to a container of std::pair as elements
 *  \todo hack to get row storage once
 */
template<typename _Scalar, typename _StorageIndex, typename InputIterators>
void SpMat_Insert(Eigen::SparseMatrix<_Scalar, Eigen::RowMajor, _StorageIndex>& spMat,
  Eigen::Index row, const InputIterators& begCol, const InputIterators& endCol) {
  for (auto col = begCol; col != endCol; ++col) {
    spMat.insert(row, col->first) = col->second;
  }
}

inline std::vector<double> VecEigen2Std(const Eigen::VectorXd& v) {
  return std::vector<double>(v.data(), v.data() + v.cols() * v.rows());
}
} // namespace earthxpert
