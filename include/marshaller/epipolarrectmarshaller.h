/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/epipolarrect.h"
#include "marshaller/iepipolarmodelmarshaller.h"
#include <cereal/archives/json.hpp>
#include <memory>

namespace watxtract {

/**
 *  The class EpipolarRectMarshaller serialize and deserialize the
 *  epipolar rect
 */
class EpipolarRectMarshaller {
public:
  EpipolarRectMarshaller(const EpipolarRect *epiRect);

  EpipolarRectMarshaller() {}
  ~EpipolarRectMarshaller() {}

  /**
   *  brief get the pointer of the epipolar rect
   *
   *  \return the pointer to the epipolar rect
   *
   */
  std::unique_ptr<EpipolarRect> GetRect();

  /**
   *  brief the seralize function for Cereal library
   */
  template <class Archive> void serialize(Archive &archive) {
    archive(cereal::make_nvp("xOffset", mXOffset),
            cereal::make_nvp("yOffset", mYOffset),
            cereal::make_nvp("scale", mScale),
            cereal::make_nvp("rotation", mRotation),
            cereal::make_nvp("height", mHeight),
            cereal::make_nvp("numRows", mNumRows),
            cereal::make_nvp("origin", mOrigin));
  }

private:
  double mXOffset{};
  double mYOffset{};
  double mScale{};
  double mRotation{};
  double mHeight{};
  int mNumRows{};
  std::array<double, 3> mOrigin{};

  double mSinRot{};
  double mCosRot{};
  double mInvScale{};
};
} // namespace watxtract
