/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "blockadjust/bbaimage.h"
#include "sensormodel/rationalmodel.h"

namespace earthxpert {
/**
 *  The class RationalBbaImage defines the behaviours of image bba info.
 */
class EXBBA_EXPORTS RationalBbaImage final : public BbaImage {
public:
  ~RationalBbaImage() = default;

  // \todo apply correction polynomials
  bool ImageToGround(const Point2d& image, Point3d& ground) const override {
    return mRatModel->ImageToGround(image, ground);
  }

  bool GroundToImage(const Point3d& ground, Point2d& image) const override {
    return mRatModel->GroundToImage(ground, image);
  }

  void SetCorrection(const double* const lineCorr, const double* const sampCorr,
    const int order = 2, const rca corrAlgo = rca::eCA_IMAGE) {
    mLineCorr = lineCorr;
    mSampCorr = sampCorr;
    mCorrOrder = order;
  }

  bool AddPointObs() const {
    return false;
  }

  void UpdateModel() override {
    mRatModel->SetCorrection(mLineCorr, mSampCorr, mCorrOrder);
  }

private:
  friend class RationalCeresSolver;

  RationalModel* mRatModel{};

  int64_t mIndex{};   // index to the vector of unknowns
  int mCorrOrder{-1};   // order of correction polynomial
  const double* mLineCorr{}; // polynomial for line correction
  const double* mSampCorr{}; // polynomial for sample correction

  bool CalcJacobian() const {
    return false;
  }
};

using UPRationalBbaImage = std::unique_ptr<RationalBbaImage>;
using SPRationalBbaImage = std::shared_ptr<RationalBbaImage>;
} // namespace earthxpert
