/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"

#include <Eigen/Eigen>

using EGTri = Eigen::Triplet<double>;

namespace earthxpert {
template<typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
using MatView = Eigen::Map<Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>>;

template<typename _Scalar, int _Rows, int _Options, int _MaxRows, int _MaxCols>
using VecView = MatView<_Scalar, _Rows, 1, _Options, _MaxRows, _MaxCols>;

/**
 *  \param [in] A a matrix from small to median size.
 *  \return Return condition number by SVD
 *
 *  \except bad_allocate if the matrix size is large.
 */
template<typename MatTy> double CondNumSvd(const MatTy& A) {
  Eigen::JacobiSVD<Eigen::MatrixXd> svd(A);
  const auto& sv = svd.singularValues();
  const double cn = sv(0) / sv(sv.size() - 1);
  return cn;
}

template<typename MatTy, typename VecTy1, typename VecTy2> 
auto RelativeErr(const MatTy& A, const VecTy1& x, const VecTy2& b) {
  return (A * x - b).norm() / b.norm();
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
} // namespace earthxpert
