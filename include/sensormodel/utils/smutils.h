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
class RationalModel;

ISensorModel::ModelType EXSENSORMODEL_EXPORTS SensorNameToType(const std::string& name);
std::string EXSENSORMODEL_EXPORTS SensorTypeToName(ISensorModel::ModelType type);

double EXSENSORMODEL_EXPORTS 
CalcGsd(const ISensorModel* sensorModel, double height = 0);
double EXSENSORMODEL_EXPORTS CalcGsd(const ISensorModel* sensorModel,
 const Point2d& ip, double height = 0);

double EXSENSORMODEL_EXPORTS CalcGsd(const ISensorModel* sensorModel,
  const EarthEllipsoid* ee, double height);
double EXSENSORMODEL_EXPORTS CalcGsd(const ISensorModel* sensorModel,
  const Point2d& ip, const EarthEllipsoid* ee, double height);

void EXSENSORMODEL_EXPORTS RegisterMulPan(const ISensorModel* mulModel,
  const ISensorModel* panModel, int ratio = 4);

bool EXSENSORMODEL_EXPORTS ImageToPlane(const Point2d& raw,
  const ISensorModel* sensorModel, const EarthEllipsoid* ee, 
  const double height, PointXYd& rect, double tolH=1.0e-4);

Polygon2d EXSENSORMODEL_EXPORTS CalcFootprint(const ISensorModel* sensorModel,
  const EarthEllipsoid* ee, double height);
Polygon2d EXSENSORMODEL_EXPORTS 
CalcFootprint(const ISensorModel* sensorModel, double height);

void EXSENSORMODEL_EXPORTS CalcStatitics(const ISensorModel* sensorModel,
  const std::vector<Point2f>& images, const std::vector<Point3d>& grounds);

std::string EXSENSORMODEL_EXPORTS ToString(const RationalModel* ratModel);
}
