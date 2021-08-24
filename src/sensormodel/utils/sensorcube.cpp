/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/utils/sensorcube.h"
#include "core/exceptions/exexcept.h"
#include "sensormodel/isensormodel.h"
#include "sensormodel/rationalmodel.h"

using namespace earthxpert;

SensorCube::SensorCube(const int numHoriz, const int numVert, 
  const int numLayers, const LayerType layerType)
  : mNumHoriz(numHoriz), mNumVert(numVert), 
    mNumLayers(numLayers), mLayerType(layerType) {
  Initialize();
}

void SensorCube::Initialize() {
}

/**
 *  \brief create a 3d cube for rational fitting.
 *
 *  \param [in] images an array of image grid points.
 *    The line/pixel coordinate system has the origin
 *    (0.0, 0.0) corresponding to the upper-left corner of
 *    the upper-left pixel, i.e., the pixel centre of the
 *    upper-left pixel is at (0.5, 0.5).
 *  \param [in] grounds an array of the corresponding ground cube points
 */
int SensorCube::CreateCube(const ISensorModel* sensorModel,
  std::vector<Point2f>& images, std::vector<Point3d>& grounds) const {
  switch (mLayerType) {
  case eFLAT:
    CreateCubeFlat(sensorModel, images, grounds);
    break;
  case eMODEL:
    CreateCubeModel(sensorModel, images, grounds);
    break;
  default:
    assert(false);
  }

  return 0;
}

/**
 *  \brief create a 3d cube for accuracy QC.
 */
int SensorCube::CreateCubeQC(const ISensorModel* sensorModel,
  std::vector<Point2f>& images, std::vector<Point3d>& grounds) const {
  return 0;
}

int SensorCube::CreateCubeFlat(const ISensorModel* sensorModel,
  std::vector<Point2f>& images, std::vector<Point3d>& grounds) const {
  return 0;
}

int SensorCube::CreateCubeModel(const ISensorModel* sensorModel,
  std::vector<Point2f>& images, std::vector<Point3d>& grounds) const {
  const auto [numCols, numRows] = sensorModel->GetImageSize();
  const auto [minHgt, maxHgt] = sensorModel->GetHeightRange();

  const auto stepCols = (numCols + mMarginCols * 2) / mNumHoriz;
  const auto stepRows = (numRows + mMarginRows * 2) / mNumVert;
  const auto stepHgt = (maxHgt - minHgt) / mNumLayers;

  for (auto col = -mMarginCols; col <= numCols+mMarginCols; col += stepCols) {
    for (auto row = -mMarginRows; row <= numRows+mMarginRows; row += stepRows) {
      Point2d image{ double(col), double(row) };
      
      for (auto hgt = minHgt; hgt <= maxHgt; hgt += stepHgt) {
        Point3d ground{ 0, 0, hgt };
        sensorModel->ImageToGround(image, ground);

        images.emplace_back(float(col), float(row));
        grounds.emplace_back(ground);
      }
    }
  }
  return 0;
}
