/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/rationalstereomodel.h"

using namespace earthxpert;

void RationalStereoModel::Initialize() {
  mFwdAlgo = eSM_EXAI;
}

/**
 *  Image-to-ground backward projection (fixed).
 *
 *  \brief Given a pair of sensor models and associated 2d positions
 *  in image coordinate system, do forward intersection of pair-rays, and
 *  output the ground position with respect to the ground coordinate system.
 *
 *  \param [in] leftImage 2d position (x, y) in left image raster coordinate
 * system
 *  \param [in] rightImage the corresponding 2d position (x, y) in right image
 * raster coordinate system
 *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate
 * system
 *  \return Returns true if the operation succeeds
 */
bool RationalStereoModel::ImageToGround(const Point2d& leftImage,
  const Point2d& rightImage, Point3d& ground) const {
  assert(2 == mModels.size());
  return true;
}
