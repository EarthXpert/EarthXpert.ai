/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"

#include <Eigen/Eigen>

using EGTri = Eigen::Triplet<double>;

namespace earthxpert {
/**
 *  \param [in] A a matrix from small to median size.
 *  \return Return condition number by SVD
 *
 *  \except bad_allocate if the matrix size is large.
 */
template<typename MatTy> double ConditionNumber(const MatTy& A) {
  Eigen::JacobiSVD<Eigen::MatrixXd> svd(A);
  const auto& sv = svd.singularValues();
  const double cn = sv(0) / sv(sv.size() - 1);
  return cn;
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
