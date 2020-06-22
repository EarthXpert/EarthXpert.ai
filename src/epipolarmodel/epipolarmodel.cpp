/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "epipolarmodel/epipolarmodel.h"
#include "epipolarmodel/epipolarrect.h"
#include "sensormodel/isensormodel.h"

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
using EpiStereoPos = IEpipolarModel::StereoPos;

/**
 *  Constructor.
 *
 *  \param [in] leftModel sensor model associated with the left image
 *  \param [in] leftRect epipolar rectification defining the transform between
 * the epipolar model coordinate system and epipolar raster coordinate system
 *              for the left image
 *  \param [in] rightModel sensor model associated with the right image
 *  \param [in] rightRect epipolar rectification defining the transform between
 * the epipolar model coordinate system and epipolar raster coordinate system
 *              for the right image
 *  \remark This implementation requires that the stereo pair are of the same sensro type.
 */
template <class GroundPolicy>
EpipolarModel<GroundPolicy>::EpipolarModel(std::shared_ptr<ISensorModel> leftModel,
                             std::unique_ptr<EpipolarRect> &&leftRect,
                             std::shared_ptr<ISensorModel> rightModel,
                             std::unique_ptr<EpipolarRect> &&rightRect,
                             std::unique_ptr<GroundPolicy> &&groundPolicy)
    : mLeftModel(leftModel), mLeftRect(std::move(leftRect)), 
  mRightModel(rightModel), mRightRect(std::move(rightRect)),
  mGroundPolicy(std::move(groundPolicy)) {
  assert(GroundPolicy::kGroundType == leftModel->GetSrsType());

  mOrigin = mLeftRect->GetOrigin();
  assert(mOrigin == mRightRect->GetOrigin());

  mHeight = mLeftRect->GetHeight();
  assert(mHeight == mRightRect->GetHeight());

  const auto[lNumCols, lNumRows] = leftModel->GetImageSize();
  mLeftGridI2G = BuildGridI2G(EpiStereoPos::eLEFT, lNumCols, lNumRows);

  const auto[rNumCols, rNumRows] = rightModel->GetImageSize();
  mRightGridI2G = BuildGridI2G(EpiStereoPos::eRIGHT, rNumCols, rNumRows);
}

/**
 *  Epipolar-to-image transform.
 *
 *  \brief Given a 2d poistion on epipolar, output the corresponding position on
 * original image.
 *
 *  \param [in] epi 2d position (col, row) in epipolar raster coordinate system
 *  \param [in] img 2d position (col, row) in image raster coordinate system
 *  \param [in] pos Component position of stereo model
 *  \return Returns true if operation succeeds
 */
template <class GroundPolicy>
bool EpipolarModel<GroundPolicy>::EpipolarToImage(const Point2d &epi, Point2d &img,
                                    const IEpipolarModel::StereoPos pos) const {
  const EpipolarRect* rectifier{};
  const ISensorModel* sensorModel{};

  switch (pos) {
  case EpiStereoPos::eLEFT:
    rectifier = mLeftRect.get();
    sensorModel = mLeftModel.get();
    break;

  case EpiStereoPos::eRIGHT:
    rectifier = mRightRect.get();
    sensorModel = mRightModel.get();
    break;

  default:
    return false;
  }

  const Point2d rect = rectifier->EpiToRect(epi);
  const Point3d ground = mGroundPolicy->RectToGround(Point3d(rect.x, rect.y, mHeight));
  return sensorModel->GroundToImage(ground, img);
}

/**
 *  \brief Given a list of 2d poistions on epipolar, output the corresponding
 * positions on original image.
 *
 *  \param [in] epi List of 2d positions (col, row) in epipolar raster coordinate
 * system 
 *  \param [in] img List of 2d positions (col, row) in image raster coordinate
 * system 
 *  \param [in] pos Component position of stereo model 
 *  \return Returns number of successful operations
 */
template <class GroundPolicy>
size_t
EpipolarModel<GroundPolicy>::EpipolarToImage(const std::vector<Point2d> &epi,
                               std::vector<Point2d> &img,
                               const IEpipolarModel::StereoPos pos) const {
  const long size = epi.size();
  assert(size == img.size());

  size_t ret{};

  #pragma omp parallel for reduction(+ : ret) //if()
  for (long i = 0; i < size; ++i) {
    if (EpipolarToImage(epi[i], img[i], pos)) {
      ++ret;
    }
  }
  return ret;
}

/**
 *  Image-to-epipolar transform.
 *
 *  \brief Given a 2d poistion on image, output the corresponding position on
 * epipolar.
 *
 *  \param [in] img 2d position (col, row) in image raster coordinate system
 *  \param [in] epi 2d position (col, row) in epipolar raster coordinate system
 *  \param [in] pos Component position of stereo model
 *  \return Returns true if operation succeeds
 */
template <class GroundPolicy>
bool EpipolarModel<GroundPolicy>::ImageToEpipolar(const Point2d &img, Point2d &epi,
                                    const IEpipolarModel::StereoPos pos) const {
  Point2d rect{};
  if (!ImageToRect(img, rect, pos))
    return false;

  const auto rectifier = (EpiStereoPos::eLEFT == pos) ? mLeftRect.get() : mRightRect.get();
  epi = rectifier->RectToEpi(rect);
  return true;
}

/**
 *  \brief Given a list of 2d poistions on image, output the corresponding
 * positions on epipolar.
 *
 *  \param [in] img List of 2d positions (col, row) in image raster coordinate
 * system 
 *  \param [in] epi List of 2d positions (col, row) in epipolar raster
 * coordinate system 
 *  \param [in] pos Component position of stereo model 
 *  \return Returns number of successful operations
 */
template <class GroundPolicy>
size_t
EpipolarModel<GroundPolicy>::ImageToEpipolar(const std::vector<Point2d> &img,
                               std::vector<Point2d> &epi,
                               const IEpipolarModel::StereoPos pos) const {
  const long size = img.size();
  assert(size == epi.size());

  size_t ret{};

  #pragma omp parallel for reduction(+ : ret)
  for (long i = 0; i < size; ++i) {
    if (ImageToEpipolar(img[i], epi[i], pos)) {
      ++ret;
    }
  }
  return ret;
}

/**
 *  Image-to-rect transform.
 *
   *  \brief Given a 2d poistion on image, output the 2d position in
   * rectification coordinate system.
   *
   *  \param [in] img 2d position (col, row) in image raster coordinate system
   *  \param [out] rect 2d position (x, y) in rectification coordinate system
   *  \param [in] pos Component position of stereo model
   *  \return Returns heihgt in model ground CRS if operation succeeds
 */
template <class GroundPolicy>
std::optional<double> EpipolarModel<GroundPolicy>::ImageToRect(const Point2d &img, Point2d &rect,
  const IEpipolarModel::StereoPos pos) const {
  const InterpGridF* interpGrid{};
  const ISensorModel* sensorModel{};

  switch (pos) {
  case EpiStereoPos::eLEFT:
    interpGrid = mLeftGridI2G.get();
    sensorModel = mLeftModel.get();
    break;

  case EpiStereoPos::eRIGHT:
    interpGrid = mRightGridI2G.get();
    sensorModel = mRightModel.get();
    break;

  default:
    return std::nullopt;
  }

  Point3d ground{ 0, 0, mHeight };
  if (interpGrid)
    ground.z += interpGrid->GetVal1(float(img.x), float(img.y));

  Point3d rect3{ 0, 0, 0 };
  for (int i = 0; i < kMaxIters; ++i) {
    if (!sensorModel->ImageToGround(img, ground))
      return std::nullopt;
    
    rect3 = mGroundPolicy->GroundToRect(ground);
    const double delta = mHeight - rect3.z;
    ground.z += delta;

    if (kHgtTol > delta)
      break;
  }

  rect = { rect3.x, rect3.y };
  return { ground.z };
}

/**
 *  \brief build grids to get heights in model CRS given image positions
 * and heights in rectification CRS.
 *
 *  \param [in] pos Component position of stereo model
 *  \param [in] numCols Image width [pixel]
 *  \param [in] numRows Image height [pixel]
 *  \return Returns interpolation grid.
 */
template <class GroundPolicy>
std::unique_ptr<InterpGridF> 
EpipolarModel<GroundPolicy>::BuildGridI2G(const IEpipolarModel::StereoPos pos,
  const int numCols, const int numRows) const {
  auto gridI2G = std::make_unique<InterpGridF>(1, 0.0f, 0.0f, 
    float(numCols), float(numRows), float(mGridStride), float(mGridStride));

  const auto numNodes = gridI2G->GetNumNodes();
  std::vector<Point2f> gridNodes(numNodes);
  gridI2G->GetNodes(gridNodes.data());

  auto grid = gridI2G->GetArray();
  for (size_t i = 0; i < numNodes; ++i) {
    Point2d rect;
    auto grdHgt = ImageToRect(gridNodes[i], rect, pos);

    if (!grdHgt)
      throw std::exception("Failed to project.");

    grid[i] = grdHgt.value() - mHeight;
  }

  return gridI2G;
}

template class EpipolarModel<GeographicGroundDeg>;
template class EpipolarModel<LocalGroundMe>; 
template class EpipolarModel<MapGroundMe>;
