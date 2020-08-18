/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "epipolarmodel/epipolarmodel.h"
#include "core/exceptions/exexcept.h"

#include <algorithm>
#include <cmath>
#include <cassert>
#include <execution>
#include <omp.h>

using namespace earthxpert;

/**
 *  Constructor.
 *
 *  \param [in] pos Component position of stereo model
 *  \param [in] sensorModel sensor model associated with the image
 *  \param [in] epiRect epipolar rectification defining the transform
 *              between the epipolar model coordinate system and epipolar
 *              raster coordinate system for the image.
 *  \param [in] epiBound epipolar boundary of valid region
 *  \param [in] groundPolicy ground policy
 *  \remark This implementation requires that the stereo pair are of the same sensro type.
 */
template <class GroundPolicy>
EpipolarModel<GroundPolicy>::EpipolarModel(StereoPos pos, 
  std::shared_ptr<ISensorModel> sensorModel, const Polygon2d& epiBound, 
  std::unique_ptr<EpipolarRect>&& epiRect, std::shared_ptr<GroundPolicy> groundPolicy)
    : EpipolarSensorModel(pos, sensorModel, epiBound),
      mEpiRect(std::move(epiRect)), mGroundPolicy(groundPolicy) {
  assert(GroundPolicy::kGroundType == sensorModel->GetSrsType());
  mOrigin = mEpiRect->mOrigin;
  mHeight = mEpiRect->mHeight;

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
  const Point3d ground = mGroundPolicy->CartesianToGround(Point3d(rect.x, rect.y, mHeight));
  return mSensorModel->GroundToImage(ground, raw);
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
 *  Raw-to-rect transform.
 *
 *  \brief Given a 2d poistion on image, output the 2d position in
 * rectification coordinate system.
 *
 *  \param [in] raw 2d position (col, row) in image raster coordinate system
 *  \param [out] rect 2d position (x, y) in rectification coordinate system
 *  \return Returns heihgt in model ground CRS if operation succeeds
 *  \ref ImageToPlane in sensormodel/utils.h
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
    
    rect3 = mGroundPolicy->GroundToCartesian(ground);
    const double dz = mHeight - rect3.z;
    ground.z += dz;

    if (kHgtTol > dz) break;
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
      throw std::exception(_("BuildGridI2G: failed to project."));

    grid[i] = grdHgt.value() - mHeight;
  }

  return gridI2G;
}

template class EpipolarModel<GeographicGroundDeg>;
template class EpipolarModel<LocalGroundMe>; 
template class EpipolarModel<MapGroundMe>;
