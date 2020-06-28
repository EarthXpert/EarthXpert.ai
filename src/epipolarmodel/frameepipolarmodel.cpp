/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "epipolarmodel/frameepipolarmodel.h"
#include "sensormodel/frameep.h"
#include "sensormodel/framemodel.h"

#include <array>
#include <cassert>
#include <cmath>

using earthxpert::FrameEpipolarModel;
using earthxpert::IEpipolarModel;
using earthxpert::ISensorModel;
using earthxpert::Point2d;
using earthxpert::Point3d;

/**
 *   \brief construct a frame epipolar model
 */
FrameEpipolarModel::FrameEpipolarModel(std::shared_ptr<ISensorModel> sensorModel,
  StereoPos pos) : EpipolarSensorModel(sensorModel, pos) {
}

/**
 *  Epipolar-to-raw point transform.
 *
 *  \brief Given a 2d poistion on epipolar, output the corresponding position on
 * original image.
 *
 *  \param [in] epi 2d position (u, v) in epipolar image pixel coordinate system
 *  \param [in] raw 2d position (x, y) in image raster coordinate system
 *  \return Returns true if operation succeeds
 */
bool FrameEpipolarModel::EpiToRaw(const Point2d &epi, Point2d &raw) const {
  return true;
}

/**
 *  Raw-to-epipolar point transform.
 *
 *  \brief Given a 2d poistion on image, output the corresponding position on
 * epipolar.
 *
 *  \param [in] raw 2d position (u, v) in image raster coordinate system
 *  \param [in] epi 2d position (x, y) in epipolar image pixel coordinate system
 *  \return Returns true if operation succeeds
 */
bool FrameEpipolarModel::RawToEpi(const Point2d &raw, Point2d &epi) const {
  return true;
}
