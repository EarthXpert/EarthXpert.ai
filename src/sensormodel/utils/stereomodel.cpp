/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/utils/stereomodel.h"
#include "sensormodel/utils/intersecti2g.h"
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
using namespace ceres;
using namespace ::Eigen;

/**
 *  Constructor.
 *
 *  \brief Form a stereo model by a pair of sensor models.
 *
 *  \param [in] leftModel left sensor model composing the stereo model
 *  \param [in] rightModel right sensor model composing the stereo model
 */
StereoModel::StereoModel(ISensorModel const* const leftModel, 
  ISensorModel const* const rightModel) {
  mModels.resize(2);
  mModels[0] = leftModel;
  mModels[1] = rightModel;
  Initialize();
}

/**
 *  \brief Form a triplet stereo model by three sensor models.
 *
 *  \param [in] sensorModels triplet sensor models composing the stereo model
 */
StereoModel::StereoModel(const std::array<ISensorModel const*, 3>& sensorModels) {
  mModels.resize(3);
  std::copy_n(std::begin(sensorModels), 3, std::begin(mModels));
  Initialize();
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
  Initialize();
}

/**
 *  \todo how to set tolerances of different parameters, e.g., Lon/lat (arc degree, 1e-8) vs height (m, 1e-3)?
 *  \todo verify consitency among all sensor models
 */
void StereoModel::Initialize() {
  if (2 > mModels.size())
    throw std::exception(_("StereoModel: requires 2 or more models"));

  // default forward intersection algorithm
  mSensorType = mModels[0]->GetModelType();
  switch (mSensorType) {
  case SensorType::eFRAME:
    break;
  case SensorType::eRATIONAL:
    mFwdAlgo = eSM_NR;
    break;
  default:
    mFwdAlgo = eSM_NR;
    break;
  }

  mSrsType = mModels[0]->GetSrsType();
  switch (mSrsType) {
  case eGEO:
    mTolXY = kTolXYD;
    break;
  default:
    mTolXY = kTolXYL;
    break;
  }
  mFacLP = 0.5 / mTolXY;

  // verify the datum be compatible
  if (mEarthEllip) {
    EqualD eq;
    const auto semiMajor = mEarthEllip->GetSemiMajor();
    const auto eccSq = mEarthEllip->GetEccSquared();

    for (auto sm : mModels) {
      auto srs = sm->GetCoordinateSystem();
      const auto a = srs->GetSemiMajor();
      const auto ecc2 = srs->GetSquaredEccentricity();

      if (!eq(semiMajor, a) || !eq(eccSq, ecc2)) 
        throw std::exception(_("StereoModel: in-compatible datum"));
    }
  }

  mOptions.max_num_iterations = 20;
  mOptions.num_threads = 1; // kNumPhysicalCores
  //mOptions.function_tolerance = 1e-3;   // [pixel]
  //mOptions.gradient_tolerance = 1e-7;
  //mOptions.parameter_tolerance = 1e-8;  // [degree, degree, meter]
  mOptions.linear_solver_type = DENSE_QR; // DENSE_NORMAL_CHOLESKY
  //mOptions.preconditioner_type = JACOBI;  // SUBSET
  mOptions.minimizer_progress_to_stdout = false;
  //mOptions.update_state_every_iteration = true;
  //mOptions.callbacks = {};
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

void StereoModel::SetForwardAlgorithm(const ForwardAlgo fwdAlgo) {
  // verify consistency
  switch (fwdAlgo) {
  case eSM_CERES: {
    break;
  }
  case eCA_INTERSECT: {
    if (eGEO == mSrsType && !(mEarthEllip && mIntersect)) 
      throw std::exception("StereoModel: SetForwardAlgorithm");
    break;
  }
  case eSM_NR: {
    break;
  }
  default: {
    break;
  }
  }

  mFwdAlgo = fwdAlgo; 
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
 */
size_t StereoModel::ImagesToGround(const std::vector<Point2d> &leftImages,
  const std::vector<Point2d> &rightImages, std::vector<Point3d> &grounds) const {
  const auto size = static_cast<long>(leftImages.size());
  assert(size == rightImages.size());

  grounds.resize(size);
  size_t ret{};

  #pragma omp parallel for reduction(+ : ret)
  for (long i = 0; i < size; ++i) {
    if (ImagesToGround(leftImages[i], rightImages[i], grounds[i])) {
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
 */
size_t StereoModel::ImagesToGround(const std::vector<Point2d> &images, 
  std::vector<Point3d> &grounds) const {
  const size_t numModels = mModels.size();
  const long size = images.size() / numModels;

  auto iter{ std::cbegin(images) };
  size_t ret{};

  #pragma omp parallel for reduction(+ : ret)
  for (long i = 0; i < size; ++i) {
      const long j = i * numModels;
      auto iterJ = std::next(iter, j);

      std::vector<Point2d> image(iterJ, std::next(iterJ, numModels));
      if (ImagesToGround(image, grounds[i])) {
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
 */
bool StereoModel::GroundToImages(const Point3d &ground, 
  std::vector<Point2d> &image) const {
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
 */
size_t StereoModel::GroundToImages(const std::vector<Point3d> &grounds, 
  std::vector<Point2d> &images) const {
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
      if (GroundToImages(grounds[i], image)) {
        std::copy_n(std::begin(image), numModels, std::next(iter, numModels * i));
        ++ret;
      }
    }
  }
  return ret;
}

// Ceres
// \todo intial position
bool StereoModel::ImagesToGroundCeres(const Point2d& leftImage,
  const Point2d& rightImage, Point3d& ground) const {
  assert(2 == mModels.size());
  ground.z = (mModels[0]->GetMeanHeight() + mModels[1]->GetMeanHeight()) / 2.0;
  mModels[0]->ImageToGround(leftImage, ground);

  const Point2d images[2] = { leftImage , rightImage };
  Solver::Summary summary;
  const auto ret = IntersectI2G<NumericG2IResidual>(images,
    mModels.data(), 2, ground, mOptions, summary);
  
  //std::atomic_fetch_add_explicit(&mNumIters, 1, std::memory_order_relaxed);
  return ret;
}

// Least Squares
bool StereoModel::ImagesToGroundLS(const Point2d& leftImage,
  const Point2d& rightImage, Point3d& ground) const {
  assert(2 == mModels.size());
  ground.z = (mModels[0]->GetMeanHeight() + mModels[1]->GetMeanHeight()) / 2.0;
  mModels[0]->ImageToGround(leftImage, ground);

  std::array<double, 12> dm;
  std::array<double, 4> cv;

  Map<Eigen::Matrix<double, 4, 3, RowMajor>> A(dm.data());
  Map<Eigen::Matrix<double, 4, 1>> b(cv.data());

  int iter{};
  for (iter = 0; iter < kMaxIters; ++iter) {
    //std::atomic_fetch_add_explicit(&mNumIters, 1, std::memory_order_relaxed);

    Point2d imgp0, imgp1;
    std::tie(dm[0], dm[1], dm[2], dm[3], dm[4], dm[5]) =
      CalcJacobianNc(mModels[0], ground, imgp0);
    std::tie(dm[6], dm[7], dm[8], dm[9], dm[10], dm[11]) =
      CalcJacobianNc(mModels[1], ground, imgp1);

    cv[0] = imgp0.x - leftImage.x;
    cv[1] = imgp0.y - leftImage.y;
    cv[2] = imgp1.x - rightImage.x;
    cv[3] = imgp1.y - rightImage.y;

    const auto& x = A.bdcSvd(ComputeFullU | ComputeFullV).solve(b);
    //if () return false;

    ground -= Point3d(x[0], x[1], x[2]);

    if (fabs(x[0]) < kTolXYD && fabs(x[1]) < kTolXYD && fabs(x[2]) < kTolZ)
      break;
  }

  // return false if reaching max. iterations
  if (kMaxIters == iter)
    return false;

  return true;
}

// Newton-Raphson
bool StereoModel::ImagesToGroundNR(const Point2d& leftImage,
  const Point2d& rightImage, Point3d& ground) const {
  assert(2 == mModels.size());
  ground.z = (mModels[0]->GetMeanHeight() + mModels[1]->GetMeanHeight()) / 2.0;

  int i;
  for (i = 0; i < kMaxIters; ++i) {
    //std::atomic_fetch_add_explicit(&mNumIters, 1, std::memory_order_relaxed);
    Point3d grd{ 0.0, 0.0, ground.z + kH };

    Point3d leftGrd{ 0.0, 0.0, ground.z };
    mModels[0]->ImageToGround(leftImage, grd);
    mModels[0]->ImageToGround(leftImage, leftGrd);
    const Point2d dLeft_dz{ (grd.x - leftGrd.x), (grd.y - leftGrd.y) };

    Point3d rightGrd{ 0.0, 0.0, ground.z };
    mModels[1]->ImageToGround(rightImage, grd);
    mModels[1]->ImageToGround(rightImage, rightGrd);
    const Point2d dRight_dz{ (grd.x - rightGrd.x), (grd.y - rightGrd.y) };

    const Point2d A = (dRight_dz - dLeft_dz) * kInvH;
    const double den = cv::normL2Sqr<double>(A);
    if (std::numeric_limits<double>::epsilon() > den) 
      return false;

    ground.x = (leftGrd.x + rightGrd.x) * 0.5;
    ground.y = (leftGrd.y + rightGrd.y) * 0.5;

    const double dz = A.ddot(Point2d(leftGrd.x - rightGrd.x, leftGrd.y - rightGrd.y)) / den;
    ground.z += dz;

    if (kTolZ > fabs(dz)) break;
  }

  if (kMaxIters == i) 
    return false;

  return true;
}

// Ceres
// \todo intial position
bool StereoModel::ImagesToGroundCeres(
  const std::array<Point2d, 3>& images, Point3d& ground) const {
  assert(3 == mModels.size());
  ground.z = (mModels[0]->GetMeanHeight() + mModels[1]->GetMeanHeight() +
    mModels[2]->GetMeanHeight()) / 3.0;
  mModels[1]->ImageToGround(images[1], ground);

  Solver::Summary summary;
  const auto ret = IntersectI2G<NumericG2IResidual>(images.data(), 
    mModels.data(), 3, ground, mOptions, summary);

  return ret;
}

bool StereoModel::ImagesToGroundLS(
  const std::array<Point2d, 3>& images, Point3d& ground) const {
  assert(3 == mModels.size());
  ground.z = (mModels[0]->GetMeanHeight() + mModels[1]->GetMeanHeight() +
    mModels[2]->GetMeanHeight()) / 3.0;

  return false;
}

bool StereoModel::ImagesToGroundNR(
  const std::array<Point2d, 3>& images, Point3d& ground) const {
  assert(3 == mModels.size());
  ground.z = (mModels[0]->GetMeanHeight() + mModels[1]->GetMeanHeight() + 
    mModels[2]->GetMeanHeight()) / 3.0;

  int i;
  for (i = 0; i < kMaxIters; ++i) {
    const double hgtRange[2] = { ground.z + kH , ground.z };

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

    const double dd_dz = (dist[0] - dist[1]) * kInvH;
    if (std::numeric_limits<double>::epsilon() > dd_dz) 
      return false;

    ground.x = (grds[0].x + grds[1].x + grds[2].x) / 3.0;
    ground.y = (grds[0].y + grds[1].y + grds[2].y) / 3.0;

    const double dz = - dist[1] / dd_dz;
    ground.z += dz;

    if (kTolZ > fabs(dz)) break;
  }

  if (kMaxIters == i) 
    return false;

  return true;
}

// Ceres
// \todo intial position
bool StereoModel::ImagesToGroundCeres(const Point2d* images, 
  const int numRays, Point3d& ground) const {
  assert(2 <= mModels.size());
  ground.z = 0.0;
  for (int k = 0; k < numRays; ++k) {
    ground.z += mModels[k]->GetMeanHeight();
  }
  ground.z /= double(numRays);
  mModels[numRays/2]->ImageToGround(images[numRays/2], ground);

  Solver::Summary summary;
  const auto ret = IntersectI2G<NumericG2IResidual>(images,
    mModels.data(), numRays, ground, mOptions, summary);

  return ret;
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
bool StereoModel::ImagesToGroundX(const Point2d* images, 
  const int numRays, Point3d& ground) const {
  assert(numRays == mModels.size());
  ground.z = 0.0;
  for (int k = 0; k < numRays; ++k) {
    ground.z += mModels[k]->GetMeanHeight();
  }
  ground.z /= double(numRays);

  std::vector<Point3d> segs(numRays * 2);
  Point3d lsr;

  for (int i = 0; i < 2; ++i) {
    //std::atomic_fetch_add_explicit(&mNumIters, 1, std::memory_order_relaxed);
    const double hgtRange[2] = { ground.z + kH, ground.z - kH };

    for (int j = 0; j < 2; ++j) {
      for (int k = 0; k < numRays; ++k) {
        const int pi = j + k * 2;   // point index
        segs[pi].z = hgtRange[j];

        mModels[k]->ImageToGround(images[k], segs[pi]);
        if (eGEO == mSrsType)
          segs[pi] = mEarthEllip->LPHToLSR(Deg2Rad(segs[pi]));
      }
    }

    // 3d intersection X
    //Solver::Summary summary;
    //const bool ret = mIntersect->IntersectCeres(segs.data(), lsr, summary);
    //std::cout << summary.BriefReport() << std::endl;
    const bool ret = mIntersect->IntersectEigen(segs.data(), lsr, numRays);   // IntersectTwoLines(segs.data(), lsr, numRays)
    
    if (!ret) return false;
    if (eGEO == mSrsType)
      ground = Rad2Deg(mEarthEllip->LSRToLPH(lsr));
    else
      ground = lsr;
  }

  return true;
}

/**
 *  \brief Calculate Jacobian at given ground position
 *  (central finite difference).
 *
 *  \param [out] image estimated col/row
 *  \return Return {dFc_dX, dFc_dY, dFc_dZ, dFr_dX, dFr_dY, dFr_dZ}
 */
Tuple6d StereoModel::CalcJacobianNc(ISensorModel const* model,
  const Point3d& ground, Point2d& image) const {
  model->GroundToImage(ground, image);

  Point3d grds[6] = {
    {ground.x - mTolXY, ground.y, ground.z}, {ground.x + mTolXY, ground.y, ground.z},
    {ground.x, ground.y - mTolXY, ground.z}, {ground.x, ground.y + mTolXY, ground.z},
    {ground.x, ground.y, ground.z - kTolZ}, {ground.x, ground.y, ground.z + kTolZ} };
  Point2d imgs[6];

  for (int i = 0; i < 6; ++i) {
    model->GroundToImage(grds[i], imgs[i]);
  }

  const auto dF_dX = (imgs[1] - imgs[0]) * mFacLP;  // dFc_dX, dFr_dX
  const auto dF_dY = (imgs[3] - imgs[2]) * mFacLP;  // dFc_dY, dFr_dY
  const auto dF_dZ = (imgs[5] - imgs[4]) * kFacH;   // dFc_dZ, dFr_dZ

  return { dF_dX.x, dF_dY.x, dF_dZ.x, dF_dX.y, dF_dY.y, dF_dZ.y };
}
