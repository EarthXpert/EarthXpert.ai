/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/utils/evalsensormodel.h"

using namespace earthxpert;

/**
 *  \return Returns accuracies in image [pixels] and ground [mm]
 */
std::pair<float, double> EvalSensorModel::EvalModel(const ISensorModel* model) {
  return {};
}

std::pair<float, double> EvalSensorModel::EvalFrameModel(const ISensorModel* model) {
  return {};
}

std::pair<float, double> EvalSensorModel::EvalRationalModel(const RationalModel* model) {
  return {};
}
