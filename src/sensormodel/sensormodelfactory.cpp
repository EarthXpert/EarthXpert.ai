/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/sensormodelfactory.h"
#include "sensormodel/utils/sensorcube.h"

using earthxpert::DigitalFrameModel;
using earthxpert::FilmFrameModel;
using earthxpert::ISensorModel;
using earthxpert::RationalModel;
using earthxpert::SensorModelFactory;

/**
 *  \brief
 */
std::unique_ptr<ISensorModel> SensorModelFactory::CreateSensorModel() {
  return std::unique_ptr<ISensorModel>();
}

/**
 *  \brief
 */
std::unique_ptr<DigitalFrameModel>
SensorModelFactory::CreateDigitalFrameModel() {
  return std::unique_ptr<DigitalFrameModel>();
}

/**
 *  \brief
 */
std::unique_ptr<FilmFrameModel> SensorModelFactory::CreateFilmFrameModel() {
  return std::unique_ptr<FilmFrameModel>();
}

/**
 *  \brief create rational model from another sensor model
 */
std::unique_ptr<RationalModel> 
SensorModelFactory::CreateRationalModel(const ISensorModel* sensorModel) {
  std::array<double, rnf::NumFact> normalFators;
  int numHoriz{ 10 };
  int numVert{ 10 };
  int numLayers{ 10 };
  CubeType cubeType{CubeType::eMODEL};
  SensorCube sensorCube(normalFators, numHoriz, numVert, numLayers, cubeType);

  std::vector<Point2f> images;
  std::vector<Point3d> grounds;
  sensorCube.CreateCube(sensorModel, images, grounds);

  return std::unique_ptr<RationalModel>();
}
