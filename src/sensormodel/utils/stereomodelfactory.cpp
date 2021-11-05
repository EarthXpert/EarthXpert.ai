/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/utils/stereomodelfactory.h"

using namespace earthxpert;

/**
 *  \brief Form a stereo model by a pair of sensor models.
 *
 *  \param [in] leftModel left sensor model composing the stereo model
 *  \param [in] rightModel right sensor model composing the stereo model
 */
std::unique_ptr<IStereoModel> 
StereoModelFactory::CreateStereoModel(ISensorModel const* leftModel,
  ISensorModel const* rightModel, StereoFwdAlgo fwdAlgo) {
  auto sm = std::make_unique<StereoModel>(leftModel, rightModel);
  sm->SetForwardAlgorithm(fwdAlgo);
  return sm;
}

/**
 *  \brief create rational stereo model
 */
std::unique_ptr<RationalStereoModel> 
StereoModelFactory::CreateRationalStereoModel() {
  return {};
}
