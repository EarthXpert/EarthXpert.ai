/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/utils/sensorcube.h"
#include "sensormodel/isensormodel.h"

using namespace earthxpert;

SensorCube::SensorCube(const std::array<double, NumFact>& normalFators,
  const int numHoriz, const int numVert, 
  const int numLayers, const LayerType layerType)
  : mNormalFators(normalFators), mNumHoriz(numHoriz), 
  mNumVert(numVert), mNumLayers(numLayers), mLayerType(layerType) {
  Initialize();
}

void SensorCube::Initialize() {
  mScaleLine = mNormalFators[eSLINE] * (1.0 + kMarginRatio);
  mScaleSamp = mNormalFators[eSSAMP] * (1.0 + kMarginRatio);
}

int SensorCube::CreateCube(const ISensorModel* sensorModel,
  std::vector<Point2f>& images, std::vector<Point3d>& grounds) const {
  return 0;
}

int SensorCube::CreateCubeFlat(const ISensorModel* sensorModel,
  std::vector<Point2f>& images, std::vector<Point3d>& grounds) const {
  return 0;
}

int SensorCube::CreateCubeModel(const ISensorModel* sensorModel,
  std::vector<Point2f>& images, std::vector<Point3d>& grounds) const {
  return 0;
}
