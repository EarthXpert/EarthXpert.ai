/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <Eigen/Dense>
#include <Eigen/IterativeLinearSolvers>

#include <complex>
#include <iostream>

using namespace earthxpert;
using namespace Eigen;

constexpr int kR = 60;
constexpr int kC = 18;
using EigMatS = Matrix<double, kR, kC, RowMajor>;
using EigVecS = Matrix<double, kR, 1>;

/**
 *  \brief fill A and b
 *
 *  \param [in] n the dimension of square matrix
 */
template<typename Ty> void
DenseFill_Rnd(EigMatXR<Ty>& A, EigVecX<Ty>& b) {
  const int n = A.rows();
  const int m = A.cols();

  for (int i = 0; i < n; ++i) {
    const int j = i % m;
    A(i, j) += Ty(i%3);
    b[i] += Ty(sqrt(i + 1));
  }

  //std::cout << "A:\n" << A << std::endl;
  //std::cout << "rhs b:\n" << b << std::endl;
}

template<typename EigMat, typename EigVec>
void DenseFill(EigMat& A, EigVec& b) {
  A << 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.55525,\
       1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.31049,   \
       1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.214481,  \
       1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.345925,  \
       1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.234187,  \
       0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.244769,  \
       0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.769766,  \
       0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.209333,  \
       0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.789474,  \
       0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.525038,  \
       0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0354484, \
       0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.544744,  \
       0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.653084,  \
       0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.560523,  \
       0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0197061, \
       0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.128067,  \
       0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.439477,  \
       0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.580233,  \
       0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.688591,  \
       0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,         \
       0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.957024,  \
       0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.108367,  \
       0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.419783,  \
       0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.376815,  \
       0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.31138,   \
       0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.268406,  \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0429698, \
       0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0.769687,  \
       0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0.610629,  \
       0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0.318587,  \
       0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0.812584,  \
       0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0.567598,  \
       0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0.275582,  \
       0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0.430143,  \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0.422487,  \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.292018,  \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0.137456,  \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0.0815321, \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0.957736,  \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0.225522,  \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0.311007,  \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0.15457,   \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0.210501,  \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0.665729,  \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0.0665052, \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0.603052,  \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.0559429, \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0.82037,   \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0.0880688, \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0.448527,  \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0.00766913,\
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.87638,   \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0.144025,  \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0.392617,  \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0.0482826, \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0.732363,  \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0.828105,  \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0.536633,  \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0.0957409, \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0.4409;
  
  //const double sclFac = 1.0 / A.rightCols<1>().lpNorm<Infinity>();
  //A.rightCols<1>() *= sclFac;

  b << -0.00751723, -0.0043577, -0.0061897, -0.00878231, -0.0173482, \
    0.0028802, 0.00449109, 0.000961373, -0.0134555, -0.00118544,     \
    0.00227552, 0.000192654, 0.00277129, 0.00354112, -0.00190976,    \
    -0.000848853, -0.00214038, -0.00335806, -0.00333281, -0.00647451,\
    0.00496788, 0.000336802, 0.00101069, 0.00372282, -0.00256181,    \
    0.0116938, 0.00193568, -0.0217963, -0.00643823, -0.0154559,      \
    -0.0070407, -0.0110185, -0.0136558, -0.00927987, -0.00890581,    \
    -0.00167668, 0.000557027, -0.0148066, -0.0074599, 0.000214905,   \
    -0.0126418, 0.00151756, -0.0070521, -0.00240879, -0.00616893,    \
    0.00189825, -0.00220487, -0.00305063, 0.000596517, - 0.00568016, \
    0.00150859, -0.00124478, 0.00195122, -0.0026374, 0.010139,       \
    0.00290875, 0.00297966, -0.00414365, -0.00234431, -0.000410536;
}

template<typename EigMat>
void RCondNums(const EigMat& A) {
  // design matrix
  const double rcnSvd = RCondSvd(A);
  std::cout << "Reciprocal condition number SVD: " << rcnSvd << std::endl;

  // normal matrix
  const auto& ldlt = (A.transpose() * A).ldlt();

  const double rcnNmSvd = RCondSvd(ldlt.matrixLDLT());
  std::cout << "Reciprocal condition number NE SVD: " << rcnNmSvd << std::endl;

  const double rcnNm = ldlt.rcond();
  std::cout << "Reciprocal condition number NE: " << rcnNm << " " << ldlt.info() << std::endl;
}

template<typename EigMat, typename EigVec>
void DenseSolve_Direct(const EigMat& A, const EigVec& b) {
  // singular value decomposition
  //const auto& xSvd = A.bdcSvd(ComputeFullU | ComputeFullV).solve(b);  // not stable?
  //std::cout << "SVD solution:\n" << xSvd << std::endl;

  //const double errSvd = RelativeErr(A, xSvd, b);
  //std::cout << "SVD relative error: " << errSvd << std::endl;

  // QR factorization
  //const auto& xQr = A.colPivHouseholderQr().solve(b);  // not stable?
  //std::cout << "QR solution:\n" << xQr << std::endl;

  //const double errQr = RelativeErr(A, xQr, b);
  //std::cout << "QR relative error: " << errQr << std::endl;

  // normal equation
  const auto& ldlt = (A.transpose() * A).ldlt();

  const auto& L = ldlt.matrixL();
  std::cout << "Lower diagonal: ";
  for (int i = 0; i < L.rows(); ++i)
    std::cout << L(i, i) << " ";
  std::cout << std::endl;

  std::cout << "D diagonal:\n" << ldlt.vectorD().diagonal() << std::endl;

  const auto& U = ldlt.matrixU();
  std::cout << "Upper diagonal: ";
  for (int i = 0; i < U.rows(); ++i)
    std::cout << U(i, i) << " ";
  std::cout << std::endl;

  const auto& xNe = ldlt.solve(A.transpose() * b);
  std::cout << "LS solution:\n" << xNe.eval() << std::endl;

  const double errNe = RelativeErr(A, xNe, b);
  std::cout << "LS relative error: " << errNe << std::endl;
}

template<typename EigMat, typename EigVec>
void DenseSolve_Iterative(const EigMat& A, const EigVec& b) {
  // LSCG
  LeastSquaresConjugateGradient<EigMat> lscg;
  lscg.compute(A);
  VectorXd xLscg = lscg.solve(b);
  //std::cout << "LS solution:\n" << xNe << std::endl;

  const double errLscg = RelativeErr(A, xLscg, b);
  std::cout << "LSCG relative error: " << errLscg << std::endl;
}

/**
 *  \brief solve Ax = b for x
 *
 *  \tparam Ty number type
 */
TEST(eigen, DenseSolve_Rnd_dynamic) {
  EigMatXR<double> A = EigMatXR<double>::Random(kR, kC);
  EigVecX<double> b = EigVecX<double>::Random(kR);
  DenseFill_Rnd<double>(A, b);

  RCondNums(A);
  //DenseSolve_Direct(A, b);
  //DenseSolve_Iterative(A, b);
}

TEST(eigen, DenseSolve_dynamic) {
  EigMatXR<double> A = EigMatXR<double>::Zero(kR, kC);
  EigVecX<double> b = EigVecX<double>::Zero(kR);
  DenseFill(A, b);

  RCondNums(A);
  //DenseSolve_Direct(A, b);
  //DenseSolve_Iterative(A, b);
}

TEST(eigen, DenseSolve_sized) {
  EigMatS A;
  EigVecS b;
  DenseFill(A, b);

  RCondNums(A);
  DenseSolve_Direct(A, b);
  //DenseSolve_Iterative(A, b);
}
