/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/sensormodelfactory.h"
#include "core/exceptions/exexcept.h"
#include "sensormodel/rationalfit.h"
#include "sensormodel/utils/sensorcube.h"
#include "sensormodel/utils/smutils.h"

using namespace earthxpert;

namespace {
  constexpr int kNUMINT = 10;  // number of intervals
}

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
  int numHoriz{ kNUMINT }, numVert{ kNUMINT }, numLayers{ kNUMINT };
  CubeType cubeType{ CubeType::eMODEL };
  SensorCube sensorCube(numHoriz, numVert, numLayers, cubeType);
  sensorCube.SetMargins(100, 100);

  // fit cube
  std::vector<Point2f> images;
  std::vector<Point3d> grounds;
  sensorCube.CreateCube(sensorModel, images, grounds);

  RationalFit ratFit;
  auto ratModelF = ratFit.CreateRationalModel(images, grounds);

  // original accuracy
  std::array<double, 4> sstats{}, lstats{};
  CalcStatitics(sensorModel, images, grounds, sstats, lstats);

  // Fit accuracy
  std::array<double, 4> sstatsF{}, lstatsF{};
  CalcStatitics(ratModelF.get(), images, grounds, sstatsF, lstatsF);

  // QC cube
  std::vector<Point2f> imagesQC;
  std::vector<Point3d> groundsQC;
  sensorCube.CreateCubeQC(sensorModel, imagesQC, groundsQC);

  // original accuracy QC
  std::array<double, 4> sstatsQC{}, lstatsQC{};
  CalcStatitics(sensorModel, imagesQC, groundsQC, sstatsQC, lstatsQC);

  // Fit accuracy QC
  std::array<double, 4> sstatsFQC{}, lstatsFQC{};
  CalcStatitics(ratModelF.get(), imagesQC, groundsQC, sstatsFQC, lstatsFQC);

  return ratModelF;
}
