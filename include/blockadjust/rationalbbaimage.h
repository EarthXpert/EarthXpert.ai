/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "blockadjust/bbaimage.h"
#include "sensormodel/rationalmodel.h"

namespace earthxpert {
/**
 *  The interface RationalBbaImage defines the behaviours of image bba info.
 */
class EXBBA_EXPORTS RationalBbaImage final : public BbaImage {
public:
  virtual ~RationalBbaImage() = default;

  bool operator==(const RationalBbaImage& rhs) const = delete;
  bool operator!=(const RationalBbaImage& rhs) const = delete;

  // \todo apply correction polynomials
  bool ImageToGround(const Point2d& image, Point3d& ground) const override {
    return mRatModel->ImageToGround(image, ground);
  }

  bool GroundToImage(const Point3d& ground, Point2d& image) const override {
    return mRatModel->GroundToImage(ground, image);
  }

  void UpdateModel() override {
    mRatModel->SetCorrection(mLineCorr, mSampCorr, mCorrOrder);
  }

private:
  friend class RationalCeresSolver;

  RationalModel* mRatModel{};

  int64_t mIndex{};   // index to the vector of unknowns
  double* mLineCorr{}; // polynomial for line correction
  double* mSampCorr{}; // polynomial for sample correction
  int mCorrOrder{};   // order of correction polynomial
};

using UPRationalBbaImage = std::unique_ptr<RationalBbaImage>;
using SPRationalBbaImage = std::shared_ptr<RationalBbaImage>;
} // namespace earthxpert
