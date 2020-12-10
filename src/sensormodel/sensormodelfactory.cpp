/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/sensormodelfactory.h"
#include "core/exceptions/exexcept.h"
#include "sensormodel/utils/sensorcube.h"

using namespace earthxpert;

static constexpr int kNUMINT = 10;  // number of intervals

/**
 *  \brief
 */
UPISensorModel SensorModelFactory::CreateSensorModel() {
  return UPISensorModel();
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
std::unique_ptr<FilmFrameModel> 
SensorModelFactory::CreateFilmFrameModel() {
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
  int numHoriz{ kNUMINT }, numVert{ kNUMINT }, numLayers{ kNUMINT };
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
