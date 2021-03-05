/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/utils/smutils.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/layoututils.h"
#include "core/utils/unitutils.h"
#include "core/spatialref/earthellipsoid.h"

#include <cmath>
#include <map>
#include <sstream>

namespace earthxpert {
namespace detail {
  constexpr int kMaxIters = 5;
  constexpr int kNN = 5;  // #nodes
  constexpr size_t kNP = 9;  // #key points

  // sensor names & types
  std::map<std::string, SensorType> sSensorN2T = {
    {"undefined", SensorType::eUNDEFINED},
    {"ads", SensorType::eMT_ADS},
    {"frame", SensorType::eMT_FRAME},
    {"pushbroom", SensorType::eMT_PUSHBROOM},
    {"rolling", SensorType::eMT_ROLLING},
    {"affine", SensorType::eMT_AFFINE},
    {"network", SensorType::eMT_NETWORK},
    {"rational", SensorType::eMT_RATIONAL} };

  std::map<SensorType, std::string> sSensorT2N = {
    {SensorType::eUNDEFINED, "undefined"},
    {SensorType::eMT_ADS, "ads"},
    {SensorType::eMT_FRAME, "frame"},
    {SensorType::eMT_PUSHBROOM, "pushbroom"},
    {SensorType::eMT_ROLLING, "rolling"},
    {SensorType::eMT_AFFINE, "affine"},
    {SensorType::eMT_NETWORK, "network"},
    {SensorType::eMT_RATIONAL, "rational"} };
}

/**
 *  \brief Convert sensor name to type
 *
 *  \param [in] name sensor name
 *  \return Returns sensor type
 */
SensorType SensorNameToType(const std::string& name) {
  SensorType type{ SensorType::eUNDEFINED };
  const auto it = detail::sSensorN2T.find(name);
  if (detail::sSensorN2T.end() != it) {
    type = it->second;
  }

  return type;
}

/**
 *  \brief Convert sensor type to name
 *
 *  \param [in] type sensor type
 *  \return Returns sensor name
 */
std::string SensorTypeToName(const SensorType type) {
  std::string name{ "undefined" };
  const auto it = detail::sSensorT2N.find(type);
  if (detail::sSensorT2N.end() != it) {
    name = it->second;
  }

  return name;
}

/**
 *  \brief estimate GSD at the given height
 *
 *  \param [in] sensorModel
 *  \param [in] height the height in model CS
 *  \return Retruns GSD at image center
 */
double CalcGsd(const ISensorModel* sensorModel, const double height) {
  auto [numCols, numRows] = sensorModel->GetImageSize();
  const auto intCols = numCols / detail::kNN;
  const auto intRows = numRows / detail::kNN;
  const auto startCol = (numCols - intCols * detail::kNN) / 2;
  const auto startRow = (numRows - intRows * detail::kNN) / 2;

  double gsd{};
  int count{};
  for (int c = startCol; c <= numCols; c+=intCols)
    for (int r = startRow; r <= numRows; r += intRows) {
      gsd += CalcGsd(sensorModel, Point2d(c, r), height);
      count++;
    }

  return (gsd / count);
}

/**
 *  \brief estimate GSD at the given location and the given height
 *
 *  \param [in] sensorModel
 *  \param [in] ip image location where GSD is to be estimated
 *  \param [in] height the height in model CS
 *  \return Retruns GSD at image center
 */
double CalcGsd(const ISensorModel* sensorModel,
  const Point2d& ip, const double height) {
  Point3d gc{ 0.0, 0.0, height };
  sensorModel->ImageToGround(ip, gc);

  Point3d gc2{ 0.0, 0.0, height };
  sensorModel->ImageToGround(ip + Point2d(1.0, 1.0), gc2);

  double gsd = cv::norm(gc2 - gc) * M_SQRT1_2;
  if (eST_GEO == sensorModel->GetSrsType()) {
    gsd = kWGS84_A * Deg2Rad(gsd);
  }
  return gsd;
}

/**
 *  \brief estimate GSD at the given height
 *
 *  \param [in] sensorModel
 *  \param [in] ee the LSR
 *  \param [in] height the height in LSR
 *  \return Retruns GSD at image center
 */
double CalcGsd(const ISensorModel* sensorModel, 
  const EarthEllipsoid* ee, const double height) {
  const auto [numCols, numRows] = sensorModel->GetImageSize();
  const auto intCols = numCols / detail::kNN;
  const auto intRows = numRows / detail::kNN;
  const auto startCol = (numCols - intCols * detail::kNN) / 2;
  const auto startRow = (numRows - intRows * detail::kNN) / 2;

  double gsd{};
  int count{};
  for (int c = startCol; c <= numCols; c += intCols)
    for (int r = startRow; r <= numRows; r += intRows) {
      gsd += CalcGsd(sensorModel, Point2d(c, r), ee, height);
      count++;
    }

  return (gsd / count);
}

/**
 *  \brief estimate GSD at the given location and the given height
 *
 *  \param [in] sensorModel
 *  \param [in] ip image location where GSD is to be estimated
 *  \param [in] ee the LSR
 *  \param [in] height the height in LSR
 *  \return Retruns GSD at image center
 */
double CalcGsd(const ISensorModel* sensorModel, const Point2d& ip,
  const EarthEllipsoid* ee, const double height) {
  PointXYd rc, rc2;
  ImageToPlane(ip, sensorModel, ee, height, rc);
  ImageToPlane(ip + Point2d(1.0, 1.0), sensorModel, ee, height, rc2);

  return cv::norm(Point2d(rc2.x() - rc.x(), rc2.y() - rc.y())) * M_SQRT1_2;
}

/**
 *  \brief register multispectral and pan images in raw imagery space by
 *  calculating a low-order polynomial.
 *
 *  \details 1) geometric co-registration: establish a sparse grid on pan image; 
 *  forward project grid nodes to several elevation layers in ground; 
 *  backward project ground points to obtain points on mul image; 
 *  scale coordinates of pan grid nodes, dividing by the known pixel size ratio,
 *  and calculate the low-oder polynomial used to transform the projected mul points
 *  to the scaled grid nodes; append the transformation polynomial to
 *  the mul image positions to correct for and get mul-pan registered positions.
 *
 *  2) image co-registration: do image matching for the pan grid nodes w.r.t 
 *  the mul image to obtain the equivelant projected mul points.
 */
void RegisterMulPan(const ISensorModel* mulModel,
  const ISensorModel* panModel, const int ratio) {

}

/**
 *  Image-to-plane projection on ground.
 *
 *  \param [out] rect position on the tangential plane [meter].
 *  \param [in] tolH tolerance for convergence, default to 0.1mm.
 */
bool ImageToPlane(const Point2d& image, const ISensorModel* sensorModel, 
  const EarthEllipsoid* ee, const double height, PointXYd& rect, const double tolH) {
  assert(eST_GEO == sensorModel->GetSrsType() && "must be geographic");
  Point3d ground{ 0, 0, height };

  Point3d rect3{ 0, 0, 0 };
  for (int iter = 0; iter < detail::kMaxIters; ++iter) {
    if (!sensorModel->ImageToGround(image, ground))
      return false;

    rect3 = ee->LPHToLSR(Deg2Rad(ground));
    const double dz = height - rect3.z;  // dz >= 0
    ground.z += dz;

    if (tolH > std::fabs(dz)) break;
  }

  rect.x(rect3.x);
  rect.y(rect3.y);
  return true;
}

/**
 *  \return Returns the scene footprint on the rectification plane [meter].
 */
Polygon2d CalcFootprint(const ISensorModel* sensorModel,
  const EarthEllipsoid* ee, const double height) {
  assert(eST_GEO == sensorModel->GetSrsType());
  auto [numCols, numRows] = sensorModel->GetImageSize();
  std::vector<Point2d> corners;
  FillCorners(numCols, numRows, corners);

  std::vector<PointXYd> fpCorners(corners.size());
  for (size_t i = 0; i < corners.size(); ++i) {
    ImageToPlane(corners[i], sensorModel, ee, height, fpCorners[i], 0.01);
  }
  fpCorners.push_back(fpCorners[0]);  // close polygon
  assert(detail::kNP == fpCorners.size());

  Polygon2d poly;
  bg::assign_points(poly, fpCorners);
  return poly;
}

/**
 *  \return Returns the scene footprint on the model plane.
 */
Polygon2d CalcFootprint(const ISensorModel* sensorModel, const double height) {
  auto [numCols, numRows] = sensorModel->GetImageSize();
  std::vector<Point2d> corners;
  FillCorners(numCols, numRows, corners);

  std::vector<PointXYd> fpCorners;

  Point3d ground{ 0.0, 0.0, height };
  for (const auto corner : corners) {
    sensorModel->ImageToGround(corner, ground);
    fpCorners.emplace_back(ground.x, ground.y);
  }
  fpCorners.push_back(fpCorners[0]);  // close polygon
  assert(detail::kNP == fpCorners.size());

  Polygon2d fp;
  bg::assign_points(fp, fpCorners);
  return fp;
}

std::string ToString(const RationalModel* ratModel) {
  std::stringstream ss;
  return ss.str();
}
}
