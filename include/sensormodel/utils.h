/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/exsensormodel.h"
#include "3rdparty/boost/exboost.h"
#include "sensormodel/isensormodel.h"
#include <string>

namespace earthxpert {
class EarthEllipsoid;

ISensorModel::ModelType EXSENSORMODEL_EXPORTS SensorNameToType(const std::string& name);
std::string EXSENSORMODEL_EXPORTS SensorTypeToName(ISensorModel::ModelType type);

double EXSENSORMODEL_EXPORTS CalcGsd(const ISensorModel* sensorModel, double height=0);
double EXSENSORMODEL_EXPORTS CalcGsd(const ISensorModel* sensorModel,
  const EarthEllipsoid* ee, double height);

bool EXSENSORMODEL_EXPORTS ImageToPlane(const Point2d& raw,
  const ISensorModel* sensorModel, const EarthEllipsoid* ee, 
  const double height, PointXYd& rect, double tolH=1.0e-4);

Polygon2d EXSENSORMODEL_EXPORTS CalcFootprint(const ISensorModel* sensorModel,
  const EarthEllipsoid* ee, double height);
Polygon2d EXSENSORMODEL_EXPORTS 
CalcFootprint(const ISensorModel* sensorModel, double height);
}
