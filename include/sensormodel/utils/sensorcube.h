/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/exsensormodel.h"
#include "sensormodel/datatypes.h"
#include "3rdparty/opencv/exopencv.h"

#include <array>
#include <memory>
#include <vector>

namespace earthxpert {
class ISensorModel;

/**
 *  \brief the class SensorCube creates a 2d grid of image positions and
 *  corresponding 3d cube of ground positions at multiple elevation layers.
 */
class EXSENSORMODEL_EXPORTS SensorCube {
public:
  // option of layers in ground
  enum LayerType {
    eFLAT,   // rectilinear: vertical layers that are flat planar in ground
    eMODEL   // curvilinear: vertical layers in model SRS, that are spherical 
    // for geographic SRS, flat for local SRS, pseudo-flat for map projection
  };

  SensorCube(const std::array<double, NumFact>& normalFators,
    int numHoriz, int numVert, int numLayers, LayerType layerType);
  ~SensorCube() = default;

  void SetNormalFators(const std::array<double, NumFact>& normalFators) {
    mNormalFators = normalFators;
    Initialize();
  }

  void SetLayout(const int numHoriz, const int numVert, 
    const int numLayers, const LayerType layerType) {
    mNumHoriz = numHoriz;
    mNumVert = numVert;
    mNumLayers = numLayers;
    mLayerType = layerType;
    Initialize();
  }

  int CreateCube(const ISensorModel* sensorModel,
    std::vector<Point2f>& images, std::vector<Point3d>& grounds) const;

  int CreateCubeQC(const ISensorModel* sensorModel,
    std::vector<Point2f>& images, std::vector<Point3d>& grounds) const;

  int CreateCubeFlat(const ISensorModel* sensorModel,
    std::vector<Point2f>& images, std::vector<Point3d>& grounds) const;
  int CreateCubeModel(const ISensorModel* sensorModel,
    std::vector<Point2f>& images, std::vector<Point3d>& grounds) const;

private:
  static constexpr double kMarginRatio = 0.1;  // ratio of margin to extend beyound the image scene

  std::array<double, NumFact> mNormalFators{}; // normalization factors including offsets & scales
  int mNumHoriz{10};
  int mNumVert{10};
  int mNumLayers{10};
  LayerType mLayerType{ eMODEL };

  void Initialize();

  // cached 
};

using CubeType = SensorCube::LayerType;

using UPSensorCube = std::unique_ptr<SensorCube>;
using SPSensorCube = std::shared_ptr<SensorCube>;
}
