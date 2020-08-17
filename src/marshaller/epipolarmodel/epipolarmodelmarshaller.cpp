/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "marshaller/epipolarmodelmarshaller.h"
#include "3rdparty/boost/exboost.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/exceptions/exexcept.h"
#include "epipolarmodel/epipolarmodel.h"
#include "marshaller/rationalmodelmarshaller.h"

#include <sstream>

using namespace earthxpert;
using CvFS = cv::FileStorage;
using CvFN = cv::FileNode;

// ingested persistence
struct EpiModelRecord {
  EpiStereoPos epiPos{ EpiStereoPos::eIGNORE };
  cv::Size2i rawSize;
  std::string mdlFile;
  std::string bound;
  Point2d offset;
  cv::Size2i epiSize;

  void write(CvFS& fs) const {
    fs << "{" << "epiPos" << epiPos << "rawSize" << rawSize << "mdlFile" << mdlFile 
      << "bound" << bound << "offset" << offset << "epiSize" << epiSize << "}";
  }

  void read(const cv::FileNode& node) {
    node["epiPos"] >> epiPos;
    node["rawSize"] >> rawSize;
    node["mdlFile"] >> mdlFile;
    node["bound"] >> bound;
    node["offset"] >> offset;
    node["epiSize"] >> epiSize;
  }
};

static inline void write(CvFS& fs, const std::string&, const EpiModelRecord& r) {
  r.write(fs);
}

static inline void read(const CvFN& node, EpiModelRecord& r, 
  const EpiModelRecord& default_value = EpiModelRecord()) {
  if (node.empty())
    r = default_value;
  else
    r.read(node);
}

/**
 *  \brief serialize an epipolarmodel into a json file
 *
 *  \param [in] modelFile name of the file to be serialized to
 *  \param [in] epiModel the epipolar model to be serialized
 *
 */
bool EpipolarModelMarshaller::Serialize(const std::string& epiMdlFile,
  const std::string& leftMdlFile, IEpipolarModel const* leftEpiModel,
  const std::string& rightMdlFile, IEpipolarModel const* rightEpiModel) const {
  switch (mMLib) {
  case eFileStorage:
    return SerializeFS(epiMdlFile, leftMdlFile, leftEpiModel, rightMdlFile, rightEpiModel);

  default:
    return false;
  }
}

/**
 *  \brief deserialize a json file into an epipolarmodel
 *
 *  \param [in] modelFile name of the file to be deserialized
 *
 *  \return the pointer to the epipolar model
 */
EpiModelPair
EpipolarModelMarshaller::Deserialize(const std::string &modelFile) const {
  switch (mMLib) {
  case eFileStorage:
    return DeserializeFS(modelFile);

  default:
    throw std::invalid_argument(_("Deserialize"));
  }
}

bool EpipolarModelMarshaller::SerializeFS(const std::string& epiMdlFile,
  const std::string& leftMdlFile, IEpipolarModel const* leftEpiModel,
  const std::string& rightMdlFile, IEpipolarModel const* rightEpiModel) const {
  auto lem = dynamic_cast<GeoEpipolarModel const*>(leftEpiModel);
  if (!lem) {
    throw std::invalid_argument(_("GeoEpipolarModel expected"));
  }
  auto& lr = *(lem->mEpiRect);
  auto [leftNumCols, leftNumRows] = lem->GetRawImageSize();
  std::stringstream lss;
  lss << bg::wkt(lem->mBound);

  auto rem = dynamic_cast<GeoEpipolarModel const*>(rightEpiModel);
  if (!rem) {
    throw std::invalid_argument(_("GeoEpipolarModel expected"));
  }
  auto& rr = *(rem->mEpiRect);
  auto [rightNumCols, rightNumRows] = rem->GetRawImageSize();
  std::stringstream rss;
  rss << bg::wkt(rem->mBound);

  CvFS fs(epiMdlFile, CvFS::WRITE);
  if (!fs.isOpened()) {
    throw std::invalid_argument(_("SerializeFS: failed to open for write"));
  }

  std::vector<EpiModelRecord> epiModels{ 
    {lem->mPos, cv::Size2i(leftNumCols, leftNumRows), leftMdlFile,
     lss.str(), lr.mOffset, cv::Size2i(lr.mNumCols, lr.mNumRows)}, 
    {rem->mPos, cv::Size2i(rightNumCols, rightNumRows), rightMdlFile,
     rss.str(), rr.mOffset, cv::Size2i(rr.mNumCols, rr.mNumRows)} };

  fs << "origin" << Point3d(Rad2Deg(lr.mOrigin.x), Rad2Deg(lr.mOrigin.y), lr.mHeight)
    << "scale" << lr.mScale << "rotation" << Rad2Deg(lr.mRotation)
    << "EpiStereo" << epiModels;

  //auto& lee = *(lem->mGroundPolicy->GetEarthEllipsoid());

  return true;
}

/**
 *  \todo relative path
 */
EpiModelPair
EpipolarModelMarshaller::DeserializeFS(const std::string& epiMdlFile) const {
  CvFS fs(epiMdlFile, CvFS::READ);
  if (!fs.isOpened()) {
    throw std::invalid_argument(_("DeserializeFS: failed to open for read"));
  }

  Point3d origin;
  fs["origin"] >> origin;
  double height = origin.z;
  origin.x = Deg2Rad(origin.x);
  origin.y = Deg2Rad(origin.y);
  origin.z = 0.0;

  double scale{}, rotation{};
  fs["scale"] >> scale;
  fs["rotation"] >> rotation;
  rotation = Deg2Rad(rotation);

  std::vector<EpiModelRecord> ems;
  fs["EpiStereo"] >> ems;
  assert(2 == ems.size());

  RationalModelMarshaller rmMarshaller;

  auto earthEllip = std::make_unique<EarthEllipsoid>();
  earthEllip->SetLsrOrigin(origin);
  auto geoGround = std::make_shared<GeographicGroundDeg>(std::move(earthEllip));
  assert(!earthEllip);

  // left epipolar model
  rmMarshaller.SetImageSize(ems[0].rawSize.width, ems[0].rawSize.height);
  std::shared_ptr<RationalModel> leftModel = rmMarshaller.Deserialize(ems[0].mdlFile);
  assert(SensorType::eRATIONAL == leftModel->GetModelType());

  Polygon2d leftBound;
  bg::read_wkt(ems[0].bound, leftBound);

  auto leftRect = std::make_unique<EpipolarRect>(origin, height,
    scale, rotation, ems[0].offset, ems[0].epiSize.width, ems[0].epiSize.height);
  auto leftEpiModel = std::make_unique<GeoEpipolarModel>(ems[0].epiPos,
    leftModel, leftBound, std::move(leftRect), geoGround);

  // right epipolar model
  rmMarshaller.SetImageSize(ems[1].rawSize.width, ems[1].rawSize.height);
  std::shared_ptr<RationalModel> rightModel = rmMarshaller.Deserialize(ems[1].mdlFile);
  assert(SensorType::eRATIONAL == rightModel->GetModelType());

  Polygon2d righftBound;
  bg::read_wkt(ems[1].bound, righftBound);

  auto rightRect = std::make_unique<EpipolarRect>(origin, height,
    scale, rotation, ems[1].offset, ems[1].epiSize.width, ems[1].epiSize.height);
  auto rightEpiModel = std::make_unique<GeoEpipolarModel>(ems[1].epiPos, 
    rightModel, righftBound, std::move(rightRect), geoGround);

  return EpiModelPair{ std::move(leftEpiModel), std::move(rightEpiModel) };
}
