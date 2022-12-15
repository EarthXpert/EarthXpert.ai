/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "utils/image2dem.h"

using namespace earthxpert;

/**
 *  \brief Construct rasterizer object
 */
Image2Dem::Image2Dem(std::shared_ptr<ISensorModel> sensorModel, std::shared_ptr<IDem> dem) 
  : mSensorModel(sensorModel), mDem(dem) {
  Initialize();
}

/**
 *  \brief
 */
void Image2Dem::Initialize() {
}

/**
 *  \brief
 */
void Image2Dem::Project(const Point2d& image, Point3d& ground) {
  ground.z = mSensorModel->GetMeanHeight();

  for (int i = 0; i < kMaxIterI2D; ++i) {
    mSensorModel->ImageToGround(image, ground);

    // \todo convert ground from model CRS to DEM CRS
    float z = mDem->GetElev(ground.x, ground.y);

    // \todo convert elevation vertical datum from DEM to model
    if (fabs(ground.z - z) < kTolZ) {
      break;
    }
    ground.z = z;
  }
}
