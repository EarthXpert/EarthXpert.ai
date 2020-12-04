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
} // namespace earthxpert
