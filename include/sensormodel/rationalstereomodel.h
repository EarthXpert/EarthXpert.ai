/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/stereomodel.h"

namespace earthxpert {
class ISensorModel;
class RationalModel;

/**
 *  The class RationalStereoModel implements the dedicated methods having logic
 *  specific to rational sensor systems for better perforamnce. In detail, 
 *  the bundle of rays from a block of sensor geometry models are used to implement
 *  forward intersection and backward projection in the network.
 */
class EXSENSORMODEL_EXPORTS RationalStereoModel final : public StereoModel {
public:
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
  bool ImageToGround(const Point2d& leftImage,
    const Point2d& rightImage, Point3d& ground) const override;

private:
  void Initialize();

  // cached for convenience/performance
  std::vector<const RationalModel*> mModels;   // one model per ray
};
}
