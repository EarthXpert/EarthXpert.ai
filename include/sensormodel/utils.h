/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/wxsensormodel.h"
#include "sensormodel/isensormodel.h"
#include <string>

namespace watxtract {
ISensorModel::ModelType WXSENSORMODEL_EXPORTS SensorNameToType(const std::string& name);
std::string WXSENSORMODEL_EXPORTS SensorTypeToName(ISensorModel::ModelType type);
}