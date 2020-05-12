/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "epipolarmodel/frameepipolarmodel.h"
#include "epipolarmodel/iepipolarmodel.h"
#include "sensormodel/frameep.h"
#include "sensormodel/framemodel.h"
#include "sensormodel/isensormodel.h"

#include <array>
#include <cassert>
#include <cmath>

using watxtract::FrameEpipolarModel;
using watxtract::IEpipolarModel;
using watxtract::ISensorModel;
using watxtract::Point2d;
using watxtract::Point3d;

/**
 *   Constructor
 *   \brief construct an epipolar frame model
 */
FrameEpipolarModel::FrameEpipolarModel(std::shared_ptr<ISensorModel> leftModel,
                                       std::shared_ptr<ISensorModel> rightModel)
    : mLeftModel(leftModel), mRightModel(rightModel) {
}

/**
 *  Epipolar-to-image point transform.
 *
 *  \brief Given a 2d poistion on epipolar, output the corresponding position on
 * original image.
 *
 *  \param [in] epi 2d position (u, v) in epipolar image pixel coordinate system
 *  \param [in] img 2d position (x, y) in image raster coordinate system
 *  \param [in] pos Component position of stereo model
 *  \return Returns true if operation succeeds
 *
 *  \details 
 */
bool FrameEpipolarModel::EpipolarToImage(const Point2d &epi, Point2d &img,
                                         const StereoPos pos) const {
  return true;
}

/**
 *   Epipolar-to-image image transformation
 *
 *  \brief Given a list of 2d poistions on epipolar, output the corresponding
 * positions on original image.
 *
 *  \param [in] epi List of 2d positions (x, y) in epipolar image pixel coordinate system 
 *  \param [in] img List of 2d positions (x, y) in image raster coordinate system 
 *  \param [in] pos Component position of stereo model \return
 *  \return Returns true if operation succeeds
 */
size_t FrameEpipolarModel::EpipolarToImage(const std::vector<Point2d> &epi,
                                           std::vector<Point2d> &img,
                                           const StereoPos pos) const {
  assert(epi.size() == img.size());
  size_t ret{};

  const long size = epi.size();

  //#pragma omp parallel for reduction(+ : ret)
  for (long i = 0; i < size; ++i) {
    if (EpipolarToImage(epi[i], img[i], pos)) {
      ++ret;
    }
  }
  return ret;
}

/**
 *  Image-to-epipolar point transform.
 *
 *  \brief Given a 2d poistion on image, output the corresponding position on
 * epipolar.
 *
 *  \param [in] img 2d position (u, v) in image raster coordinate system
 *  \param [in] epi 2d position (x, y) in epipolar image pixel coordinate system
 *  \param [in] pos Component position of stereo model
 *  \return Returns true if operation succeeds
 *
 *   \details 
 */
bool FrameEpipolarModel::ImageToEpipolar(const Point2d &img, Point2d &epi,
                                         const StereoPos pos) const {
  return true;
}

/**
 *   Image-to-epipolar  image transformation
 *  \brief Given a list of 2d poistions on image, output the corresponding
 * positions on epipolar.
 *
 *  \param [in] img List of 2d positions (x, y) in image raster coordinate
 * system \param [in] epi List of 2d positions (x, y) in epipolar raster
 * coordinate system \param [in] pos Component position of stereo model \return
 * Returns true if operation succeeds
 */
size_t FrameEpipolarModel::ImageToEpipolar(const std::vector<Point2d> &img,
                                           std::vector<Point2d> &epi,
                                           const StereoPos pos) const {
  assert(img.size() == epi.size());
  size_t ret{};

  const long size = img.size();

  //#pragma omp parallel for reduction(+ : ret)
  for (long i = 0; i < size; ++i) {
    if (ImageToEpipolar(img[i], epi[i], pos)) {
      ++ret;
    }
  }
  return ret;
}
