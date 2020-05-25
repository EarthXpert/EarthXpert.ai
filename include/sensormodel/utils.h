/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/exsensormodel.h"
#include "sensormodel/isensormodel.h"
#include <string>

namespace earthxpert {
ISensorModel::ModelType EXSENSORMODEL_EXPORTS SensorNameToType(const std::string& name);
std::string EXSENSORMODEL_EXPORTS SensorTypeToName(ISensorModel::ModelType type);
}