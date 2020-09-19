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
  static constexpr int kMaxIters = 5;
  static constexpr size_t kNP = 9;

  // sensor names & types
  static std::map<std::string, SensorType> sSensorN2T = {
  {"undefined", SensorType::eUNDEFINED},
  {"ads", SensorType::eADS},
  {"frame", SensorType::eFRAME},
  {"pushbroom", SensorType::ePUSHBROOM},
  {"rolling", SensorType::eROLLING},
  {"affine", SensorType::eAFFINE},
  {"network", SensorType::eNETWORK},
  {"rational", SensorType::eRATIONAL} };

  static std::map<SensorType, std::string> sSensorT2N = {
  {SensorType::eUNDEFINED, "undefined"},
  {SensorType::eADS, "ads"},
  {SensorType::eFRAME, "frame"},
  {SensorType::ePUSHBROOM, "pushbroom"},
  {SensorType::eROLLING, "rolling"},
  {SensorType::eAFFINE, "affine"},
  {SensorType::eNETWORK, "network"},
  {SensorType::eRATIONAL, "rational"} };
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
 *  \param [in] height
 *  \return Retruns GSD at image center
 */
double CalcGsd(const ISensorModel* sensorModel, const double height) {
  auto [numCols, numRows] = sensorModel->GetImageSize();
  const Point2d ic{ numCols * 0.5, numRows * 0.5 };

  // image center at height
  Point3d gc{ 0.0, 0.0, height };
  sensorModel->ImageToGround(ic, gc);

  Point3d gc2{ 0.0, 0.0, height };
  sensorModel->ImageToGround(ic + Point2d(1.0, 1.0), gc2);

  double gsd = cv::norm(gc2 - gc) * M_SQRT1_2;
  if (eGEO == sensorModel->GetSrsType()) {
    gsd = kWGS84_A * Deg2Rad(gsd);
  }
  return gsd;
}

double CalcGsd(const ISensorModel* sensorModel, 
  const EarthEllipsoid* ee, const double height) {
  auto [numCols, numRows] = sensorModel->GetImageSize();
  const Point2d ic{ numCols * 0.5, numRows * 0.5 };

  // image center at height
  PointXYd rc;
  ImageToPlane(ic, sensorModel, ee, height, rc);

  PointXYd rc2;
  ImageToPlane(ic + Point2d(1.0, 1.0), sensorModel, ee, height, rc);

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
  assert(eGEO == sensorModel->GetSrsType());
  Point3d ground{ 0, 0, height };

  Point3d rect3{ 0, 0, 0 };
  for (int i = 0; i < detail::kMaxIters; ++i) {
    if (!sensorModel->ImageToGround(image, ground))
      return false;

    rect3 = ee->LPHToLSR(Deg2Rad(ground));
    const double dz = height - rect3.z;
    ground.z += dz;

    if (tolH > dz) break;
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
  assert(eGEO == sensorModel->GetSrsType());
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
