/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "epipolarmodel/epipolarmodelfactory.h"
#include "core/utils/typehelper.h"
#include "epipolarmodel/epipolarmodel.h"
#include "sensormodel/framemodel.h"
#include "sensormodel/rationalmodel.h"
#include "sensormodel/utils.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <exception>

using namespace earthxpert;

/**
 *  \brief
 */
EpipolarModelFactory::EpipolarModelFactory(const EpipolarOption epiOpt)
  : mEpiOpt(epiOpt) {}

/**
 *  \brief Create an epipolar models from two sensor models composing of a stereo pair.
 *
 *  \param [in] leftModel sensor model associated with the left image
 *  \param [in] rightModel sensor model associated with the right image
 *  \param [in] height the average height used for epipolar rectficiation
 *  \return Returns pointers to epipolar model objects.
 *  \remark This implementation requires that the stereo pair are of the same sensro type.
 */
EpiModelPair EpipolarModelFactory::CreateEpipolarModel(
  std::shared_ptr<ISensorModel> leftModel, 
  std::shared_ptr<ISensorModel> rightModel, 
  const int sample, const double height) {
  mSensorType = leftModel->GetModelType();
  if (mSensorType != rightModel->GetModelType())
    throw std::exception("The stereo pair should be of the same sensor type.");

  switch (mSensorType) {
  case SensorType::eFRAME:
    return CreateFrameEpiModel(leftModel, rightModel, sample, height);
  case SensorType::eRATIONAL:
    return CreateRationalEpiModel(leftModel, rightModel, sample);
  default:
    return { nullptr, nullptr }; // model types not supported 
  }
}

/**
 *  \brief
 */
std::vector<std::unique_ptr<IEpipolarModel>> EpipolarModelFactory::CreateEpipolarModel(
  const std::vector<std::shared_ptr<ISensorModel>> &sensorModels,
  const int sample, const double height) {
  if (sensorModels.size() < 2) {
    throw std::exception("Require a minimum of two images");
  }
  return std::vector<std::unique_ptr<IEpipolarModel>>{};
}

/**
 *  \brief Create an epipolar models for a stereo pair of frame images.
 *
 *  \param [in] leftModel sensor model associated with the left image
 *  \param [in] rightModel sensor model associated with the right image
 *  \param [in] height the optional average height used for epipolar rectficiation
 *  \return Returns pointers to epipolar model objects.
 */
EpiModelPair EpipolarModelFactory::CreateFrameEpiModel(
  std::shared_ptr<ISensorModel> leftModel, 
  std::shared_ptr<ISensorModel> rightModel, 
  const int sample, const double height) {
  return { nullptr,nullptr };
  }

/**
 *  \brief Create an epipolar models for a stereo pair of RPC images.
 *
 *  \param [in] leftModel sensor model associated with the left image
 *  \param [in] rightModel sensor model associated with the right image
 *  \return Returns pointer to epipolar model objects.
 */
EpiModelPair EpipolarModelFactory::CreateRationalEpiModel(
  std::shared_ptr<ISensorModel> leftModel, 
  std::shared_ptr<ISensorModel> rightModel, const int sample) {
  auto leftRM = dynamic_cast<const RationalModel*>(leftModel.get());
  auto rightRM = dynamic_cast<const RationalModel*>(rightModel.get());

  auto lnfc = leftRM->GetNormalFators();
  auto rnfc = rightRM->GetNormalFators();

  // \todo across anti-meridian; unit literal
  const double LonOrig = Deg2Rad((lnfc[rnf::eOLONG] + rnfc[rnf::eOLONG]) / 2);
  const double LatOrig = Deg2Rad((lnfc[rnf::eOLAT] + rnfc[rnf::eOLAT]) / 2);
  const Point3d origin(LonOrig, LatOrig, 0);

  auto earthEllip = std::make_unique<EarthEllipsoid>();
  earthEllip->SetLsrOrigin(origin);

  const double height{ (lnfc[rnf::eOHEIGHT] + rnfc[rnf::eOHEIGHT]) / 2 };

  const auto leftGsd = CalcGsd(leftRM, height);
  const auto righttGsd = CalcGsd(rightRM, height);
  const double invScale = sqrt(leftGsd * righttGsd) * sample;
  const double scale{ 1.0 / invScale };

  auto leftFp = CalcFootprint(leftRM, earthEllip.get(), height);
  auto rightFp = CalcFootprint(rightRM, earthEllip.get(), height);

  const double rotation = CalcEpiRotation(leftRM, rightRM,
    earthEllip.get(), height, leftFp, rightFp);
  const double cosRot{ cos(rotation) };
  const double sinRot{ sin(rotation) };

  auto [leftEpiMin, leftEpiMax] = CalcEpiBound(leftFp, scale, cosRot, sinRot);
  auto [rightEpiMin, rightEpiMax] = CalcEpiBound(rightFp, scale, cosRot, sinRot);

  const double offsetY{std::min(leftEpiMin.y, rightEpiMin.y)};
  const int numRows{ int(std::max(leftEpiMax.y, rightEpiMax.y) - offsetY + 0.5) };

  double leftOffsetX{};
  int leftNumCols{};
  double rightOffsetX{};
  int rightNumCols{};

  switch (mEpiOpt) {
  case EpiOpt::eCOMPACT:
    leftOffsetX = leftEpiMin.x;
    leftNumCols = int(leftEpiMax.x - leftOffsetX + 0.5);
    rightOffsetX = rightEpiMin.x;
    rightNumCols = int(rightEpiMax.x - rightOffsetX + 0.5);
    break;
  case EpiOpt::eFULL:
    leftOffsetX = rightOffsetX = std::min(leftEpiMin.x, rightEpiMin.x);
    leftNumCols = rightNumCols = int(std::max(leftEpiMax.x, rightEpiMax.x) - leftOffsetX + 0.5);
    break;
  case EpiOpt::eINTERSECT:  // \todo parallax buffer
    leftOffsetX = rightOffsetX = std::max(leftEpiMin.x, rightEpiMin.x);
    leftNumCols = rightNumCols = int(std::min(leftEpiMax.x, rightEpiMax.x) - leftOffsetX + 0.5);
    break;
  default:
    assert(false && "undefined epipolar option");
  }

  auto geoGround = std::make_shared<GeographicGroundDeg>(std::move(earthEllip));
  assert(!earthEllip);

  // left epipolar model
  auto leftRect = std::make_unique<EpipolarRect>(origin, height, 
    scale, rotation, Point2d(leftOffsetX, offsetY), leftNumCols, numRows);
  auto leftEpiModel = std::make_unique<GeoEpipolarModel>(
    leftModel, EpiStereoPos::eLEFT, std::move(leftRect), geoGround);

  // right epipolar model
  auto rightRect = std::make_unique<EpipolarRect>(origin, height,
    scale, rotation, Point2d(rightOffsetX, offsetY), rightNumCols, numRows);
  auto rightEpiModel = std::make_unique<GeoEpipolarModel>(
    rightModel, EpiStereoPos::eRIGHT, std::move(rightRect), geoGround);

  return EpiModelPair{ std::move(leftEpiModel), std::move(rightEpiModel) };
}

/**
 *  \brief find the rotation angle of the epipolar direction.
 *  In west-east direction, raster-up defaults to north;
 *  In north-south direction, raster-up defaults to east.
 *
 *  \return Return rotation angle of epipolar direction [rad]
 */
double EpipolarModelFactory::CalcEpiRotation(const ISensorModel* leftModel,
  const ISensorModel* rightModel, const EarthEllipsoid* ee, const double height, 
  const Polygon2d& leftFp, const Polygon2d& rightFp) const {
  std::vector<PointXYd> points;
  bool ret = bg::intersection(leftFp, rightFp, points);

  std::vector<Point2d> epiDir;  // epipolar direction vectors at corners
  std::vector<double> dirLen;  // length of direction vectors
  for (auto& point : points) {
    Point3d lsr{ point.x(), point.y(), height };
    auto lph = ee->LSRToLPH(lsr);

    Point2d img;
    leftModel->GroundToImage(lph, img);
    PointXYd leftRect;
    ImageToPlane(img, leftModel, ee, height - 10.0, leftRect, 0.01);  // [1cm]

    rightModel->GroundToImage(lph, img);
    PointXYd rightRect;
    ImageToPlane(img, rightModel, ee, height - 10.0, rightRect, 0.01);

    const Point2d dir{ rightRect.x() - leftRect.x(), rightRect.y() - leftRect.y() };
    epiDir.push_back(dir);
    dirLen.push_back(cv::norm(dir));
  }

  const auto index = std::distance(std::begin(dirLen),
    std::max_element(std::begin(dirLen), std::end(dirLen)));
  Point2d dir{ epiDir[index] };

  if ((fabs(dir.x) > fabs(dir.y) && 0 > dir.x) ||
    (fabs(dir.x) < fabs(dir.y) && 0 < dir.y)) {
    dir = -dir;
  }

  return atan2(dir.y, dir.x);
}

/**
 *  \return Returns the lower-left and upper-right epiolar bound [pixel].
 */
std::pair<Point2d, Point2d> EpipolarModelFactory::CalcEpiBound(
  const Polygon2d& fp, const double scale, 
  const double cosRot, const double sinRot) const {
  const std::vector<PointXYd>& fpPts = fp.outer();

  Point2d pixMin{ DBL_MAX, DBL_MAX };
  Point2d pixMax{ -DBL_MAX, -DBL_MAX };
  for (auto& p : fpPts) {
    Point2d pt{ cosRot * p.x() + sinRot * p.y(), 
                -sinRot * p.x() + cosRot * p.y() };
    pt *= scale;

    if (pixMin.x > pt.x) pixMin.x = pt.x;
    if (pixMin.y > pt.y) pixMin.y = pt.y;
    if (pixMax.x < pt.x) pixMax.x = pt.x;
    if (pixMax.y < pt.y) pixMax.y = pt.y;
  }

  return { cv::Point2d(floor(pixMin.x), floor(pixMin.y)),
           cv::Point2d(ceil(pixMax.x), ceil(pixMax.y)) };
}
