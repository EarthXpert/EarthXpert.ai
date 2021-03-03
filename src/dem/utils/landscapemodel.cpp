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

/**
 *  \brief find the intersection point between the LOS and the DEM surface.
 * 
 *  \details determine the LOS segment in ground from the elevation range;
 *  transform the two ends of LOS segment from model SRS to DEM SRS; 
 *  project the LOS segment from 3D to 2D and divide the Brehensen line 
 *  using half DEM resolution; read DEM elevations at vertices to form 
 *  a 2D profile; intersect between the LOS and the profile in Cartesian.
 *  
 *  \optimize determine the search direction by comparing the average 
 *  elevation the and read elevation; 1) read one vertex and do the 
 *  intersectiontest until successful; 2) overlap the read of next vertice
 *  and the intersection test between the LOS and the profile segment.
 */
I2DStatus LandscapeModel::ImageToDem(const Point2d& image, Point3d& dem) {
  return eI2D_GOOD;
}

I2DStatus LandscapeModel::DemToImage(const Point3d& dem, Point2d& image) {
  Point3d ground;
  mCtD2I->Transform(dem, ground);
  auto ret = mModel->GroundToImage(ground, image);

  return ret ? eI2D_GOOD : eI2D_FAIL;
}
