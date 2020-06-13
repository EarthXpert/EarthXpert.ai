/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/stereomodel.h"
#include "sensormodel/isensormodel.h"
#include "core/utils/unitutils.h"

#include <algorithm>
#include <cassert>
#include <omp.h>

using earthxpert::ISensorModel;
using earthxpert::Point2d;
using earthxpert::Point3d;
using earthxpert::StereoModel;

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
bool StereoModel::ImageToGround(ISensorModel const* const leftModel,
  const Point2d& leftImage,
  ISensorModel const* const rightModel,
  const Point2d& rightImage,
  Point3d& ground) const {
  if (!mEarthEllip) {
    return ImageToGroundAprox(leftModel, leftImage, rightModel, rightImage, ground);
  }
  assert(eGEO == leftModel->GetSrsType());

  const double meanHgt = (leftModel->GetMeanHeight() + rightModel->GetMeanHeight()) / 2.0;
  const double hgtRange[2] = { meanHgt + mH, meanHgt - mH };

  Point3d leftLsr[2];
  Point3d rightLsr[2];

  for (int i = 0; i < 2; ++i) {
    Point3d grd{ 0.0, 0.0, hgtRange[i] };

    leftModel->ImageToGround(leftImage, grd);
    leftLsr[i] = mEarthEllip->LPHToLSR(Deg2Rad(grd));
    
    rightModel->ImageToGround(rightImage, grd);
    rightLsr[i] = mEarthEllip->LPHToLSR(Deg2Rad(grd));
  }

  // 3d intersection
  Point3d lsr;


  ground = Rad2Deg(mEarthEllip->LSRToLPH(lsr));

  return true;
}

// Newton-Raphson
bool StereoModel::ImageToGroundAprox(ISensorModel const *const leftModel,
                                const Point2d &leftImage,
                                ISensorModel const *const rightModel,
                                const Point2d &rightImage,
                                Point3d &ground) const {
  const double meanHgt = (leftModel->GetMeanHeight() + rightModel->GetMeanHeight()) / 2.0;
  ground.z = meanHgt;

  int i;
  for (i = 0; i < mMaxIters; ++i) {
    Point3d grd{ 0.0, 0.0, ground.z + mH };

    Point3d leftGrd{ 0.0, 0.0, ground.z };
    leftModel->ImageToGround(leftImage, grd);
    leftModel->ImageToGround(leftImage, leftGrd);
    const Point2d dLeft_dz{(grd.x - leftGrd.x) * mInvH, (grd.y - leftGrd.y) * mInvH};

    Point3d rightGrd{ 0.0, 0.0, ground.z };
    rightModel->ImageToGround(rightImage, grd);
    rightModel->ImageToGround(rightImage, rightGrd);
    const Point2d dRight_dz{ (grd.x - rightGrd.x) * mInvH, (grd.y - rightGrd.y) * mInvH };

    const Point2d A = dRight_dz - dLeft_dz;
    const double den = cv::normL2Sqr<double>(A);
    if (std::numeric_limits<double>::epsilon() > den) {
      return false;
    }

    ground.x = (leftGrd.x + rightGrd.x) * 0.5;
    ground.y = (leftGrd.y + rightGrd.y) * 0.5;

    const double dz = A.ddot(Point2d(leftGrd.x - rightGrd.x, leftGrd.y - rightGrd.y)) / den;
    ground.z += dz;

    if (mTolZ > fabs(dz)) break;
  }

  if (mMaxIters == i) {
    return false;
  }

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
  for (long i = 0; i < size; ++i) {
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
 * stereo model 
 *  \param [in/out] images List of 2d positions (x, y) on image,
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
  for (long i = 0; i < size; ++i) {
      std::vector<Point2d> image(numModels);
      if (GroundToImage(grounds[i], sensorModels, image)) {
          std::copy_n(std::begin(image), numModels, std::next(iter,
  numModels*i));
          ++ret;
      }
  }
  return ret;
}
