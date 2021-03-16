/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "dem/idem.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/spatialref/ict.h"
#include "sensormodel/isensormodel.h"

namespace earthxpert {
/**
 *  \brief the class LandscapeModel represents the landscape
 *  formed by an image and the DEM.
 */
class EXDEM_EXPORTS LandscapeModel {
public:
  enum I2DStatus {
    eI2D_GOOD,    // succeed
    eI2D_BKGD,    // background
    eI2D_OFFB,    // out of DEM boundary
    eI2D_FAIL
  };

  LandscapeModel(ISensorModel const* model, IDem const* dem);
  ~LandscapeModel() = default;

  I2DStatus ImageToDem(const Point2d& image, Point3d& dem);
  inline I2DStatus operator()(const Point2d& image, Point3d& dem) {
    return ImageToDem(image, dem);
  }

  I2DStatus DemToImage(const Point3d& dem, Point2d& image);
  inline I2DStatus operator()(const Point3d& dem, Point2d& image) {
    return DemToImage(dem, image);
  }

private:
  const ISensorModel* mModel{};
  const IDem* mDem{};

  void Initialize();

  std::shared_ptr<ICt> mCtI2D;
  std::shared_ptr<ICt> mCtD2I;
};

using I2DStatus = LandscapeModel::I2DStatus;

using UPLandscapeModel = std::unique_ptr<LandscapeModel>;
using SPLandscapeModel = std::shared_ptr<LandscapeModel>;
}
