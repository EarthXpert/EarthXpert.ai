/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/utils/sensorcube.h"
#include "sensormodel/isensormodel.h"

using namespace earthxpert;

SensorCube::SensorCube(const std::array<double, NumFact>& normalFators,
  const int numHoriz, const int numVert, const int numLayers)
  : mNormalFators(normalFators), mNumHoriz(numHoriz), 
  mNumVert(numVert), mNumLayers(numLayers) {
  Initialize();
}

void SensorCube::Initialize() {

}

int SensorCube::CreateCube(const ISensorModel* sensorModel,
  std::vector<Point2f>& images, std::vector<Point3d>& grounds) const {
  return 0;
}
