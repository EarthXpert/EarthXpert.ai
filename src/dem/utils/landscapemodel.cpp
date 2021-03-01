/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "dem/utils/landscapemodel.h"
#include "core/spatialref/ctfactory.h"

using namespace earthxpert;

LandscapeModel::LandscapeModel(ISensorModel const* const model, 
  IDem const* const dem) : mModel(model), mDem(dem) {
  Initialize();
}

void LandscapeModel::Initialize() {
  mCtI2D = CtFactory::CreateOgrCt(nullptr);
}

bool LandscapeModel::ImageToDem(const Point2d& image, Point3d& dem) {
  return true;
}

bool LandscapeModel::DemToImage(const Point3d& dem, Point2d& image) {
  Point3d ground;
  mCtD2I->Transform(dem, ground);
  return mModel->GroundToImage(ground, image);
}
