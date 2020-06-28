/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "epipolarmodel/epipolarmodel.h"

#include <algorithm>
#include <cmath>
#include <cassert>
#include <execution>
#include <omp.h>

using earthxpert::GeographicGroundDeg;
using earthxpert::LocalGroundMe;
using earthxpert::MapGroundMe;
using earthxpert::EpipolarModel;
using earthxpert::EpipolarRect;
using earthxpert::IEpipolarModel;
using earthxpert::ISensorModel;
using earthxpert::InterpGridF;
using earthxpert::Point2d;
using earthxpert::Point3d;

/**
 *  Constructor.
 *
 *  \param [in] sensorModel sensor model associated with the image
 *  \param [in] epiRect epipolar rectification defining the transform
 *              between the epipolar model coordinate system and epipolar
 *              raster coordinate system for the image.
 *  \param [in] pos Component position of stereo model
 *  \param [in] groundPolicy ground policy
 *  \remark This implementation requires that the stereo pair are of the same sensro type.
 */
template <class GroundPolicy>
EpipolarModel<GroundPolicy>::EpipolarModel(std::shared_ptr<ISensorModel> sensorModel,
  std::unique_ptr<EpipolarRect>&& epiRect, StereoPos pos, std::shared_ptr<GroundPolicy> groundPolicy)
    : mSensorModel(sensorModel), mEpiRect(std::move(epiRect)), 
      mPos(pos), mGroundPolicy(groundPolicy) {
  assert(GroundPolicy::kGroundType == sensorModel->GetSrsType());
  mOrigin = mEpiRect->GetOrigin();
  mHeight = mEpiRect->GetHeight();

  const auto[numCols, numRows] = sensorModel->GetImageSize();
  mGridI2G = BuildGridI2G(numCols, numRows);
}

/**
 *  Epipolar-to-raw transform.
 *
 *  \brief Given a 2d poistion on epipolar, output the corresponding position on
 * original image.
 *
 *  \param [in] epi 2d position (col, row) in epipolar raster coordinate system
 *  \param [in] raw 2d position (col, row) in image raster coordinate system
 *  \return Returns true if operation succeeds
 */
template <class GroundPolicy>
bool EpipolarModel<GroundPolicy>::EpiToRaw(const Point2d &epi, Point2d &raw) const {
  const Point2d rect = mEpiRect->EpiToRect(epi);
  const Point3d ground = mGroundPolicy->RectToGround(Point3d(rect.x, rect.y, mHeight));
  return mSensorModel->GroundToImage(ground, raw);
}

/**
 *  \brief Given a list of 2d poistions on epipolar, output the corresponding
 * positions on original image.
 *
 *  \param [in] epi List of 2d positions (col, row) in epipolar raster coordinate
 * system 
 *  \param [in] raw List of 2d positions (col, row) in image raster coordinate
 * system 
 *  \return Returns number of successful operations
 */
template <class GroundPolicy>
size_t EpipolarModel<GroundPolicy>::EpiToRaw(const std::vector<Point2d> &epi,
                               std::vector<Point2d> &raw) const {
  const long size = epi.size();
  assert(size == raw.size());
  size_t ret{};

  #pragma omp parallel for reduction(+ : ret) //if()
  for (long i = 0; i < size; ++i) {
    if (EpiToRaw(epi[i], raw[i])) {
      ++ret;
    }
  }
  return ret;
}

/**
 *  Raw-to-epipolar transform.
 *
 *  \brief Given a 2d poistion on image, output the corresponding position on
 * epipolar.
 *
 *  \param [in] raw 2d position (col, row) in image raster coordinate system
 *  \param [in] epi 2d position (col, row) in epipolar raster coordinate system
 *  \return Returns true if operation succeeds
 */
template <class GroundPolicy>
bool EpipolarModel<GroundPolicy>::RawToEpi(const Point2d &raw, Point2d &epi) const {
  Point2d rect{};
  if (!RawToRect(raw, rect))
    return false;

  epi = mEpiRect->RectToEpi(rect);
  return true;
}

/**
 *  \brief Given a list of 2d poistions on image, output the corresponding
 * positions on epipolar.
 *
 *  \param [in] raw List of 2d positions (col, row) in image raster coordinate
 * system 
 *  \param [in] epi List of 2d positions (col, row) in epipolar raster
 * coordinate system 
 *  \return Returns number of successful operations
 */
template <class GroundPolicy> size_t
EpipolarModel<GroundPolicy>::RawToEpi(const std::vector<Point2d> &raw, std::vector<Point2d> &epi) const {
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
template <class GroundPolicy> size_t 
EpipolarModel<GroundPolicy>::ImageToGround(const Point2d& image,
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
template <class GroundPolicy> size_t 
EpipolarModel<GroundPolicy>::ImageToGround(const std::vector<Point2d>& image,
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
template <class GroundPolicy> size_t 
EpipolarModel<GroundPolicy>::GroundToImage(const std::vector<Point3d>& ground,
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

/**
 *  Raw-to-rect transform.
 *
   *  \brief Given a 2d poistion on image, output the 2d position in
   * rectification coordinate system.
   *
   *  \param [in] raw 2d position (col, row) in image raster coordinate system
   *  \param [out] rect 2d position (x, y) in rectification coordinate system
   *  \return Returns heihgt in model ground CRS if operation succeeds
 */
template <class GroundPolicy> std::optional<double> 
EpipolarModel<GroundPolicy>::RawToRect(const Point2d &raw, Point2d &rect) const {
  Point3d ground{ 0, 0, mHeight };
  if (mGridI2G)
    ground.z += mGridI2G->GetVal1(float(raw.x), float(raw.y));

  Point3d rect3{ 0, 0, 0 };
  for (int i = 0; i < kMaxIters; ++i) {
    if (!mSensorModel->ImageToGround(raw, ground))
      return std::nullopt;
    
    rect3 = mGroundPolicy->GroundToRect(ground);
    const double delta = mHeight - rect3.z;
    ground.z += delta;

    if (kHgtTol > delta) break;
  }

  rect = { rect3.x, rect3.y };
  return { ground.z };
}

/**
 *  \brief build grids to get heights in model CRS given image positions
 * and heights in rectification CRS.
 *
 *  \param [in] numCols Image width [pixel]
 *  \param [in] numRows Image height [pixel]
 *  \return Returns interpolation grid.
 */
template <class GroundPolicy> std::unique_ptr<InterpGridF> 
EpipolarModel<GroundPolicy>::BuildGridI2G(const int numCols, const int numRows) const {
  auto gridI2G = std::make_unique<InterpGridF>(1, 0.0f, 0.0f, 
    float(numCols), float(numRows), float(kGridStride), float(kGridStride));

  const auto numNodes = gridI2G->GetNumNodes();
  std::vector<Point2f> gridNodes(numNodes);
  gridI2G->GetNodes(gridNodes.data());

  auto grid = gridI2G->GetArray();
  for (size_t i = 0; i < numNodes; ++i) {
    Point2d rect;
    auto grdHgt = RawToRect(gridNodes[i], rect);

    if (!grdHgt)
      throw std::exception("BuildGridI2G: failed to project.");

    grid[i] = grdHgt.value() - mHeight;
  }

  return gridI2G;
}

template class EpipolarModel<GeographicGroundDeg>;
template class EpipolarModel<LocalGroundMe>; 
template class EpipolarModel<MapGroundMe>;
