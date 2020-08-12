/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/exsensormodel.h"
#include <utility>

namespace earthxpert {
class IStereoModel;

/**
 *  \brief the class EvalStereoModel checks the accuracy and consistency
 *  of the internals of a stereo model.
 */
class EXSENSORMODEL_EXPORTS EvalStereoModel {
public:
  ~EvalStereoModel() = default;

  std::pair<float, double> EvalModel(const IStereoModel* model);
};
}
