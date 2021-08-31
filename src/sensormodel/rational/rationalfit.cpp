/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/rationalfit.h"
#include "core/exceptions/exexcept.h"
#include "core/math/mutils.h"
#include "sensormodel/rmutils.h"

#include <cmath>

using namespace earthxpert;
using namespace ::Eigen;

namespace {
std::array<std::function<double(const Point2d&)>, 2> imgGetters {
  [](const Point2d& img) { return img.y; },
  [](const Point2d& img) { return img.x; } };

std::array<std::function<double(const Point3d&)>, 3> grdGetters {
  [](const Point3d& grd) { return grd.y; },
  [](const Point3d& grd) { return grd.x; },
  [](const Point3d& grd) { return grd.z; } };
}

RationalFit::RationalFit(const int coefGroup) : mCoefGroup(coefGroup) {
  Initialize();
}

void RationalFit::Initialize() {
  SetCoefGroup(mCoefGroup);
  std::cout << std::setprecision(10);
}

/**
 *  \brief Set flags indicating coefficients group to be estimated.
 *
 *  \param [in] coefGroup flag a bitwise-OR combination of selected groups
 */
void RationalFit::SetCoefGroup(const int coefGroup) {
  mCoefFlags.set(); // default using all polynomial coefficients

  int flag{ coefGroup };
  if (coefGroup & kDLT) {
    flag = kEqDenom | kNoQuad | kNoCubic;
  }
  if (coefGroup & kPrj2D) {
    flag = kEqDenom | kNoQuad | kNoCubic | kNoHgt;
  }

  if (flag & kEqDenom)
    mRfmType = kEqDenom;

  std::vector<rpc> noIdxs;

  if (flag & kNoQuad) {  // No quadratic terms
    for (std::underlying_type_t<rpc> idx = rpc::eLP; idx <= rpc::eHH; ++idx) {
      noIdxs.push_back(static_cast<rpc>(idx));
    }
  }
  if (flag & kNoCubic) {  // No cubic terms
    for (std::underlying_type_t<rpc> idx = rpc::ePLH; idx <= rpc::eHHH; ++idx) {
      noIdxs.push_back(static_cast<rpc>(idx));
    }
  }
  if (flag & kNoHgt) {  // No height terms: single layer for ortho
    noIdxs.insert(noIdxs.cbegin(), { rpc::eH, rpc::eLH, rpc::ePH,
      rpc::eHH, rpc::ePLH, rpc::eLHH, rpc::ePHH, rpc::eHHH });
  }

  std::sort(noIdxs.begin(), noIdxs.end());
  noIdxs.erase(std::unique(noIdxs.begin(), noIdxs.end()), noIdxs.end());
  
  for (const auto idx : noIdxs) {
    mCoefFlags.set(idx, false);
  }

  mNumCoefs = int(mCoefFlags.count());  // number of non-masked coefficients
  switch (mRfmType) {
  case kStandard:
    mNumUnks = mNumCoefs * 2 - 1;
    break;
  case kEqDenom:
    mNumUnks = mNumCoefs + (mNumCoefs - 1) / 2;
    break;
  case kUnitDenom:
    break;
  }
}

/**
 *  \brief specify normalization factors (offsets + scales),                            
 *  which are desired and useful in certain situations (stereo).                                                  
 */
void RationalFit::SetNormalFactors(const NormalFactorArray& normalFators) {
  std::copy_n(normalFators.cbegin(), kNumOffScl, mOffFactors.begin());
  mOffFlags.set();
  std::copy_n(normalFators.cbegin() + kNumOffScl, kNumOffScl, mSclFactors.begin());
  mSclFlags.set();
}

void RationalFit::GetNormalFactors(NormalFactorArray& normalFators) const {
  std::copy_n(mOffFactors.cbegin(), kNumOffScl, normalFators.begin());
  std::copy_n(mSclFactors.cbegin(), kNumOffScl, normalFators.begin()+kNumOffScl);
}

/**
 *  \brief Set the prior weights used to constrain polynomial coefficients.
 *
 *  For most sensors, the amount of contribution of the terms to
 *  the imaging geometry should be going down from offset 
 *  to linear to quadratic and through to cubic terms.
 * 
 *  By experience, it is often desirable to set a small prior weight
 *  on quadratic and cubic terms when performing coefficient estimation. This
 *  keeps them from getting large values, resulting in a situation where the
 *  high-order polynomial terms and the low-order polynomial terms cancel each
 *  other to get a stable solution. 
 *
 * \param [in] wtOff weight of offset terms
 * \param [in] wtLin weight of linear terms
 * \param [in] wtQuad weight of quadratic terms
 * \param [in] wtCub weight of cubic terms
 *
 * \except thrown if any paraemter is negative
 */
void RationalFit::SetCoefWeights(const double wtOff,
  const double wtLin, const double wtQuad, const double wtCub) {
  auto coefWt = [](const double wt, const std::string& var) {
    if (0.0 > wt) 
      throw std::invalid_argument(var);
    return wt;
  };

  mCoefWts[eWtOff] = coefWt(wtOff, "wtOff");
  mCoefWts[eWtLin] = coefWt(wtLin, "wtLin");
  mCoefWts[eWtQuad] = coefWt(wtQuad, "wtQuad");
  mCoefWts[eWtCub] = coefWt(wtCub, "wtCub");
}

/**
 * Get weights used to constrain polynomial coefficients
 *
 * \param [out] wtOff weight for offset terms
 * \param [out] wtLin weight for linear terms
 * \param [out] wtQuad weight for quadratic terms
 * \param [out] wtCub weight for cubic terms
 */
void RationalFit::GetCoefWeights(double& wtOff,
  double& wtLin, double& wtQuad, double& wtCub) const {
  wtOff = mCoefWts[eWtOff];
  wtLin = mCoefWts[eWtLin];
  wtQuad = mCoefWts[eWtQuad];
  wtCub = mCoefWts[eWtCub];
}

/**
 *  \brief calculate coefficients from given points.
 *
 *  \param [in] images an array of image grid points.
 *    The line/pixel coordinate system has the origin (0.0, 0.0) corresponding
 *    to the upper-left corner of the upper-left pixel, i.e., the pixel centre
 *    of the upper-left pixel is at (0.5, 0.5).
 *  \param [in] grounds an array of the corresponding ground cube points
 */
UPRationalModel RationalFit::CreateRationalModel(
  const std::vector<Point2f>& images, const std::vector<Point3d>& grounds) {
  const size_t count = grounds.size();
  if (count != images.size()) {
    throw std::invalid_argument(_("image grid and ground cube are different in size."));
  }

  // Convert row/col coordinates from raster CS to indices.
  // The RPCs internally uses the pixel centre as the origin.                                                         */
  std::vector<Point2d> imgs0;
  std::transform(images.cbegin(), images.cend(), std::back_inserter(imgs0),
    [](const Point2f& img) -> Point2d { return (img - 0.5f); });

  CalcNormalFactors(imgs0, grounds);

  auto offsetScale = [this](const double val, const int idx) {
    return (val - mOffFactors[idx]) * mInvSclFactors[idx];
  };

  std::vector<Point2d> imgs(count);  // normalized image coordinates
  std::vector<Point3d> grds(count); // normalized ground coordinates
  for (size_t i = 0; i < count; ++i) {
    imgs[i].x = offsetScale(imgs0[i].x, eSAMP);
    imgs[i].y = offsetScale(imgs0[i].y, eLINE);
    grds[i].x = offsetScale(grounds[i].x, eLON);
    grds[i].y = offsetScale(grounds[i].y, eLAT);
    grds[i].z = offsetScale(grounds[i].z, eHGT);
  }

  PolyCoefArray lineNumCoef{}, lineDenCoef{1.0, 0.0};
  PolyCoefArray sampNumCoef{}, sampDenCoef{1.0, 0.0};
  switch (mRfmType) {
  case kStandard:
    SolveStandardEigen(imgs, grds,
      lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef);
    break;
  case kEqDenom:
    SolveEqualDenomEigen(imgs, grds,
      lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef);
    break;
  case kUnitDenom:
    SolveUnitDenomEigen(imgs, grds, lineNumCoef, sampNumCoef);
    break;
  default:
    assert(false);
  }

  std::array<double, rfi::NumFact> nfc{};
  GetNormalFactors(nfc);

  auto groundSRS = std::make_shared<OGRSpatialRef>("WGS84");
  auto ratModel = std::make_unique<RationalModel>(nfc, 
    lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef, groundSRS);
  return ratModel;
}

/**
 *  \brief update normalization factors
 */
void RationalFit::CalcNormalFactors(const std::vector<Point2d>& images,
  const std::vector<Point3d>& grounds) {
  if (kNumOffScl > int(mOffFlags.count()) || 
      kNumOffScl > int(mSclFlags.count())) {
    CalcNormalFactors(images, imgGetters, 0);
    CalcNormalFactors(grounds, grdGetters, 2);
  }

  std::transform(mSclFactors.cbegin(), mSclFactors.cend(),
    mInvSclFactors.begin(), [](const double val) { return 1.0 / val; });
}

/**
 *  \brief solve coefficients from given points using iterative weighted LS.
 * 
 *    (N + P + r) * dx = t + P * (x - x0) + r * dx
 *  
 *  \param [in] images an array of normalized image grid points. 
 *    The line/sample coordinate system has the origin (0.0, 0.0) 
 *    corresponding to the center of the upper-left pixel.
 *  \param [in] grounds an array of the corresponding normalized ground cube points
 *  \param [in] lineNumCoef numerator polynomial of line formulae
 *  \param [in] lineDenCoef denominator polynomial of line formulae
 *  \param [in] sampNumCoef numerator polynomial of sample formulae
 *  \param [in] sampDenCoef denominator polynomial of sample formulae
 * 
 *  \todo Levenberg-Marquad
 *  \todo constrain 3rd-order coefficients that is 1.0e-3
 */
void RationalFit::SolveStandardEigen(const std::vector<Point2d>& images,
  const std::vector<Point3d>& grounds,
  PolyCoefArray& lineNumCoef, PolyCoefArray& lineDenCoef,
  PolyCoefArray& sampNumCoef, PolyCoefArray& sampDenCoef) {
  const size_t count = images.size();

  EigVecX<double> P(kNumUnks_STD); // weights of 39 unkown coefs
  EigVecX<double> T(kNumUnks_STD); // trust regions of 39 unkown coefs
  EigVecXR<double> A(kNumUnks_STD);  // dF_dx
  EigMatXR<double> N(kNumUnks_STD, kNumUnks_STD);  // normal matrix
  EigVecX<double> t(kNumUnks_STD); // normal misclosure
  EigVecX<double> dx(kNumUnks_STD); // correction to unkowns
  EigVecX<double> x(kNumUnks_STD); // unkowns

  int ci = 0;
  std::vector<double> arr(count);
  
  for (auto& arrGetter : imgGetters) {
    std::transform(images.cbegin(), images.cend(), arr.begin(), arrGetter);

    std::bitset<kNumUnks_STD> coefFlags;  // flag of 39 coefficients
    coefFlags[0] = mCoefFlags[0];
    for (int i = 1; i < rpc::NumCoef; ++i) {
      coefFlags[i] = mCoefFlags[i];
      coefFlags[i+rpc::NumCoef-1] = mCoefFlags[i];
    }

    GetCoefWeights(P.data());
    GetCoefTrustRegions(T.data());

    int it{};
    for (it = 0; it < mMaxCC; ++it) {
      N.setZero();

      for (size_t k = 0; k < count; ++k) {
        CalcJacobian(grounds[k], arr[k], coefFlags, A);
        N += A.transpose() * A; // add AtA (\todo upper triangle)
        //N.triangularView<Upper>() += A.transpose() * A;
      }

      // add weights and unbiased regularization
      N.diagonal() += (P.array() + mR).matrix();
      //N.triangularView<StrictlyLower>() = N.transpose();  // selfadjointView<Upper>()

      //const auto& ldlt = N.ldlt();
      //std::cout << "rcond: " << ldlt.rcond() << std::endl;

      // cross-correlation analysis \todo denominator
      const auto [mcc, rl, cl] = LocateMaxCorrelation(N);
      if (mCCTol >= mcc) break;

      // deactivate the strongly correlated higher-order term
      int idx{};
      if ((rpc::NumCoef > rl && rpc::NumCoef > cl) ||   // numerator
          (rpc::NumCoef <= rl && rpc::NumCoef <= cl)) { // denominator
        idx = std::max(rl, cl);
      }
      else {
        idx = (rl > (cl - rpc::NumCoef + 1)) ? rl : cl;
      }

      if ((rpc::eC <=idx && idx <= rpc::eH) || 
          (rpc::NumCoef-1+rpc::eL <= idx && idx <= rpc::NumCoef-1+rpc::eH)) {
        std::cout << "Do not deactivate constant or linear term: " << idx << std::endl;
        break;
      }

      coefFlags.reset(idx);
      P[idx] = 1.0;
      std::cout << "max cross-correlation " << mcc << ": " 
        << rl << ", " << cl << " (" << idx << ")" << std::endl;
    }

    // reach allowed max. number of deactivated coefficients
    if (it == mMaxCC) {   
      N.setZero();

      for (size_t k = 0; k < count; ++k) {
        CalcJacobian(grounds[k], arr[k], coefFlags, A);
        N += A.transpose() * A; // add AtA (\todo upper triangle)
        //N.triangularView<Upper>() += A.transpose() * A;
      }

      // add weights and unbiased regularization
      N.diagonal() += (P.array() + mR).matrix();
      //N.triangularView<StrictlyLower>() = N.transpose();  // selfadjointView<Upper>()
    }
    const auto& ldlt = N.ldlt();
    std::cout << "rcond: " << ldlt.rcond() << std::endl;

    dx.setZero();
    x.setZero();

    for (it = 0; it < mMaxIter; ++it) {
      t.setZero();

      for (size_t k = 0; k < count; ++k) {
        CalcJacobian(grounds[k], arr[k], coefFlags, A);
        const double w = arr[k] + A.dot(x); // misclosure
        t += A.transpose() * w;   // add Atw
      }

      // add weights and unbiased regularization
      t += P.cwiseProduct(x) + dx * mR;

      dx = ldlt.solve(t); // correction to coefficients
      EnforceTrustRegions(T, dx);

      x -= dx;
      EnforceTrustRegions(T, x);

      if (mTol > dx.lpNorm<Eigen::Infinity>())
        break;
    }

    if (0 == ci++) {
      std::copy_n(x.data(), rpc::NumCoef, lineNumCoef.begin());
      std::copy_n(x.data() + rpc::NumCoef, rpc::NumCoef - 1, lineDenCoef.begin() + 1);
    }
    else {
      std::copy_n(x.data(), rpc::NumCoef, sampNumCoef.begin());
      std::copy_n(x.data() + rpc::NumCoef, rpc::NumCoef - 1, sampDenCoef.begin() + 1);
    }
  }
}

void RationalFit::SolveEqualDenomEigen(const std::vector<Point2d>& images,
  const std::vector<Point3d>& grounds,
  PolyCoefArray& lineNumCoef, PolyCoefArray& lineDenCoef,
  PolyCoefArray& sampNumCoef, PolyCoefArray& sampDenCoef) {

}

void RationalFit::SolveUnitDenomEigen(const std::vector<Point2d>& images,
  const std::vector<Point3d>& grounds,
  PolyCoefArray& lineNumCoef, PolyCoefArray& sampNumCoef) {

}

void RationalFit::SolveStandardCeres(const std::vector<Point2d>& images,
  const std::vector<Point3d>& grounds,
  PolyCoefArray& lineNumCoef, PolyCoefArray& lineDenCoef,
  PolyCoefArray& sampNumCoef, PolyCoefArray& sampDenCoef) {

}

/**
 *  \brief get weights of 39 unknown coefficients
 */
void RationalFit::GetCoefWeights(double* const weights) const {
  weights[rpc::eC] = mCoefWts[eWtOff];
  for (std::underlying_type_t<rpc> idx = rpc::eL; idx <= rpc::eH; ++idx) {
    weights[idx] = mCoefWts[eWtLin];
  }
  for (std::underlying_type_t<rpc> idx = rpc::eLP; idx <= rpc::eHH; ++idx) {
    weights[idx] = mCoefWts[eWtQuad];
  }
  for (std::underlying_type_t<rpc> idx = rpc::ePLH; idx <= rpc::eHHH; ++idx) {
    weights[idx] = mCoefWts[eWtCub];
  }

  // implicitly disable the un-used coefficients
  for (int idx = 1; idx < rpc::NumCoef; ++idx) {
    if (!mCoefFlags.test(idx))
      weights[idx] = 1.0;
  }
  std::copy_n(weights + 1, rpc::NumCoef - 1, weights + rpc::NumCoef);
}

/**
 *  \brief get trust regions of 39 unknown coefficients
 */
void RationalFit::GetCoefTrustRegions(double* const regions) const {
  regions[rpc::eC] = mCoefTrs[eWtOff];
  for (std::underlying_type_t<rpc> idx = rpc::eL; idx <= rpc::eH; ++idx) {
    regions[idx] = mCoefTrs[eWtLin];
  }
  for (std::underlying_type_t<rpc> idx = rpc::eLP; idx <= rpc::eHH; ++idx) {
    regions[idx] = mCoefTrs[eWtQuad];
  }
  for (std::underlying_type_t<rpc> idx = rpc::ePLH; idx <= rpc::eHHH; ++idx) {
    regions[idx] = mCoefTrs[eWtCub];
  }

  // implicitly disable the un-used coefficients
  for (int idx = 1; idx < rpc::NumCoef; ++idx) {
    if (!mCoefFlags.test(idx))
      regions[idx] = 0.0;
  }
  std::copy_n(regions + 1, rpc::NumCoef - 1, regions + rpc::NumCoef);
}

/**
 *  \brief calculate Jacobian of rational functions.
 */
void RationalFit::CalcJacobian(const Point3d& grd, const double obs,
  const std::bitset<kNumUnks_STD>& coefFlags, EigVecXR<double>& A) const {
  PolyCoefArray uvw{};  // combinations of u, v, w for polynomial terms
  CalcTerms(grd, uvw.data());  

  std::transform(uvw.cbegin(), uvw.cend(), A.data(), std::negate());
  std::transform(uvw.cbegin() + 1, uvw.cend(), A.data() + rpc::NumCoef,
    [obs](const double val) { return obs * val; });

  for (int i = 0; i < kNumUnks_STD; ++i) {
    if (!coefFlags.test(i))
      A[i] = 0.0;
  }
}

/**
 *  \brief clamp coefficients within trust regions.
 */
void RationalFit::EnforceTrustRegions(const EigVecX<double>& T,
  EigVecX<double>& x) const {
  const int n = x.rows();
  assert(n == T.rows());

  for (int i = 0; i < n; ++i) {
    if (fabs(x[i]) > T[i]) {
      x[i] = std::copysign(T[i], x[i]);
    }
  }
}
