/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "epipolarmodel/frameepipolarmodel.h"
#include "core/exceptions/exexcept.h"
#include "sensormodel/frameep.h"
#include "sensormodel/framemodel.h"

#include <array>
#include <cassert>
#include <cmath>

using namespace earthxpert;

/**
 *   \brief construct a frame epipolar model
 */
template <class ImageModelPolicy>
FrameEpipolarModel<ImageModelPolicy>::FrameEpipolarModel(StereoPos pos, 
  std::shared_ptr<FrameModel<ImageModelPolicy>> sensorModel, const Polygon2d& epiBound)
  : EpipolarSensorModel(pos, sensorModel, epiBound), 
    mFrameModel(sensorModel.get()) {
  Initialize();
}

template <class ImageModelPolicy>
void FrameEpipolarModel<ImageModelPolicy>::Initialize() {
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
template <class ImageModelPolicy>
bool FrameEpipolarModel<ImageModelPolicy>::EpiToRaw(const Point2d &epi, Point2d &raw) const {
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
template <class ImageModelPolicy>
bool FrameEpipolarModel<ImageModelPolicy>::RawToEpi(const Point2d &raw, Point2d &epi) const {
  return true;
}

template class FrameEpipolarModel<DigitalImageModel>; 
template class FrameEpipolarModel<FilmImageModel>;
