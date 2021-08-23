/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/rationalmodel.h"
#include <array>
#include <bitset>

namespace earthxpert {
/**
 *  The class RationalFit solves the rational function model coefficients
 *  (RPCs) from points composed of a 2D image grid and a 3D ground cube.
 */
class EXSENSORMODEL_EXPORTS RationalFit {
public:
  // group of selected coefficients
  static constexpr int kStandard = 0; // 78-parameter 3rd-order rational function model
  static constexpr int kEqDenom = 1;  // line and pixel denominators are equal
  static constexpr int kUnitDenom = 2;  // line and pixel denominators are fixed to be 1
  static constexpr int kNoQuad = 4;   // no quadratic terms
  static constexpr int kNoCubic = 8;  // no cubic terms
  static constexpr int kNoHgt = 16;   // no height terms
  static constexpr int kDLT = 32;     // 11-parameter 3D direct linear transform: kEqDenom | kNoQuad | kNoCubic
  static constexpr int kPrj2D = 64;   // 8-parameter 2D projective transformation: kEqDenom | kNoQuad | kNoCubic | kNoHgt

  explicit RationalFit(int coefGroup = kFull);
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
  
  void CalcCoefficients(const std::vector<Point2f>& images, 
    const std::vector<Point3d>& grounds,
    std::array<double, rpc::NumCoef>& lineNumCoef,
    std::array<double, rpc::NumCoef>& lineDenCoef,
    std::array<double, rpc::NumCoef>& sampNumCoef,
    std::array<double, rpc::NumCoef>& sampDenCoef);

private:
  static constexpr int NumOffScl = NumFact / 2;

  // estimation parameters
  int mCoefGroup{kFull}; // coefficients group
  std::bitset<rpc::NumCoef> mCoefFlags; // flag indicating which polynomial coefficients to estimate

  // normalization factors
  std::array<double, NumOffScl> mOffFactors{0, 0, 0, 0, 0}; // offsets
  std::array<double, NumOffScl> mSclFactors{1.0, 1.0, 1.0, 1.0, 1.0}; // scales
  std::bitset<NumOffScl> mOffFlags; // flag indicating known offsets
  std::bitset<NumOffScl> mSclFlags; // flag indicating known scales
  std::array<short, NumOffScl> mOffDigits{ 0, 0, 4, 4, 0}; // number of digits past decimal place
  std::array<short, NumOffScl> mSclDigits{ 0, 0, 4, 4, 0 };

  // a priori weights of coefficients group are set to values that work well in practice
  // (no theoretical justification). In general, the contribution of higher-order terms
  // (given larger weights) is expected to be less than of lower-order ones. 
  double mWtOff{ 0.0 };   // constant offset coefficients
  double mWtLin{ 0.0 };   // linear coefficients
  double mWtQuad{ 0.001 };  // quadratic coefficients
  double mWtCub{ 0.01 };   // cubic coefficients

  int mMaxIter{ 10 }; // max. number of iterations allowed for CalcCoefficients
  double mTol{ 1.0e-10 };  // tolerance for convergence of coefficients

  // cached 
  bool mEqDenom{ false };  // flag indicating common line and pixel denominator terms
  bool mUnitDenom{ false };  // flag indicating eliminated line and pixel denominator terms
  int mNumCoefs{ 20 }; // number of polynomial coefficeints to estimate
  int mNumUnks{ 20 };  // number of RPCs to estimate
  void Initialize();

  void CalcNormalFactors(const std::vector<Point2d>& images, 
    const std::vector<Point3d>& grounds);
  void TruncateNormalFactors(double* const padfOff,
    double* const padfScale) const;
  void GetCoefMask(double* const padfMask) const;
  void GetPriorWeightArray(double* const padfWeights) const;
  void CalcLPHTerms(const double& dfL, const double& dfP, const double& dfH,
    const double* const padfCoefMask, double* const padfLPHTerms) const;
  void CreateRpcMathModel(const double* const padfOff, const double* const padfScale,
    const double* const padfLineNumCoef, const double* const padfLineDenCoef,
    const double* const padfSampNumCoef, const double* const padfSampDenCoef,
    pci::ogc::cs::ICRS* poCoordSys,
    pci::mathmodel::RpcMathModel*& poModel) const;
  void CalcRms(const std::vector<double>& oLat,
    const std::vector<double>& oLon,
    const std::vector<double>& oHgt, const std::vector<double>& oRow,
    const std::vector<double>& oCol,
    const pci::mathmodel::RpcMathModel* poModel, const int& nNumEstParms,
    double& dfRmsR, double& dfRmsC) const;

  long LsqEstimate(const std::vector<double>& oLat,
    const std::vector<double>& oLon,
    const std::vector<double>& oHgt, const std::vector<double>& oRow,
    const std::vector<double>& oCol,
    const double* const padfOff, const double* const padfScale,
    pci::ogc::cs::ICRS* poCoordSys,
    pci::mathmodel::RpcMathModel*& poModel) const;
  long LsqEstimateCommonDenom(const std::vector<double>& oLat,
    const std::vector<double>& oLon,
    const std::vector<double>& oHgt, const std::vector<double>& oRow,
    const std::vector<double>& oCol,
    const double* const padfOff, const double* const padfScale,
    pci::ogc::cs::ICRS* poCoordSys,
    pci::mathmodel::RpcMathModel*& poModel) const;
};
} // namespace earthxpert
