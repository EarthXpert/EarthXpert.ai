/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "epipolarmodel/epipolarstereofactory.h"
#include "core/utils/typehelper.h"
#include "core/exceptions/exexcept.h"
#include "core/math/mutils.h"
#include "core/utils/geomutils.h"
#include "epipolarmodel/epipolarmodel.h"
#include "sensormodel/framemodel.h"
#include "sensormodel/rationalmodel.h"
#include "sensormodel/utils/smutils.h"

#include <algorithm>
#include <deque>
#include <numeric>

using namespace earthxpert;

// output option
static const std::map<std::string, FacEpiOpt> sOutOptMap = {
  {"compact", FacEpiOpt::eCOMPACT},
  {"full", FacEpiOpt::eFULL},
  {"intersect", FacEpiOpt::eINTERSECT} };

/**
 *  \brief
 */
EpipolarStereoFactory::EpipolarStereoFactory(const EpiOutputOption outOpt)
  : mOutOpt(outOpt) {}

EpipolarStereoFactory::EpipolarStereoFactory(const std::string& outOpt)
  : EpipolarStereoFactory(sOutOptMap.at(outOpt)) {}

/**
 *  \brief Create an epipolar models for a stereo pair of frame images.
 *
 *  \param [in] leftModel sensor model associated with the left image
 *  \param [in] rightModel sensor model associated with the right image
 *  \param [in] height the optional average height used for epipolar rectficiation
 *  \return Returns pointers to epipolar model objects.
 */
EpiModelPair EpipolarStereoFactory::CreateFrameEpiStereo(
  std::shared_ptr<ISensorModel> leftModel,
  std::shared_ptr<ISensorModel> rightModel,
  const int sample, const double height) {
  if (leftModel->GetModelType() != rightModel->GetModelType())
    throw std::exception(_("The stereo pair should be of the same sensor type."));
  return { nullptr,nullptr };
}

/**
 *  \brief Create an epipolar models for a stereo pair of RPC images.
 *
 *  \param [in] leftModel sensor model associated with the left image
 *  \param [in] rightModel sensor model associated with the right image
 *  \return Returns pointer to epipolar model objects.
 */
EpiModelPair EpipolarStereoFactory::CreateRationalEpiStereo(
  std::shared_ptr<RationalModel> leftModel,
  std::shared_ptr<RationalModel> rightModel, const int sample) {
  if (leftModel->GetModelType() != rightModel->GetModelType())
    throw std::exception(_("The stereo pair should be of the same sensor type."));
  auto lnfc = leftModel->GetNormalFactors();
  auto rnfc = rightModel->GetNormalFactors();

  // \todo across anti-meridian; unit literal
  const double LonOrig = Deg2Rad((lnfc[rnf::eOLON] + rnfc[rnf::eOLON]) / 2);
  const double LatOrig = Deg2Rad((lnfc[rnf::eOLAT] + rnfc[rnf::eOLAT]) / 2);
  const Point3d origin(LonOrig, LatOrig, 0);

  auto earthEllip = std::make_unique<EarthEllipsoid>();
  earthEllip->SetLsrOrigin(origin);

  auto EpiGsd = [](const RationalModel* leftModel,
    const RationalModel* rightModel, const double height) {
      const auto leftGsd = CalcGsd(leftModel, height);
      const auto righttGsd = CalcGsd(rightModel, height);
      return sqrt(leftGsd * righttGsd);
  };

  // \todo determine the rectification height so that the average
  // resolution of epipolar is 1:1 before considering the sample
  double height{ (lnfc[rnf::eOHGT] + rnfc[rnf::eOHGT]) / 2 };

  const double invScale = EpiGsd(leftModel.get(), rightModel.get(), height) * sample;
  const double scale{ 1.0 / invScale };

  // epipolar footprint/boundary in metric unit [meter]
  auto leftFp = CalcFootprint(leftModel.get(), earthEllip.get(), height);
  auto rightFp = CalcFootprint(rightModel.get(), earthEllip.get(), height);

  const double rotation = CalcEpiRotation(leftModel.get(), rightModel.get(),
    earthEllip.get(), height, leftFp, rightFp);
  const double cosRot{ cos(rotation) };
  const double sinRot{ sin(rotation) };

  // convert footprint in epipolar unit [pixel]
  ConvertEpiRect(leftFp, scale, cosRot, sinRot);
  ConvertEpiRect(rightFp, scale, cosRot, sinRot);

  // bounding box in epipolar pixel
  auto [leftEpiMin, leftEpiMax] = utils::CalcBound(leftFp, kPosTfm);
  auto [rightEpiMin, rightEpiMax] = utils::CalcBound(rightFp, kPosTfm);

  const double offsetY{ std::min(leftEpiMin.y, rightEpiMin.y) };
  const int numRows{ int(std::max(leftEpiMax.y, rightEpiMax.y) - offsetY + 0.5) };
  if (1 >= numRows) 
    throw EpipolarError(_("CreateRationalEpiStereo: not valid"));

  double leftOffsetX{}, rightOffsetX{};
  int leftNumCols{}, rightNumCols{};

  switch (mOutOpt) {
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

  if (1 >= leftNumCols || 1 >= rightNumCols) 
    throw EpipolarError(_("CreateRationalEpiStereo: not valid"));

  // convert to epipolar boundary in image space based on epipolar options.
  ConvertEpiImage(leftFp, leftOffsetX, offsetY, numRows);
  ConvertEpiImage(rightFp, rightOffsetX, offsetY, numRows);

  auto geoGround = std::make_shared<GeographicGroundDeg>(std::move(earthEllip));
  assert(!earthEllip);

  // left epipolar model
  auto leftRect = std::make_unique<EpipolarRect>(origin, height,
    scale, rotation, Point2d(leftOffsetX, offsetY), leftNumCols, numRows);
  auto leftEpiModel = std::make_unique<GeoEpipolarModel>(EpiStereoPos::eLEFT,
    leftModel, leftFp, std::move(leftRect), geoGround);

  // right epipolar model
  auto rightRect = std::make_unique<EpipolarRect>(origin, height,
    scale, rotation, Point2d(rightOffsetX, offsetY), rightNumCols, numRows);
  auto rightEpiModel = std::make_unique<GeoEpipolarModel>(EpiStereoPos::eRIGHT,
    rightModel, rightFp, std::move(rightRect), geoGround);

  return EpiModelPair{ std::move(leftEpiModel), std::move(rightEpiModel) };
}

/**
 *  \brief Create an epipolar models for a stereo pair of images.
 *
 *  \param [in] leftModel sensor model associated with the left image
 *  \param [in] rightModel sensor model associated with the right image
 *  \return Returns pointers to epipolar model objects.
 */
EpiModelPair EpipolarStereoFactory::CreateSensorEpiStereo(
  std::shared_ptr<ISensorModel> leftModel,
  std::shared_ptr<ISensorModel> rightModel, const int) {
  if (leftModel->GetModelType() != rightModel->GetModelType())
    throw std::exception(_("The stereo pair should be of the same sensor type."));

  auto [numCols, numRows] = leftModel->GetImageSize();
  auto [rightNumCols, rightNumRows] = leftModel->GetImageSize();
  if (numCols != rightNumCols || numRows != rightNumRows)
    throw std::invalid_argument(_("CreateSensorEpiStereo: sensor models are not compatible"));

  std::vector<PointXYd> fpCorners{ {0.0, 0.0}, {double(numCols), 0.0}, 
    {double(numCols), double(numRows)}, {0.0, double(numRows)}, {0.0, 0.0} };

  Polygon2d leftFp;
  bg::assign_points(leftFp, fpCorners);
  auto leftEpiModel = std::make_unique<SensorEpipolarModel>(
    EpiStereoPos::eLEFT, leftModel, leftFp);

  Polygon2d rightFp;
  bg::assign_points(rightFp, fpCorners);
  auto rightEpiModel = std::make_unique<SensorEpipolarModel>(
    EpiStereoPos::eRIGHT, rightModel, rightFp);

  return EpiModelPair{ std::move(leftEpiModel), std::move(rightEpiModel) };
}

/**
 *  \brief
 */
std::vector<std::unique_ptr<IEpipolarModel>>
EpipolarStereoFactory::CreateEpipolarStereo(
  const std::vector<std::shared_ptr<ISensorModel>> &sensorModels,
  const int sample, const double height) {
  if (sensorModels.size() < 2)
    throw std::exception(_("Require a minimum of two images"));

  std::vector<std::unique_ptr<IEpipolarModel>> epiModels;
  return epiModels;
}

/**
 *  \brief find the rotation angle of the epipolar direction.
 *  In west-east direction, raster-up defaults to north;
 *  In north-south direction, raster-up defaults to east.
 *
 *  \return Return rotation angle of epipolar direction [rad]
 */
double EpipolarStereoFactory::CalcEpiRotation(const ISensorModel* leftModel,
  const ISensorModel* rightModel, const EarthEllipsoid* ee, const double height,
  const Polygon2d& leftFp, const Polygon2d& rightFp) const {
  std::vector<PointXYd> points;
  bool ret = bg::intersection(leftFp, rightFp, points);
  if (!ret) {
    PointXYd lc, rc;
    bg::centroid(leftFp, lc);
    bg::centroid(rightFp, rc);
    points.emplace_back((lc.x() + rc.x()) / 2, (lc.y() + rc.y()) / 2);
  }

  std::vector<Point2d> epiDir;  // epipolar direction vectors at corners
  for (auto& point : points) {
    Point3d lsr{ point.x(), point.y(), height };
    auto lph = Rad2Deg(ee->LSRToLPH(lsr));

    Point2d img;
    leftModel->GroundToImage(lph, img);
    PointXYd leftRect;
    ImageToPlane(img, leftModel, ee, height - 10.0, leftRect, 0.01);  // [1cm]

    rightModel->GroundToImage(lph, img);
    PointXYd rightRect;
    ImageToPlane(img, rightModel, ee, height - 10.0, rightRect, 0.01);

    const Point2d dir{ rightRect.x() - leftRect.x(), rightRect.y() - leftRect.y() };
    epiDir.push_back(dir);
  }

  // \todo plot all directions. select median/average of the three longest vectors?
  Point2d ed;

  switch (mEpiDir) {
  case eLONGEST: {// select the longest vector as direction having largest parallax
    auto it = std::max_element(epiDir.cbegin(), epiDir.cend(),
      [](const Point2d& da, const Point2d& db) {
        return cv::norm(da) < cv::norm(db); });
    ed = epiDir[std::distance(epiDir.cbegin(), it)];
  }
  break;

  case eOVERALL: {// sum all direction vectors
    ed = std::accumulate(epiDir.cbegin(), epiDir.cend(), Point2d());
  }
  break;

  default:
    assert(false);
  }

  if ((fabs(ed.x) > fabs(ed.y) && 0 > ed.x) ||
    (fabs(ed.x) < fabs(ed.y) && 0 < ed.y)) {
    ed = -ed;
  }

  return atan2(ed.y, ed.x);
}

/**
 *  \brief convert vertex coordinates from rectification CS [meter]
 *  to epipolar CS [pixel].
 */
void EpipolarStereoFactory::ConvertEpiRect(Polygon2d& fp,
  const double scale, const double cosRot, const double sinRot) const {
  std::vector<PointXYd>& fpPts = fp.outer();

  for (auto& p : fpPts) {
    Point2d pt{ cosRot * p.x() + sinRot * p.y(),
                -sinRot * p.x() + cosRot * p.y() };
    pt *= scale;
    p.x(pt.x);
    p.y(pt.y);
  }
}

/**
 *  \brief convert rectified footprint to epipolar boundary in image space.
 *  \todo return Returns Polygon2f
 */
void EpipolarStereoFactory::ConvertEpiImage(Polygon2d& fp,
  const double offsetX, const double offsetY, const int numRows) const {
  std::vector<PointXYd>& fpPts = fp.outer();

  for (auto& p : fpPts) {
    p.x(p.x() - offsetX - 0.5);
    p.y(offsetY + numRows - p.y() - 0.5);
  }
}
