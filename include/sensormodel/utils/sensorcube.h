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

  SensorCube(int numHoriz, int numVert,
    int numLayers, LayerType layerType);
  ~SensorCube() = default;

  void SetLayout(const int numHoriz, const int numVert, 
    const int numLayers, const LayerType layerType) {
    mNumHoriz = numHoriz;
    mNumVert = numVert;
    mNumLayers = numLayers;
    mLayerType = layerType;
    Initialize();
  }

  /**
   *  \brief Set the margin size and kind.
   *
   *  \param [in] marginCols the horizontal margin size
   *  \param [in] marginRows the vertical margin size
   */
  void SetMargins(const int marginCols, const int marginRows) {
    mMarginCols = marginCols;
    mMarginRows = marginRows;
  }

  /**
 *  \return Returns margin size.
 */
  std::pair<int, int> GetMargins() const noexcept {
    return { mMarginCols, mMarginRows };
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

  int mNumHoriz{10};  // number of points in column of image grid 
  int mNumVert{10};   // number of points in row of image grid 
  int mNumLayers{10}; // number of layers in height of ground cube 
  LayerType mLayerType{ eMODEL };

  int mMarginCols{};
  int mMarginRows{};

  void Initialize();

  // cached 
};

using CubeType = SensorCube::LayerType;

using UPSensorCube = std::unique_ptr<SensorCube>;
using SPSensorCube = std::shared_ptr<SensorCube>;
}
