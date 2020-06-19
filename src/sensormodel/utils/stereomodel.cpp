/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/stereomodel.h"
#include "3rdparty/gettext/exgettext.h"
#include "core/exceptions/exexcept.h"
#include "core/math/intersect.h"
#include "core/utils/unitutils.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <omp.h>

using namespace earthxpert;
using namespace earthxpert::math;
using namespace earthxpert::thirdparty;

/**
 *  Constructor.
 *
 *  \brief Form a stereo model by a pair of sensor models.
 *
 *  \param [in] leftModel left sensor model composing the stereo model
 *  \param [in] rightModel right sensor model composing the stereo model
 */
StereoModel::StereoModel(ISensorModel const* const leftModel, ISensorModel const* const rightModel) {
  mModels.resize(2);
  mModels[0] = leftModel;
  mModels[1] = rightModel;
  ObtainModelsInfo();
}

/**
 *  \brief Form a triplet stereo model by three sensor models.
 *
 *  \param [in] sensorModels triplet sensor models composing the stereo model
 */
StereoModel::StereoModel(const std::array<ISensorModel const*, 3>& sensorModels) {
  mModels.resize(3);
  std::copy_n(std::begin(sensorModels), 3, std::begin(mModels));
  ObtainModelsInfo();
}

/**
 *  \brief form a stereo model by the given two or more sensor models.
 *
 *  \param [in] sensorModels multiple (two or more) sensor models composing
 * the stereo model.
 */
StereoModel::StereoModel(const std::vector<ISensorModel const*>& sensorModels) {
  assert(2 <= sensorModels.size());
  mModels = sensorModels;
  ObtainModelsInfo();
}

void StereoModel::SetEarthEllipsoid(std::shared_ptr<EarthEllipsoid> earthEllip) {
  if (!earthEllip)
    throw std::invalid_argument("StereoModel: earthEllip");
  mEarthEllip = earthEllip;

  const auto numRays = mModels.size();
  if (2 == numRays) {
    mIntersect = std::make_unique<XTwoLines>();
  }
  else if (3 == numRays) {
    mIntersect = std::make_unique<XThreeLines>();
  }
  else {
    //mIntersect = std::make_unique<XMultipleLines>();
    throw std::exception(_("StereoModel: not implemented for four or more rays"));
  }
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

#pragma omp parallel reduction(+ : ret)
  {
    std::vector<Point2d> image(numModels);

#pragma omp for
    for (long i = 0; i < size; ++i) {
      if (GroundToImage(grounds[i], image)) {
        std::copy_n(std::begin(image), numModels, std::next(iter, numModels * i));
        ++ret;
      }
    }
  }
  return ret;
}

// Newton-Raphson
bool StereoModel::ImageToGroundNR(const Point2d& leftImage,
  const Point2d& rightImage, Point3d& ground) const {
  assert(2 == mModels.size());
  ground.z = (mModels[0]->GetMeanHeight() + mModels[1]->GetMeanHeight()) / 2.0;

  int i;
  for (i = 0; i < mMaxIters; ++i) {
    Point3d grd{ 0.0, 0.0, ground.z + mH };

    Point3d leftGrd{ 0.0, 0.0, ground.z };
    mModels[0]->ImageToGround(leftImage, grd);
    mModels[0]->ImageToGround(leftImage, leftGrd);
    const Point2d dLeft_dz{ (grd.x - leftGrd.x), (grd.y - leftGrd.y) };

    Point3d rightGrd{ 0.0, 0.0, ground.z };
    mModels[1]->ImageToGround(rightImage, grd);
    mModels[1]->ImageToGround(rightImage, rightGrd);
    const Point2d dRight_dz{ (grd.x - rightGrd.x), (grd.y - rightGrd.y) };

    const Point2d A = (dRight_dz - dLeft_dz) * mInvH;
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

bool StereoModel::ImageToGroundNR(const std::array<Point2d, 3>& images, Point3d& ground) const {
  assert(3 == mModels.size());
  ground.z = (mModels[0]->GetMeanHeight() + mModels[1]->GetMeanHeight() + 
    mModels[2]->GetMeanHeight()) / 3.0;

  int i;
  for (i = 0; i < mMaxIters; ++i) {
    const double hgtRange[2] = { ground.z + mH , ground.z };

    double dist[2];   // misclosure
    Point3d grds[3];

    for (int j = 0; j < 2; ++j) {
      grds[0].z = grds[0].z = grds[0].z = hgtRange[j];

      for (int k = 0; k < 3; ++k) {
        mModels[k]->ImageToGround(images[k], grds[k]);
      }
      dist[j] = normL2Sqr(grds[0] - grds[1]) +
        normL2Sqr(grds[1] - grds[2]) + normL2Sqr(grds[2] - grds[0]);
    }

    const double dd_dz = (dist[0] - dist[1]) * mInvH;
    if (std::numeric_limits<double>::epsilon() > dd_dz) {
      return false;
    }

    ground.x = (grds[0].x + grds[1].x + grds[2].x) / 3.0;
    ground.y = (grds[0].y + grds[1].y + grds[2].y) / 3.0;

    const double dz = - dist[1] / dd_dz;
    ground.z += dz;

    if (mTolZ > fabs(dz)) break;
  }

  if (mMaxIters == i) {
    return false;
  }

  return true;
}

/**
 *  \brief Given two or more sensor models and associated list of 2d positions
 *  in image coordinate system, do forward intersection of rays, and
 *  output the ground positions with respect to the ground coordinate system.
 *
 *  \param [in] images the corresponding 2d positions (x, y) in
 * imagery raster coordinate systems, interleaved by sensor models.
 *  \param [in] numRays one ray per model.
 *  \param [in/out] grounds List of 3d ground positions (X, Y, Z) in ground coordinate
 * system
 */
bool StereoModel::ImageToGroundX(const Point2d* images, const int numRays, Point3d& ground) const {
  assert(numRays == mModels.size());
  ground.z = 0.0;
  for (int k = 0; k < numRays; ++k) {
    ground.z += mModels[k]->GetMeanHeight();
  }
  ground.z /= double(numRays);

  std::vector<Point3d> segs(numRays * 2);
  Point3d lsr;

  for (int i = 0; i < 2; ++i) {
    const double hgtRange[2] = { ground.z + mH, ground.z - mH };

    for (int j = 0; j < 2; ++j) {
      Point3d grd{ 0.0, 0.0, hgtRange[j] };

      for (int k = 0; k < numRays; ++k) {
        mModels[k]->ImageToGround(images[k], grd);
        segs[j + k * 2] = mEarthEllip->LPHToLSR(Deg2Rad(grd));
      }
    }

    // 3d intersection X
    Solver::Summary summary;
    const bool ret = mIntersect->IntersectCeres(segs.data(), lsr, summary);
    //std::cout << summary.BriefReport() << std::endl;
    //const bool ret = mIntersect->IntersectEigen(segs.data(), lsr, numRays);   // IntersectTwoLines(segs.data(), lsr, numRays)
    
    if (!ret) return false;
    ground = Rad2Deg(mEarthEllip->LSRToLPH(lsr));
  }

  return true;
}

void StereoModel::ObtainModelsInfo() {
  if (2 > mModels.size())
    throw std::exception(_("StereoModel: requires 2 or more models"));

  // verify the datum be compatible
  if (mEarthEllip) {
    EqualD eq;
    const auto semiMajor = mEarthEllip->GetSemiMajor();
    const auto eccSq = mEarthEllip->GetEccSquared();

    for (auto sm : mModels) {
      auto srs = sm->GetCoordinateSystem();
      const auto a = srs->GetSemiMajor();
      const auto ecc2 = srs->GetSquaredEccentricity();

      if (!eq(semiMajor, a) || !eq(eccSq, ecc2)) {
        throw std::exception(_("StereoModel: in-compatible datum"));
      }
    }
  }
}
