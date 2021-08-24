/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/rationalmodel.h"

#include <array>
#include <bitset>
#include <tuple>
#include <type_traits>

namespace earthxpert {
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

  void SetNormalFactors(const std::array<double, NumFact>& normalFators);
  void GetNormalFactors(std::array<double, NumFact>& normalFators) const;

  void SetCoefWeights(double wtOff, double wtLin, 
    double wtQuad, double wtCub);
  void GetCoefWeights(double& wtOff, double& wtLin,
    double& wtQuad, double& wtCub) const;

  UPRationalModel CreateRationalModel(const std::vector<Point2f>& images,
    const std::vector<Point3d>& grounds);

private:
  enum CoefWeight {
    eWtOff = 0,   // constant offset coefficients
    eWtLin = 1,   // linear coefficients
    eWtQuad = 2,  // quadratic coefficients
    eWtCub = 3,   // cubic coefficients
    NumWts = 4
  };

  static constexpr int NumOffScl = NumFact / 2;

  // estimation parameters
  int mCoefGroup{kStandard}; // coefficients group
  std::bitset<rpc::NumCoef> mCoefFlags; // flag indicating which polynomial coefficients to estimate

  // normalization factors: line, samp, lat, lon, hgt
  std::array<double, NumOffScl> mOffFactors{0, 0, 0, 0, 0}; // offsets
  std::array<double, NumOffScl> mSclFactors{1.0, 1.0, 1.0, 1.0, 1.0}; // scales
  std::bitset<NumOffScl> mOffFlags; // flag indicating known offsets
  std::bitset<NumOffScl> mSclFlags; // flag indicating known scales
  std::array<int, NumOffScl> mOffDigits{ 0, 0, 4, 4, 0}; // number of digits past decimal place
  std::array<int, NumOffScl> mSclDigits{ 0, 0, 4, 4, 0 };

  // a priori weights of coefficients group are set to values that work well in practice
  // (no theoretical justification). In general, the contribution of higher-order terms
  // (given larger weights) is expected to be less than of lower-order ones. 
  std::array<double, 4> mCoefWts{0.0, 0.0, 0.001, 0.01};  // constant offset, linear, quadratic, cubic

  int mMaxIter{ 10 }; // max. number of iterations allowed for CalcCoefficients
  double mTol{ 1.0e-10 };  // tolerance for convergence of coefficients

  // cached 
  bool mEqDenom{ false };  // flag indicating common line and sample denominator terms
  bool mUnitDenom{ false };  // flag indicating eliminated line and pixel denominator terms
  int mNumCoefs{ 20 }; // number of polynomial coefficeints to estimate
  int mNumUnks{ 39 };  // number of line or sample coefficeints to estimate
  void Initialize();

  /**
   *  \brief calculate the normalization factors used for scaling values from
   *  (row, col, lat, lon, hgt) to apply to the polynomial coefficients. 
   *  If not normalized, the estimation could have numerical difficulties 
   *  for 3rd-order polynomials when the coordinate values are large.
   *  
   *  \tparam Ty1 point type
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

  void CalcCoefficients(const std::vector<Point2d>& images, 
    const std::vector<Point3d>& grounds,
    std::array<double, rpc::NumCoef>& lineNumCoef,
    std::array<double, rpc::NumCoef>& lineDenCoef,
    std::array<double, rpc::NumCoef>& sampNumCoef,
    std::array<double, rpc::NumCoef>& sampDenCoef);
};
} // namespace earthxpert
