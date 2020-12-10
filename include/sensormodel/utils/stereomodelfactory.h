/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/rationalstereomodel.h"

namespace earthxpert {
/**
 *  The class StereoModelFactory defines the behaviours for creating
 *  stereo model from overlapping image products with sensor models.
 */
class EXSENSORMODEL_EXPORTS StereoModelFactory {
public:
  /**
   *  \brief Form a stereo model by a pair of sensor models.
   *
   *  \param [in] leftModel left sensor model composing the stereo model
   *  \param [in] rightModel right sensor model composing the stereo model
   */
  static std::unique_ptr<IStereoModel> 
  CreateStereoModel(ISensorModel const* leftModel, 
    ISensorModel const* rightModel, StereoFwdAlgo fwdAlgo);

  /**
   *  \brief create rational stereo model
   */
  static std::unique_ptr<RationalStereoModel> CreateRationalStereoModel();
};
} // namespace earthxpert
