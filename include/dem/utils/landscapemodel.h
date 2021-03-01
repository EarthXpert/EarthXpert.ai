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
  LandscapeModel(ISensorModel const* model, IDem const* dem);
  ~LandscapeModel() = default;

  bool ImageToDem(const Point2d& image, Point3d& dem);

  bool DemToImage(const Point3d& dem, Point2d& image);

private:
  const ISensorModel* mModel{};
  const IDem* mDem{};

  void Initialize();

  std::shared_ptr<ICt> mCtI2D;
  std::shared_ptr<ICt> mCtD2I;
};

using UPLandscapeModel = std::unique_ptr<LandscapeModel>;
using SPLandscapeModel = std::shared_ptr<LandscapeModel>;
}
