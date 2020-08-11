/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/exepipolarmodel.h"
#include <utility>

namespace earthxpert {
class IEpipolarModel;

/**
 *  \brief the class EvalEpipolarModel checks the accuracy and consistency
 *  of the internals of an epipolar model.
 */
class EXEPIPOLARMODEL_EXPORTS EvalEpipolarModel {
public:
  ~EvalEpipolarModel() = default;

  std::pair<float, double> EvalModel(const IEpipolarModel* model);
};
}
