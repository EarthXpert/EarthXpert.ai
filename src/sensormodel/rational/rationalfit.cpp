/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/rationalfit.h"
#include "core/exceptions/exexcept.h"
#include "core/math/mutils.h"

using namespace earthxpert;

RationalFit::RationalFit(const int coefGroup) : mCoefGroup(coefGroup) {
  Initialize();
}

void RationalFit::Initialize() {
  SetCoefGroup(mCoefGroup);
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
void RationalFit::SetNormalFactors(const std::array<double, NumFact>& normalFators) {
  std::copy_n(normalFators.cbegin(), NumOffScl, mOffFactors.begin());
  mOffFlags.set();
  std::copy_n(normalFators.cbegin() + NumOffScl, NumOffScl, mSclFactors.begin());
  mSclFlags.set();
}

void RationalFit::GetNormalFactors(std::array<double, NumFact>& normalFators) const {
  std::copy_n(mOffFactors.cbegin(), NumOffScl, normalFators.begin());
  std::copy_n(mSclFactors.cbegin(), NumOffScl, normalFators.begin()+NumOffScl);
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

  std::vector<Point2d> imgs(count);  // normalized image coordinates
  std::vector<Point3d> grds(count); // normalized ground coordinates
  for (size_t i = 0; i < count; ++i) {
    imgs[i].y = (imgs0[i].y - mOffFactors[eLINE]) * mInvSclFactors[eLINE];
    imgs[i].x = (imgs0[i].x - mOffFactors[eSAMP]) * mInvSclFactors[eSAMP];
    grds[i].y = (grounds[i].y - mOffFactors[eLAT]) * mInvSclFactors[eLAT];
    grds[i].x = (grounds[i].x - mOffFactors[eLON]) * mInvSclFactors[eLON];
    grds[i].z = (grounds[i].z - mOffFactors[eHGT]) * mInvSclFactors[eHGT];
  }

  std::array<double, rpc::NumCoef> lineNumCoef{}, lineDenCoef{1.0, 0.0};
  std::array<double, rpc::NumCoef> sampNumCoef{}, sampDenCoef{1.0, 0.0};
  switch (mRfmType) {
  case kStandard:
    SolveStandard(imgs, grds,
      lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef);
    break;
  case kEqDenom:
    SolveEqualDenom(imgs, grds,
      lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef);
    break;
  case kUnitDenom:
    SolveUnitDenom(imgs, grds, lineNumCoef, sampNumCoef);
    break;
  default:
    assert(false);
  }

  std::array<double, rnf::NumFact> nfc{};
  GetNormalFactors(nfc);

  auto groundSRS = std::make_shared<OGRSpatialRef>("WGS84");
  auto ratModel = std::make_unique<RationalModel>(nfc, 
    lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef, groundSRS);
  return ratModel;
}

void RationalFit::CalcNormalFactors(const std::vector<Point2d>& images,
  const std::vector<Point3d>& grounds) {
  if (NumOffScl > int(mOffFlags.count()) || NumOffScl > int(mSclFlags.count())) {
    // update normalization factors
    std::array<std::function<double(const Point2d&)>, 2> imgGetters{
      [](const Point2d& img) { return img.y; },
      [](const Point2d& img) { return img.x; } };
    CalcNormalFactors(images, imgGetters, 0);

    std::array<std::function<double(const Point3d&)>, 3> grdGetters{
      [](const Point3d& grd) { return grd.y; },
      [](const Point3d& grd) { return grd.x; },
      [](const Point3d& grd) { return grd.z; } };
    CalcNormalFactors(grounds, grdGetters, 2);
  }

  std::transform(mSclFactors.cbegin(), mSclFactors.cend(),
    mInvSclFactors.begin(), [](const double val) { return 1.0 / val; });
}

/**
 *  \brief solve coefficients from given points.
 *  
 *  \param [in] images an array of image grid points. 
 *    The line/sample coordinate system has the origin (0.0, 0.0) 
 *    corresponding to the center of the upper-left pixel.
 *  \param [in] grounds an array of the corresponding ground cube points
 *  \param [in] lineNumCoef numerator polynomial of line formulae
 *  \param [in] lineDenCoef denominator polynomial of line formulae
 *  \param [in] sampNumCoef numerator polynomial of sample formulae
 *  \param [in] sampDenCoef denominator polynomial of sample formulae
 */
void RationalFit::SolveStandard(const std::vector<Point2d>& images,
  const std::vector<Point3d>& grounds,
  std::array<double, rpc::NumCoef>& lineNumCoef,
  std::array<double, rpc::NumCoef>& lineDenCoef,
  std::array<double, rpc::NumCoef>& sampNumCoef,
  std::array<double, rpc::NumCoef>& sampDenCoef) {

}

void RationalFit::SolveEqualDenom(const std::vector<Point2d>& images,
  const std::vector<Point3d>& grounds,
  std::array<double, rpc::NumCoef>& lineNumCoef,
  std::array<double, rpc::NumCoef>& lineDenCoef,
  std::array<double, rpc::NumCoef>& sampNumCoef,
  std::array<double, rpc::NumCoef>& sampDenCoef) {

}

void RationalFit::SolveUnitDenom(const std::vector<Point2d>& images,
  const std::vector<Point3d>& grounds,
  std::array<double, rpc::NumCoef>& lineNumCoef,
  std::array<double, rpc::NumCoef>& sampNumCoef) {

}
