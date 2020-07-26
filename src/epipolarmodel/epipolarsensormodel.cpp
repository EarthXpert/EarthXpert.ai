/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "epipolarmodel/epipolarsensormodel.h"

#include <array>
#include <cassert>
#include <cmath>

using earthxpert::EpipolarSensorModel;
using earthxpert::IEpipolarModel;
using earthxpert::ISensorModel;
using earthxpert::Point2d;
using earthxpert::Point3d;

/**
 *   \brief construct a frame epipolar model
 */
EpipolarSensorModel::EpipolarSensorModel(std::shared_ptr<ISensorModel> sensorModel,
  StereoPos pos, const Polygon2d& epiBound) 
  : mSensorModel(sensorModel), mPos(pos), mBound(epiBound) {
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
bool EpipolarSensorModel::EpiToRaw(const Point2d& epi, Point2d& raw) const {
  return false;
}

/**
 *   Epipolar-to-raw image transformation
 *
 *  \brief Given a list of 2d poistions on epipolar, output the corresponding
 * positions on original image.
 *
 *  \param [in] epi List of 2d positions (x, y) in epipolar image pixel coordinate system
 *  \param [in] raw List of 2d positions (x, y) in image raster coordinate system
 *  \return Returns true if operation succeeds
 */
size_t EpipolarSensorModel::EpiToRaw(const std::vector<Point2d>& epi, std::vector<Point2d>& raw) const {
  const long size = epi.size();
  assert(size == raw.size());
  size_t ret{};

#pragma omp parallel for reduction(+ : ret)
  for (long i = 0; i < size; ++i) {
    if (EpiToRaw(epi[i], raw[i])) {
      ++ret;
    }
  }
  return ret;
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
bool EpipolarSensorModel::RawToEpi(const Point2d& raw, Point2d& epi) const {
  return false;
}

/**
 *   Raw-to-epipolar  image transformation
 *
 *  \brief Given a list of 2d poistions on image, output the corresponding
 * positions on epipolar.
 *
 *  \param [in] raw List of 2d positions (x, y) in image raster coordinate
 * system
 *  \param [in] epi List of 2d positions (x, y) in epipolar raster
 * coordinate system
 *   \return Returns true if operation succeeds
 */
size_t EpipolarSensorModel::RawToEpi(const std::vector<Point2d>& raw, std::vector<Point2d>& epi) const {
  const long size = raw.size();
  assert(size == epi.size());
  size_t ret{};

#pragma omp parallel for reduction(+ : ret)
  for (long i = 0; i < size; ++i) {
    if (RawToEpi(raw[i], epi[i])) {
      ++ret;
    }
  }
  return ret;
}

/**
 *  \brief Given a 2d position in image coordinate system and a list of
 *  heights, output ground horizontal positions in ground coordinate system.
 *
 *  \param [in] image 2d position (x, y) in imagery raster coordinate system
 *  \param [in/out] ground 3d ground positions (X, Y, Z) in ground coordinate
 * system
 */
size_t EpipolarSensorModel::ImageToGround(const Point2d& image,
  std::vector<Point3d>& ground) const {
  size_t ret{};

#pragma omp parallel for reduction(+ : ret)
  for (long i = 0; i < ground.size(); ++i) {
    if (ImageToGround(image, ground[i])) {
      ++ret;
    }
  }
  return ret;
}

/**
 *  \brief Given a list of 2d positions in image coordinate system and a
 * corresponding list of heights, output the ground horizontal positions in
 * ground coordinate system.
 *
 *  \param [in] image List of 2d positions (x, y) on image
 *  \param [in/out] ground List of 3d ground positions (X, Y, Z)
 */
size_t EpipolarSensorModel::ImageToGround(const std::vector<Point2d>& image,
  std::vector<Point3d>& ground) const {
  const long size = static_cast<long>(image.size());
  assert(size == ground.size());
  size_t ret{};

#pragma omp parallel for reduction(+ : ret)
  for (long i = 0; i < size; ++i) {
    if (ImageToGround(image[i], ground[i])) {
      ++ret;
    }
  }
  return ret;
}

/**
 *  \brief Given a list of 3d positions in ground coordinate system,
 *  output a list of 2d sensor positions in image coordinate system.
 *
 *  \param [in] ground List of 3d ground positions (X, Y, Z)
 *  \param [in/out] image List of 2d positions (x, y) on image
 */
size_t EpipolarSensorModel::GroundToImage(const std::vector<Point3d>& ground,
  std::vector<Point2d>& image) const {
  const long size = static_cast<long>(ground.size());
  assert(size == image.size());
  size_t ret{};

#pragma omp parallel for reduction(+ : ret)
  for (long i = 0; i < size; ++i) {
    if (GroundToImage(ground[i], image[i])) {
      ++ret;
    }
  }
  return ret;
}
