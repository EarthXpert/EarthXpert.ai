/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/sensormodel.h"

#include <algorithm>
#include <cassert>
//#include <execution>
#include <omp.h>

using earthxpert::ISensorModel;
using earthxpert::Point2d;
using earthxpert::Point3d;
using earthxpert::SensorModel;

/**
 *  Image-to-ground forward projection.
 *
 *  \brief Given a 2d position in image coordinate system and a height
 *  in the ground vertical coordinate system, output the ground horizontal
 *  position with respect to the ground coordinate system.
 *
 *  \param [in] image 2d position (x, y) in imagery raster coordinate system
 *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate
 * system \return Returns true if the operation succeeds
 *
 *  \details
 */
bool SensorModel::ImageToGround(const Point2d &image, Point3d &ground) const {
  return false;
}

/**
 *  \brief Given a 2d position in image coordinate system and a list of
 *  heights, output ground horizontal positions in ground coordinate system.
 *
 *  \param [in] image 2d position (x, y) in imagery raster coordinate system
 *  \param [in/out] ground 3d ground positions (X, Y, Z) in ground coordinate
 * system
 */
size_t SensorModel::ImageToGround(const Point2d &image,
                                  std::vector<Point3d> &ground) const {
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
 *
 *  \details More details
 */
size_t SensorModel::ImageToGround(const std::vector<Point2d> &image,
                                  std::vector<Point3d> &ground) const {
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
 *  Ground-to-image backward projection.
 *
 *  \brief Given a 3d ground position with reference to the ground coordinate
 * system, output the 2d image position in the image coordinate system.
 *
 *  \param [in] ground 3d position (X, Y, Z) in ground coordinate system
 *  \param [out] image 2d position (x, y) in imagery raster coordinate system
 *  \return Returns true if operation succeeds
 *
 *  \details
 */
bool SensorModel::GroundToImage(const Point3d &ground, Point2d &image) const {
  return false;
}

/**
 *  \brief Given a list of 3d positions in ground coordinate system,
 *  output a list of 2d sensor positions in image coordinate system.
 *
 *  \param [in] ground List of 3d ground positions (X, Y, Z)
 *  \param [in/out] image List of 2d positions (x, y) on image
 */
size_t SensorModel::GroundToImage(const std::vector<Point3d> &ground,
                                  std::vector<Point2d> &image) const {
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
