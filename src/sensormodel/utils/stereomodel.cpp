/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/stereomodel.h"
#include "sensormodel/isensormodel.h"

#include <algorithm>
#include <cassert>
#include <omp.h>

using watxtract::ISensorModel;
using watxtract::Point2d;
using watxtract::Point3d;
using watxtract::StereoModel;

/**
 *  Constructor.
 */
StereoModel::StereoModel() {}

/**
 *  Image-to-ground forward intersection.
 *
 *  \brief Given a pair of sensor models and associated 2d positions
 *  in image coordinate system, do forward intersection of pair-rays, and
 *  output the ground position with respect to the ground coordinate system.
 *
 *  \param [in] leftModel left sensor model composing the stereo model
 *  \param [in] leftImage 2d position (x, y) in left image raster coordinate
 * system 
 *  \param [in] rightModel right sensor model composing the stereo model
 *  \param [in] rightImage the corresponding 2d position (x, y) in right image
 * raster coordinate system 
 *  \param [in/out] ground 3d ground position (X, Y, Z)
 * in ground coordinate system 
 *  \return Returns true if the operation succeeds
 *
 *  \details
 */
bool StereoModel::ImageToGround(ISensorModel const *const leftModel,
                                const Point2d &leftImage,
                                ISensorModel const *const rightModel,
                                const Point2d &rightImage,
                                Point3d &ground) const {
  const double interval{100.0};
  const double leftMeanHgt{};  // leftModel->GetMeanHeight()
  const double rightMeanHgt{}; // rightModel->GetMeanHeight()
  const double meanHgt = (leftMeanHgt + rightMeanHgt) / 2.0;
  const double upperHgt{meanHgt + interval};
  const double lowerHgt{meanHgt - interval};

  // forward projection
  Point3d leftGrd[2]{{0.0, 0.0, upperHgt}, {0.0, 0.0, lowerHgt}};
  Point3d rightGrd[2]{{0.0, 0.0, upperHgt}, {0.0, 0.0, lowerHgt}};

  for (int i = 0; i < 2; ++i) {
    leftModel->ImageToGround(leftImage, leftGrd[i]);
    rightModel->ImageToGround(rightImage, rightGrd[i]);
  }

  // 3D intersection

  return true;
}

/**
 *  \brief Given a pair of sensor models and associated list of 2d positions
 *  in image coordinate system, do forward intersection of pair-rays, and
 *  output the list of ground positions with respect to the ground coordinate
 * system.
 *
 *  \param [in] leftModel left sensor model composing the stereo model
 *  \param [in] leftImages a list of 2d positions (x, y) in left sensor raster
 * coordinate system 
 *  \param [in] rightModel right sensor model composing the stereo model 
 *  \param [in] rightImages a list of corresponding 2d position (x,
 * y) in right sensor raster coordinate system 
 *  \param [in/out] grounds a list of 3d ground positions (X, Y, Z) in ground coordinate system
 *
 *  \details
 */
size_t StereoModel::ImageToGround(ISensorModel const *const leftModel,
                                  const std::vector<Point2d> &leftImages,
                                  ISensorModel const *const rightModel,
                                  const std::vector<Point2d> &rightImages,
                                  std::vector<Point3d> &grounds) const {
  const auto size = static_cast<long>(leftImages.size());
  assert(size == rightImages.size());

  grounds.resize(size);
  size_t ret{};

  #pragma omp parallel for reduction(+ : ret)
  for (long i = 0; i < size; ++i) {
    if (ImageToGround(leftModel, leftImages[i], rightModel, rightImages[i],
                      grounds[i])) {
      ++ret;
    }
  }
  return ret;
}

/**
 *  \brief Given a triplet of sensor models and associated 2d positions
 *  in image coordinate system, do forward intersection of triple-rays, and
 *  output the ground position with respect to the ground coordinate system.
 *
 *  \param [in] sensorModels triplet sensor models composing the stereo model
 *  \param [in] image the corresponding 2d positions (x, y) in imagery raster
 * coordinate systems 
 *  \param [in/out] ground 3d ground position (X, Y, Z) in
 * ground coordinate system 
 *  \return Returns true if the operation succeeds
 *
 *  \details
 */
bool StereoModel::ImageToGround(
    const std::array<ISensorModel const *, 3> &sensorModels,
    const std::array<Point2d, 3> &image, Point3d &ground) const {
  return true;
}

/**
 *  \brief Given a triplet of sensor models and associated list of triplet 2d
 * positions in image coordinate system, do forward intersection of triple-rays,
 * and output the ground position with respect to the ground coordinate system.
 *
 *  \param [in] sensorModels triplet sensor models composing the stereo model
 *  \param [in] images the corresponding 2d positions (x, y) in imagery raster
 * coordinate systems, interleaved by sensor models. 
 *  \param [in/out] grounds 3d ground position (X, Y, Z) in ground coordinate system
 *
 *  \details
 */
size_t StereoModel::ImageToGround(
    const std::array<ISensorModel const *, 3> &sensorModels,
    const std::vector<Point2d> &images, std::vector<Point3d> &grounds) const {
  const auto size = static_cast<long>(images.size()) / 3;

  grounds.resize(size);
  size_t ret{};

  #pragma omp parallel for reduction(+ : ret)
  for (long i = 0; i < size; ++i) {
    const long j = i * 3;
    std::array<Point2d, 3> image{images[j], images[j + 1], images[j + 2]};

    if (ImageToGround(sensorModels, image, grounds[i])) {
      ++ret;
    }
  }
  return ret;
}

/**
 *  \brief Given two or more sensor models and associated 2d positions
 *  in image coordinate system, do forward intersection of rays, and
 *  output the ground position with respect to the ground coordinate system.
 *
 *  \param [in] sensorModels multiple (two or more) sensor models composing the
 * stereo model 
 *  \param [in] image the corresponding 2d positions (x, y) in
 * imagery raster coordinate systems, interleaved by sensor models. 
 *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate system
 *  \return Returns true if the operation succeeds
 *
 *  \details
 */
bool StereoModel::ImageToGround(
    const std::vector<ISensorModel const *> &sensorModels,
    const std::vector<Point2d> &image, Point3d &ground) const {
  // const size_t numModels = sensorModels.size();
  // assert(numModels == image.size());

  // forward projection

  // 3D intersection

  return true;
}

/**
 *  \brief Given two or more sensor models and associated list of 2d positions
 *  in image coordinate system, do forward intersection of rays, and
 *  output the ground positions with respect to the ground coordinate system.
 *
 *  \param [in] sensorModels multiple (two or more) sensor models composing the
 * stereo model 
 *  \param [in] images the corresponding 2d positions (x, y) in
 * imagery raster coordinate systems, interleaved by sensor models. 
 *  \param [in/out] grounds List of 3d ground positions (X, Y, Z) in ground coordinate
 * system
 *
 *  \details
 */
size_t StereoModel::ImageToGround(
    const std::vector<ISensorModel const *> &sensorModels,
    const std::vector<Point2d> &images, std::vector<Point3d> &grounds) const {
  const size_t numModels = sensorModels.size();
  const long size = images.size() / numModels;

  auto iter{ std::cbegin(images) };
  size_t ret{};

  #pragma omp parallel for reduction(+ : ret)
  for (long i = 0; i < size; ++i)
  {
      const long j = i * numModels;
      auto iterJ = std::next(iter, j);

      std::vector<Point2d> image(iterJ, std::next(iterJ, numModels));
      if (ImageToGround(sensorModels, image, grounds[i])) {
          ++ret;
      }
  }
  return ret;
}

/**
 *  Ground-to-image backward projection.
 *
 *  \brief Given a 3d ground position with reference to the ground coordinate
 * system and multiple sensor models, output the corresponding 2d sensor
 * positions in the image coordinate system for each sensor model.
 *
 *  \param [in] ground 3d position (X, Y, Z) in ground coordinate system
 *  \param [in] sensorModels multiple (two or more) sensor models composing the
 * stereo model 
 *  \param [out] image 2d position (x, y) in imagery raster coordinate system 
 *  \return Returns true if operation succeeds
 *
 *  \details
 */
bool StereoModel::GroundToImage(
    const Point3d &ground,
    const std::vector<ISensorModel const *> &sensorModels,
    std::vector<Point2d> &image) const {
  const long size = sensorModels.size();
  image.resize(size);

  bool ret{true};
  for (long i = 0; i < size; ++i)
  {
      auto g2s = sensorModels[i]->GroundToImage(ground, image[i]);
      ret = ret && g2s;
  }
  return ret;
}

/**
 *  \brief Given a list of 3d positions in ground coordinate system and
 *  multiple sensor models, output a list of 2d sensor positions in image
 * coordinate system.
 *
 *  \param [in] grounds List of 3d ground positions (X, Y, Z)
 *  \param [in] sensorModels multiple (two or more) sensor models composing the
 * stereo model \param [in/out] images List of 2d positions (x, y) on image,
 * interleaved by sensor models
 *
 *  \details
 */
size_t StereoModel::GroundToImage(
    const std::vector<Point3d> &grounds,
    const std::vector<ISensorModel const *> &sensorModels,
    std::vector<Point2d> &images) const {
  const long size = grounds.size();
  const auto numModels{sensorModels.size()};
  images.resize(size * numModels);

  auto iter = std::begin(images);
  size_t ret{};

  #pragma omp parallel for reduction(+ : ret)
  for (long i = 0; i < size; ++i)
  {
      std::vector<Point2d> image(numModels);
      if (GroundToImage(grounds[i], sensorModels, image))
      {
          std::copy_n(std::begin(image), numModels, std::next(iter,
  numModels*i));
          ++ret;
      }
  }
  return ret;
}
