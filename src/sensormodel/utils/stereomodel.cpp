/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/stereomodel.h"
#include "sensormodel/isensormodel.h"
#include "core/math/intersect.h"
#include "core/utils/unitutils.h"

#include <algorithm>
#include <cassert>
#include <omp.h>

using earthxpert::ISensorModel;
using earthxpert::Point2d;
using earthxpert::Point3d;
using earthxpert::StereoModel;
using namespace earthxpert::math;

/**
 *  Image-to-ground forward intersection.
 *
 *  \brief Given a pair of sensor models and associated 2d positions
 *  in image coordinate system, do forward intersection of pair-rays, and
 *  output the ground position with respect to the ground coordinate system.
 *
 *  \param [in] leftImage 2d position (x, y) in left image raster coordinate
 * system 
 *  \param [in] rightImage the corresponding 2d position (x, y) in right image
 * raster coordinate system 
 *  \param [in/out] ground 3d ground position (X, Y, Z)
 * in ground coordinate system 
 *  \return Returns true if the operation succeeds
 *
 *  \todo extract X class
 */
bool StereoModel::ImageToGround(const Point2d& leftImage, 
  const Point2d& rightImage, Point3d& ground) const {
  assert(2 == mModels.size());
  if (!mEarthEllip) {
    return ImageToGroundNR(leftImage, rightImage, ground);
  }
  assert(eGEO == mModels[0]->GetSrsType());

  ground.z = (mModels[0]->GetMeanHeight() + mModels[1]->GetMeanHeight()) / 2.0;

  Point3d leftLsr[2];
  Point3d rightLsr[2];
  Point3d lsr;

  for (int i = 0; i < 2; ++i) {
    const double hgtRange[2] = { ground.z + mH, ground.z - mH };

    for (int j = 0; j < 2; ++j) {
      Point3d grd{ 0.0, 0.0, hgtRange[j] };

      mModels[0]->ImageToGround(leftImage, grd);
      leftLsr[j] = mEarthEllip->LPHToLSR(Deg2Rad(grd));

      mModels[1]->ImageToGround(rightImage, grd);
      rightLsr[j] = mEarthEllip->LPHToLSR(Deg2Rad(grd));
    }

    // 3d intersection X
    const bool ret = IntersectTwoLines(leftLsr, rightLsr, lsr);
    if (!ret) return false;

    ground = Rad2Deg(mEarthEllip->LSRToLPH(lsr));
  }

  return true;
}

// Newton-Raphson
bool StereoModel::ImageToGroundNR(const Point2d &leftImage,
  const Point2d &rightImage, Point3d &ground) const {
  assert(2 == mModels.size());
  ground.z = (mModels[0]->GetMeanHeight() + mModels[1]->GetMeanHeight()) / 2.0;

  int i;
  for (i = 0; i < mMaxIters; ++i) {
    Point3d grd{ 0.0, 0.0, ground.z + mH };

    Point3d leftGrd{ 0.0, 0.0, ground.z };
    mModels[0]->ImageToGround(leftImage, grd);
    mModels[0]->ImageToGround(leftImage, leftGrd);
    const Point2d dLeft_dz{(grd.x - leftGrd.x) * mInvH, (grd.y - leftGrd.y) * mInvH};

    Point3d rightGrd{ 0.0, 0.0, ground.z };
    mModels[1]->ImageToGround(rightImage, grd);
    mModels[1]->ImageToGround(rightImage, rightGrd);
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
 *  \param [in] leftImages a list of 2d positions (x, y) in left sensor raster
 * coordinate system 
 *  \param [in] rightImages a list of corresponding 2d position (x,
 * y) in right sensor raster coordinate system 
 *  \param [in/out] grounds a list of 3d ground positions (X, Y, Z) in ground coordinate system
 *
 *  \details
 */
size_t StereoModel::ImageToGround(const std::vector<Point2d> &leftImages,
  const std::vector<Point2d> &rightImages, std::vector<Point3d> &grounds) const {
  const auto size = static_cast<long>(leftImages.size());
  assert(size == rightImages.size());

  grounds.resize(size);
  size_t ret{};

  #pragma omp parallel for reduction(+ : ret)
  for (long i = 0; i < size; ++i) {
    if (ImageToGround(leftImages[i], rightImages[i], grounds[i])) {
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
 *  \param [in] image the corresponding 2d positions (x, y) in imagery raster
 * coordinate systems 
 *  \param [in/out] ground 3d ground position (X, Y, Z) in
 * ground coordinate system 
 *  \return Returns true if the operation succeeds
 *
 *  \details
 */
bool StereoModel::ImageToGround(const std::array<Point2d, 3> &image, Point3d &ground) const {
  return true;
}

/**
 *  \brief Given two or more sensor models and associated 2d positions
 *  in image coordinate system, do forward intersection of rays, and
 *  output the ground position with respect to the ground coordinate system.
 *
 *  \param [in] image the corresponding 2d positions (x, y) in
 * imagery raster coordinate systems, interleaved by sensor models. 
 *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate system
 *  \return Returns true if the operation succeeds
 *
 *  \details
 */
bool StereoModel::ImageToGround(const std::vector<Point2d> &image, Point3d &ground) const {
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
 *  \param [in] images the corresponding 2d positions (x, y) in
 * imagery raster coordinate systems, interleaved by sensor models. 
 *  \param [in/out] grounds List of 3d ground positions (X, Y, Z) in ground coordinate
 * system
 *
 *  \details
 */
size_t StereoModel::ImageToGround(const std::vector<Point2d> &images, std::vector<Point3d> &grounds) const {
  const size_t numModels = mModels.size();
  const long size = images.size() / numModels;

  auto iter{ std::cbegin(images) };
  size_t ret{};

  #pragma omp parallel for reduction(+ : ret)
  for (long i = 0; i < size; ++i)
  {
      const long j = i * numModels;
      auto iterJ = std::next(iter, j);

      std::vector<Point2d> image(iterJ, std::next(iterJ, numModels));
      if (ImageToGround(image, grounds[i])) {
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
 *  \param [out] image 2d position (x, y) in imagery raster coordinate system 
 *  \return Returns true if operation succeeds
 *
 *  \details
 */
bool StereoModel::GroundToImage(const Point3d &ground, std::vector<Point2d> &image) const {
  const long size = mModels.size();
  image.resize(size);

  bool ret{true};
  for (long i = 0; i < size; ++i) {
      auto g2s = mModels[i]->GroundToImage(ground, image[i]);
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
 *  \param [in/out] images List of 2d positions (x, y) on image,
 * interleaved by sensor models
 *
 *  \details
 */
size_t StereoModel::GroundToImage(const std::vector<Point3d> &grounds, std::vector<Point2d> &images) const {
  const long size = grounds.size();
  const auto numModels{mModels.size()};
  images.resize(size * numModels);

  auto iter = std::begin(images);
  size_t ret{};

  #pragma omp parallel for reduction(+ : ret)
  for (long i = 0; i < size; ++i) {
      std::vector<Point2d> image(numModels);
      if (GroundToImage(grounds[i], image)) {
          std::copy_n(std::begin(image), numModels, std::next(iter, numModels*i));
          ++ret;
      }
  }
  return ret;
}
