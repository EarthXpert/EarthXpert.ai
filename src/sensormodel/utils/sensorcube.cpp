/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/utils/sensorcube.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/layoututils.h"
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
 *  \brief create a 3d cube for rational fitting, which is equally 
 *  spaced in image and in height range.
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
  switch (mLayerType) {
  case eFLAT:
    CreateCubeFlatQC(sensorModel, images, grounds);
    break;
  case eMODEL:
    CreateCubeModelQC(sensorModel, images, grounds);
    break;
  default:
    assert(false);
  }

  return 0;
}

int SensorCube::CreateCubeFlat(const ISensorModel* sensorModel,
  std::vector<Point2f>& images, std::vector<Point3d>& grounds) const {
  return 0;
}

int SensorCube::CreateCubeModel(const ISensorModel* sensorModel,
  std::vector<Point2f>& images, std::vector<Point3d>& grounds) const {
  std::vector<Point2d> imgGrid;
  CreateImageGrid(sensorModel, imgGrid);

  const auto [minHgt, maxHgt] = sensorModel->GetHeightRange();
  const auto stepHgt = double(maxHgt - minHgt) / (mNumLayers - 1) - DBL_EPSILON;
  std::vector<double> gridHgts;
  FillNodesByStride<double>(minHgt, maxHgt, stepHgt, gridHgts);

  for (auto img : imgGrid) {
    for (auto hgt : gridHgts) {
      Point3d grd{ 0, 0, hgt };
      sensorModel->ImageToGround(img, grd);

      images.emplace_back(img);
      grounds.emplace_back(grd);
    }
  }  
  return 0;
}

int SensorCube::CreateCubeFlatQC(const ISensorModel* sensorModel,
  std::vector<Point2f>& images, std::vector<Point3d>& grounds) const {
  return 0;
}

int SensorCube::CreateCubeModelQC(const ISensorModel* sensorModel,
  std::vector<Point2f>& images, std::vector<Point3d>& grounds) const {
  const auto [numCols, numRows] = sensorModel->GetImageSize();

  std::vector<Point2d> imgGrid;
  const auto [stepCols, stepRows] = CreateImageGrid(sensorModel, imgGrid);

  // three QC locations
  const std::array<Point2d, 3> qcLoc{ Point2d{stepCols/2.0, 0.0}, 
    Point2d{0.0, stepRows/2.0}, Point2d{stepCols/2.0, stepRows/2.0} };

  const auto [minHgt, maxHgt] = sensorModel->GetHeightRange();
  const auto stepHgt = double(maxHgt - minHgt) / (mNumLayers*2 - 1) - DBL_EPSILON;
  std::vector<double> gridHgts;
  FillNodesByStride<double>(minHgt, maxHgt, stepHgt, gridHgts);

  for (auto img0 : imgGrid) {
    for (auto loc : qcLoc) {
      Point2d img{img0 + loc};
      if (numCols + mMarginCols < img.x || numRows + mMarginRows < img.y)
        continue;

      for (auto hgt : gridHgts) {
        Point3d grd{ 0, 0, hgt };
        sensorModel->ImageToGround(img, grd);

        images.emplace_back(img);
        grounds.emplace_back(grd);
      }
    }
  }
  return 0;
}

/**
 *  \brief equally spaced grid in image
 */
std::pair<int, int> SensorCube::CreateImageGrid(
  const ISensorModel* sensorModel, std::vector<Point2d>& images) const {
  const auto [numCols, numRows] = sensorModel->GetImageSize();
  const auto stepCols = (numCols + mMarginCols * 2) / (mNumHoriz - 1);
  const auto stepRows = (numRows + mMarginRows * 2) / (mNumVert - 1);

  std::vector<double> gridCols, gridRows;
  FillNodesByStride<double>(-mMarginCols, numCols + mMarginCols, stepCols, gridCols);
  FillNodesByStride<double>(-mMarginRows, numRows + mMarginRows, stepRows, gridRows);

  for (auto col : gridCols) {
    for (auto row : gridRows) {
      images.emplace_back(col, row);
    }
  }

  return { stepCols, stepRows };
}
