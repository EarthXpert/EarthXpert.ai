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

struct EpiModelRecord {
  EpiStereoPos epiPos{ EpiStereoPos::eIGNORE };
  cv::Size2i rawSize;
  std::string mdlFile;
  std::string fp;
  Point2d offset;
  cv::Size2i epiSize;
};

namespace cv {
static inline void write(CvFS& fs, const EpiModelRecord& rec) {
  write(fs, rec.epiPos);
  write(fs, rec.rawSize);
  write(fs, rec.mdlFile);
  write(fs, rec.fp);
  write(fs, rec.offset);
  write(fs, rec.epiSize);
}

cv::FileStorage& operator<<(CvFS& fs, const EpiModelRecord& rec) {
  fs << "{";
  write(fs, rec);
  fs << "}";
  return fs;
}

void operator>>(const cv::FileNode& node, EpiModelRecord& rec) {
  node["leftStereoPos"] >> rec.epiPos;
  node["leftRawSize"] >> rec.rawSize;
  node["leftMdlFile"] >> rec.mdlFile;
  node["leftBound"] >> rec.fp;
  node["leftOffset"] >> rec.offset;
  node["leftEpiSize"] >> rec.epiSize;
}
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

  std::vector<EpiModelRecord> recs{ 
    {lem->mPos, cv::Size2i(leftNumCols, leftNumRows), leftMdlFile,
     lss.str(), lr.mOffset, cv::Size2i(lr.mNumCols, lr.mNumRows)}, 
    {rem->mPos, cv::Size2i(rightNumCols, rightNumRows), rightMdlFile,
     rss.str(), rr.mOffset, cv::Size2i(rr.mNumCols, rr.mNumRows)} };

  fs << "origin" << Point3d(Rad2Deg(lr.mOrigin.x), Rad2Deg(lr.mOrigin.y), lr.mHeight)
    << "scale" << lr.mScale << "rotation" << Rad2Deg(lr.mRotation)
    << "leftStereoPos" << lem->mPos 
    << "leftRawSize" << cv::Size2i(leftNumCols, leftNumRows) 
    << "leftMdlFile" << leftMdlFile 
    << "leftBound" << lss.str() << "leftOffset" << lr.mOffset 
    << "leftEpiSize" << cv::Size2i(lr.mNumCols, lr.mNumRows)
    << "rightStereoPos" << rem->mPos 
    << "rightRawSize" << cv::Size2i(rightNumCols, rightNumRows)
    << "rightMdlFile" << rightMdlFile 
    << "rightBound" << rss.str() << "rightOffset" << rr.mOffset 
    << "rightEpiSize" << cv::Size2i(rr.mNumCols, rr.mNumRows);
    //<< "EpiStereo" << recs;

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
  RationalModelMarshaller rmMarshaller;

  EpiStereoPos leftPos{}, rightPos{};
  cv::Size2i leftRawSize, rightRawSize;
  std::string leftMdlFile, rightMdlFile;
  std::string leftFp, rightFp;
  Polygon2d leftBound, rightBound;
  Point2d leftOffset, rightOffset;
  cv::Size2i leftEpiSize, rightEpiSize;

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

  fs["leftStereoPos"] >> leftPos;
  fs["leftRawSize"] >> leftRawSize;
  fs["leftMdlFile"] >> leftMdlFile;
  rmMarshaller.SetImageSize(leftRawSize.width, leftRawSize.height);
  std::shared_ptr<RationalModel> leftModel = rmMarshaller.Deserialize(leftMdlFile);
  assert(SensorType::eRATIONAL == leftModel->GetModelType());

  fs["leftBound"] >> leftFp;
  bg::read_wkt(leftFp, leftBound);

  fs["leftOffset"] >> leftOffset;
  fs["leftEpiSize"] >> leftEpiSize;

  fs["rightStereoPos"] >> rightPos;
  fs["rightRawSize"] >> rightRawSize;
  fs["rightMdlFile"] >> rightMdlFile;
  rmMarshaller.SetImageSize(rightRawSize.width, rightRawSize.height);
  std::shared_ptr<RationalModel> rightModel = rmMarshaller.Deserialize(rightMdlFile);
  assert(SensorType::eRATIONAL == rightModel->GetModelType());

  fs["rightBound"] >> rightFp;
  bg::read_wkt(rightFp, rightBound);

  fs["rightOffset"] >> rightOffset;
  fs["rightEpiSize"] >> rightEpiSize;

  auto earthEllip = std::make_unique<EarthEllipsoid>();
  earthEllip->SetLsrOrigin(origin);
  auto geoGround = std::make_shared<GeographicGroundDeg>(std::move(earthEllip));
  assert(!earthEllip);

  // left epipolar model
  auto leftRect = std::make_unique<EpipolarRect>(origin, height,
    scale, rotation, leftOffset, leftEpiSize.width, leftEpiSize.height);
  auto leftEpiModel = std::make_unique<GeoEpipolarModel>(leftModel,
    leftPos, leftBound, std::move(leftRect), geoGround);

  // right epipolar model
  auto rightRect = std::make_unique<EpipolarRect>(origin, height,
    scale, rotation, rightOffset, rightEpiSize.width, rightEpiSize.height);
  auto rightEpiModel = std::make_unique<GeoEpipolarModel>(rightModel,
    rightPos, rightBound, std::move(rightRect), geoGround);

  return EpiModelPair{ std::move(leftEpiModel), std::move(rightEpiModel) };
}
