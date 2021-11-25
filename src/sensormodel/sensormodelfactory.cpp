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
SensorModelFactory::CreateRationalModel(const ISensorModel* sensorModel,
  const int numInt, const int numLyr, const int margin, const bool cc) {
  CubeType cubeType{ CubeType::eMODEL };
  SensorCube sensorCube(numInt, numInt, numLyr, cubeType);
  sensorCube.SetMargins(margin, margin);

  // fit cube
  std::vector<Point2f> images;
  std::vector<Point3d> grounds;
  sensorCube.CreateCube(sensorModel, images, grounds);

  const int maxIters = cc ? 10 : 0;

  RationalFit ratFit;
  ratFit.SetCrossCorelation(maxIters, 0.9);
  auto ratModelF = ratFit.CreateRationalModel(images, grounds);

  auto print = [](const std::array<double, 4>& stats, const std::string& name) {
    std::cout << name << " mean: " << stats[0] << " rms: " << stats[1]
      << " min: " << stats[2] << " max: " << stats[3] << std::endl;
  };

  // original accuray
  //std::array<double, 4> sstats{}, lstats{};
  //CalcStatitics(sensorModel, images, grounds, sstats, lstats);
  //print(lstats, "original model line -");
  //print(sstats, "original model samp -");

  // Fit accuracy
  std::array<double, 4> sstatsF{}, lstatsF{};
  CalcStatitics(ratModelF.get(), images, grounds, sstatsF, lstatsF);
  print(lstatsF, "\nfitted model line -");
  print(sstatsF, "fitted model samp -");

  // QC cube
  std::vector<Point2f> imagesQC;
  std::vector<Point3d> groundsQC;
  sensorCube.CreateCubeQC(sensorModel, imagesQC, groundsQC);

  // original accuracy QC
  //std::array<double, 4> sstatsQC{}, lstatsQC{};
  //CalcStatitics(sensorModel, imagesQC, groundsQC, sstatsQC, lstatsQC);
  //print(lstatsQC, "original model line QC -");
  //print(sstatsQC, "original model samp QC -");

  // Fit accuracy QC
  std::array<double, 4> sstatsFQC{}, lstatsFQC{};
  CalcStatitics(ratModelF.get(), imagesQC, groundsQC, sstatsFQC, lstatsFQC);
  print(lstatsFQC, "\nfitted model line QC -");
  print(sstatsFQC, "fitted model samp QC -");

  return ratModelF;
}
