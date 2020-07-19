/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/exsensormodel.h"
#include <utility>

namespace earthxpert {
class ISensorModel;
class RationalModel;

/**
 *  \brief the class EvalSensorModel checks the accuracy and consistency
 *  of the internals of a senosr model. The idea has two folds: first is
 *  accuracy in image metric [pixels], it builds a 2d grid in image, project grid nodes
 *  to multiple flat layers in ground, re-project back to image, and calculate
 *  statistics at individual nodes and linearity along grid edges;
 *  second is accuracy in ground metric [mm], it builds a rectilinear 3d cube in ground, 
 *  project cube nodes to image, re-project to ground, and calculate statistics 
 *  at individual nodes and linearity along cube edges 
 */
class EXSENSORMODEL_EXPORTS EvalSensorModel {
public:
  ~EvalSensorModel() = default;

  std::pair<float, double> EvalModel(const ISensorModel* model);
  std::pair<float, double> EvalFrameModel(const ISensorModel* model);
  std::pair<float, double> EvalRationalModel(const RationalModel* model);
};
}
