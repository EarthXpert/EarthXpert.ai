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
 *  \brief the class SensorCube creates a 3d cube of image positions and
 *  corresponding ground positions at multiple elevation layers.
 */
class EXSENSORMODEL_EXPORTS SensorCube {
public:
  // option of layers in ground
  enum LayerType {
    eFLAT,   // vertical layers that are flat planar in ground
    eMODEL   // vertical layers in model SRS, that are spherical for geographic SRS, 
             // flat for local SRS, pseudo-flat for map projection
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

private:
  static constexpr double kMarginRatio = 0.1;  // ratio of margin to extend beyound the scene

  std::array<double, NumFact> mNormalFators{}; // normalization factors including offsets & scales
  int mNumHoriz{};
  int mNumVert{};
  int mNumLayers{};
  LayerType mLayerType{ eMODEL };

  void Initialize();
};

using CubeType = SensorCube::LayerType;
}
