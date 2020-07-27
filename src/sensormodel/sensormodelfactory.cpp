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
 *  \brief create rational model from another sensor model using 
 *  a grid of image points and the corresponding cube of ground points.
 *
 *  \details 1) fitting points: rpc parameters;
 *           2) validation points: hyper-parameters, i.e., dimensions of cube
 *           3) testing points: generalization accuracy.
 */
std::unique_ptr<RationalModel> 
SensorModelFactory::CreateRationalModel(const ISensorModel* sensorModel) {
  std::array<double, rnf::NumFact> normalFators;
  int numHoriz{ 10 }, numVert{ 10 }, numLayers{ 10 };
  CubeType cubeType{ CubeType::eMODEL };
  SensorCube sensorCube(normalFators, numHoriz, numVert, numLayers, cubeType);

  std::vector<Point2f> images;
  std::vector<Point3d> grounds;
  sensorCube.CreateCube(sensorModel, images, grounds);

  //RationalFit ratFit;

  // Fit accuracy

  // QC accuracy
  std::vector<Point2f> imagesQC;
  std::vector<Point3d> groundsQC;
  sensorCube.CreateCubeQC(sensorModel, imagesQC, groundsQC);

  return std::unique_ptr<RationalModel>();
}
