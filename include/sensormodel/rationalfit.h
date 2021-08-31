/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/rationalmodel.h"
#include "3rdparty/ceres/exceres.h"
#include "3rdparty/eigen/exeigen.h"

#include <array>
#include <bitset>
#include <tuple>
#include <type_traits>

namespace earthxpert {
using thirdparty::IResidual;
using PolyCoefArray = RationalModel::PolyCoefArray;

/**
 *  \brief AnalyticDiff cost functors implements CostFunction
 */
class AnalyticRationalCostFunc final : ceres::DynamicCostFunction {

};

/**
 *  \brief NumericDiff cost functors implements IResidual
 */
struct NumericRationalResidual final : IResidual {
  NumericRationalResidual() = default;
  ~NumericRationalResidual() = default;

  bool operator()(const double* const pb, double* residual) const {
    return true;
  }

private:
  const Point2d* image{};
  const Point3d* ground{};
};

/**
 *  The class RationalFit solves the rational function model coefficients
 *  (RPCs) from points composed of a 2D image grid and a 3D ground cube.
 */
class EXSENSORMODEL_EXPORTS RationalFit final {
public:
  // group of selected coefficients
  static constexpr int kStandard = 0; // 78-parameter 3rd-order rational function model
  static constexpr int kEqDenom = 1;  // line and sample denominators are equal
  static constexpr int kUnitDenom = 2;  // line and pixel denominators are fixed to be 1
  static constexpr int kNoQuad = 4;   // no quadratic terms
  static constexpr int kNoCubic = 8;  // no cubic terms
  static constexpr int kNoHgt = 16;   // no height terms
  static constexpr int kDLT = 32;     // 11-parameter 3D direct linear transform: kEqDenom | kNoQuad | kNoCubic
  static constexpr int kPrj2D = 64;   // 8-parameter 2D projective transformation: kEqDenom | kNoQuad | kNoCubic | kNoHgt

  explicit RationalFit(int coefGroup = kStandard);
  RationalFit(const RationalFit&) = delete;
  RationalFit(RationalFit&&) = delete;
  ~RationalFit() = default;

  RationalFit& operator=(const RationalFit&) = delete;
  RationalFit& operator=(RationalFit&&) = delete;

  void SetCoefGroup(int coefGroup);

  void SetNormalFactors(const NormalFactorArray& normalFators);
  void GetNormalFactors(NormalFactorArray& normalFators) const;

  void SetCoefWeights(double wtOff, double wtLin, 
    double wtQuad, double wtCub);
  void GetCoefWeights(double& wtOff, double& wtLin,
    double& wtQuad, double& wtCub) const;

  UPRationalModel CreateRationalModel(const std::vector<Point2f>& images,
    const std::vector<Point3d>& grounds);

private:
  enum ReguMethod {
    eLevenberg = 0, // add ratio of diagonal to normal diagonal
    eMarquad = 1    // add value to normal diagonal
  };

  enum CoefWeight {
    eWtOff = 0,   // constant offset coefficients
    eWtLin = 1,   // linear coefficients
    eWtQuad = 2,  // quadratic coefficients
    eWtCub = 3,   // cubic coefficients
    NumWts = 4
  };

  static constexpr int kNumOffScl = NumFact / 2;
  static constexpr int kNumUnks_STD = 39;

  // estimation parameters
  int mCoefGroup{kStandard}; // coefficients group
  std::bitset<rpc::NumCoef> mCoefFlags; // flag indicating which polynomial coefficients to estimate

  // normalization factors: line, samp, lat, lon, hgt
  std::array<double, kNumOffScl> mOffFactors{0}; // offsets
  std::array<double, kNumOffScl> mSclFactors{1.0}; // scales
  std::bitset<kNumOffScl> mOffFlags; // flag indicating known offsets
  std::bitset<kNumOffScl> mSclFlags; // flag indicating known scales
  std::array<int, kNumOffScl> mOffDigits{0, 0, 4, 4, 0}; // number of digits past decimal place
  std::array<int, kNumOffScl> mSclDigits{0, 0, 4, 4, 0};

  // a priori weights and value ranges of coefficient groups 
  // (constant offset, linear, quadratic, cubic)
  // are set to values that work well in practice (no theoretical justification).
  // In general, the contribution of higher-order terms
  // (given larger weights) is expected to be less than of lower-order ones. 
  std::array<double, 4> mCoefWts{0.0, 0.0, 0.001, 0.01};  // weights 
  std::array<double, 4> mCoefTrs{0.1, 2.0, 0.1, 0.1};  // +/- trust regions

  ReguMethod mReguMethod{ eMarquad };
  double mR{1.0e-6};  // Tikhonov regularization: ratio as Levenberg, value as Marquad
  int mMaxCC{ 10 }; // max. number of coefficients allowed to deactivate for cross-correlation
  double mCCTol{ 0.9 };  // tolerance for correlation coefficient
  int mMaxIter{ 10 }; // max. number of iterations allowed for solution
  double mTol{ 1.0e-10 };  // tolerance for convergence of coefficients

  // cached 
  int mRfmType{kStandard}; // flag indicating type of denominator terms
  int mNumCoefs{ 20 }; // number of polynomial coefficeints to estimate
  int mNumUnks{ 39 };  // number of line or sample coefficeints to estimate
  std::array<double, kNumOffScl> mInvSclFactors{1.0}; // inverse scales
  
  void Initialize();

  /**
   *  \brief calculate the normalization factors used for scaling values from
   *  (row, col, lat, lon, hgt) to apply to the polynomial coefficients. 
   *  If not normalized, the estimation could have numerical difficulties 
   *  for 3rd-order polynomials when the coordinate values are large.
   *  
   *  \tparam Ty point type
   *
   *  \param [in] points an array of grid/cube points.
   *  \param [in] arrGetters an array of the corresponding ground cube points
   */
  template<typename Ty> void CalcNormalFactors(const std::vector<Ty>& points,
    const std::array<std::function<double(const Ty&)>, kPointDim<Ty>>& arrGetters, const size_t idxStart) {
    std::vector<double> arr(points.size());

    size_t idx{idxStart};
    for (auto& arrGetter : arrGetters) {
      std::transform(points.cbegin(), points.cend(), arr.begin(), arrGetter);

      auto [off, scl] = CalcOffsetScale(arr, mOffDigits[idx], mSclDigits[idx]);
      if (!mOffFlags.test(idx)) {
        mOffFactors[idx] = off;
        mOffFlags.set(idx);
      }
      if (!mSclFlags.test(idx)) {
        mSclFactors[idx] = scl;
        mSclFlags.set(idx);
      }
      ++idx;
    }
  }

  void CalcNormalFactors(const std::vector<Point2d>& images, 
    const std::vector<Point3d>& grounds);

  // SolveCoefficients
  void SolveStandardEigen(const std::vector<Point2d>& images,
    const std::vector<Point3d>& grounds,
    PolyCoefArray& lineNumCoef, PolyCoefArray& lineDenCoef,
    PolyCoefArray& sampNumCoef, PolyCoefArray& sampDenCoef);
  void SolveEqualDenomEigen(const std::vector<Point2d>& images,
    const std::vector<Point3d>& grounds,
    PolyCoefArray& lineNumCoef, PolyCoefArray& lineDenCoef,
    PolyCoefArray& sampNumCoef, PolyCoefArray& sampDenCoef);
  void SolveUnitDenomEigen(const std::vector<Point2d>& images,
    const std::vector<Point3d>& grounds,
    PolyCoefArray& lineNumCoef, PolyCoefArray& sampNumCoef);

  void SolveStandardCeres(const std::vector<Point2d>& images,
    const std::vector<Point3d>& grounds,
    PolyCoefArray& lineNumCoef, PolyCoefArray& lineDenCoef,
    PolyCoefArray& sampNumCoef, PolyCoefArray& sampDenCoef);

  void GetCoefWeights(double* const weights) const;
  void GetCoefTrustRegions(double* const regions) const;
  void CalcJacobian(const Point3d& grd, double obs,
    const std::bitset<kNumUnks_STD>& coefFlags, EigVecXR<double>& A) const;
  void EnforceTrustRegions(const EigVecX<double>& T, EigVecX<double>& x) const;
};
} // namespace earthxpert
