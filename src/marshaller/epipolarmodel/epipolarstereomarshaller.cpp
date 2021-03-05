/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "marshaller/epipolarstereomarshaller.h"
#include "3rdparty/boost/exboost.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/exceptions/exexcept.h"
#include "epipolarmodel/epipolarmodel.h"
#include "marshaller/sensormodel/rationalmodelmarshaller.h"

#include <sstream>

using namespace earthxpert;
using CvFS = cv::FileStorage;
using CvFN = cv::FileNode;

namespace {
  // ingested persistence
  struct EpiModelRecord { // EpipolarModel
    EpiStereoPos epiPos{ EpiStereoPos::eIGNORE };
    cv::Size2i rawSize;
    std::string mdlFile;
    std::string bound;
    Point2d offset;
    cv::Size2i epiSize;
    std::string epiFile;

    void write(CvFS& fs) const {
      fs << "{" << "epiPos" << epiPos << "rawSize" << rawSize
        << "mdlFile" << mdlFile << "bound" << bound << "offset" << offset
        << "epiSize" << epiSize << "epiFile" << epiFile << "}";
    }

    void read(const cv::FileNode& node) {
      node["epiPos"] >> epiPos;
      node["rawSize"] >> rawSize;
      node["mdlFile"] >> mdlFile;
      node["bound"] >> bound;
      node["offset"] >> offset;
      node["epiSize"] >> epiSize;
      node["epiFile"] >> epiFile;
    }
  };

  inline void write(CvFS& fs, const std::string&, const EpiModelRecord& r) {
    r.write(fs);
  }

  inline void read(const CvFN& node, EpiModelRecord& r,
    const EpiModelRecord& default_value = EpiModelRecord()) {
    if (node.empty())
      r = default_value;
    else
      r.read(node);
  }

  struct SensorEpiModelRecord { // SensorEpipolarModel
    EpiStereoPos epiPos{ EpiStereoPos::eIGNORE };
    std::string mdlFile;
    std::string bound;
    cv::Size2i size;   // epi/raw Size
    std::string file;  // epi/raw File
  };
}

/**
 *  \brief serialize an epipolar model into a json file
 *
 *  \param [in] modelFile name of the file to be serialized to
 *  \param [in] epiModel the epipolar model to be serialized
 */
bool EpipolarStereoMarshaller::Serialize(const std::string& epiMdlFile,
  const std::vector<EpiModelInfo>& epiModels) const {
  CheckEpiStereo(epiModels);

  switch (mMLib) {
  case eFileStorage:
    return SerializeFS(epiMdlFile, epiModels);
  default:
    return false;
  }
}

/**
 *  \brief deserialize a json file into an epipolar model
 *
 *  \param [in] modelFile name of the file to be deserialized
 *
 *  \return the pointer to the epipolar model
 */
std::vector<UPIEpipolarModel>
EpipolarStereoMarshaller::Deserialize(const std::string &modelFile) const {
  switch (mMLib) {
  case eFileStorage:
    return DeserializeFS(modelFile);

  default:
    throw std::invalid_argument(_("Deserialize"));
  }
}

/**
 *  \todo relative path
 */
bool EpipolarStereoMarshaller::SerializeFS(const std::string& epiMdlFile,
  const std::vector<EpiModelInfo>& epiModels) const {
  CheckEpiStereo(epiModels);

  std::optional<Point3d> origin;
  double scale{}, rotation{};
  std::vector<EpiModelRecord> epiMdlRecs;

  for (const auto& em : epiModels) {
    auto epiMdl = dynamic_cast<GeoEpipolarModel const*>(em.epiModel);
    if (!epiMdl) 
      throw std::invalid_argument(_("GeoEpipolarModel expected"));

    auto& epiRect = *(epiMdl->mEpiRect);
    if (!origin) {
      origin = Point3d(Rad2Deg(epiRect.mOrigin.x), Rad2Deg(epiRect.mOrigin.y), epiRect.mHeight);
      scale = epiRect.mScale;
      rotation = Rad2Deg(epiRect.mRotation);
    }

    auto [rawNumCols, rawNumRows] = epiMdl->GetRawImageSize();

    std::stringstream ss;
    ss << bg::wkt(epiMdl->mBound);

    epiMdlRecs.push_back(EpiModelRecord{ epiMdl->mPos,
      cv::Size2i(rawNumCols, rawNumRows),
      em.sensorMdlFile, ss.str(), epiRect.mOffset,
      cv::Size2i(epiRect.mNumCols, epiRect.mNumRows), em.epiImgFile });
  }

  CvFS fs(epiMdlFile, CvFS::WRITE);
  if (!fs.isOpened()) 
    throw std::invalid_argument(_("SerializeFS: failed to open for write"));

  fs << "signature" << "exai.epipolar" << "version" << std::vector<int>{1, 0, 0}
    << "origin" << origin.value() << "scale" << scale << "rotation" << rotation
    << "epiStereo" << epiMdlRecs;

  //auto& lee = *(lem->mGroundPolicy->GetEarthEllipsoid());

  return true;
}

std::vector<UPIEpipolarModel> EpipolarStereoMarshaller::DeserializeFS(
  const std::string& epiMdlFile) const {
  CvFS fs(epiMdlFile, CvFS::READ);
  if (!fs.isOpened()) 
    throw std::invalid_argument(_("DeserializeFS: failed to open for read"));

  std::string signature;
  fs["signature"] >> signature;
  std::vector<int> version;
  fs["version"] >> version;

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

  std::vector<EpiModelRecord> epiMdlRecs;
  fs["epiStereo"] >> epiMdlRecs;

  auto earthEllip = std::make_unique<EarthEllipsoid>();
  earthEllip->SetLsrOrigin(origin);
  auto geoGround = std::make_shared<GeographicGroundDeg>(std::move(earthEllip));
  assert(!earthEllip);

  RationalModelMarshaller rmMarshaller;
  std::vector<UPIEpipolarModel> epiModels;

  for (const auto& em : epiMdlRecs) {
    rmMarshaller.SetImageSize(em.rawSize.width, em.rawSize.height);
    std::shared_ptr<RationalModel> sensorMdl = rmMarshaller.Deserialize(em.mdlFile);
    assert(SensorType::eMT_RATIONAL == sensorMdl->GetModelType());

    Polygon2d bound;
    bg::read_wkt(em.bound, bound);

    auto epiRect = std::make_unique<EpipolarRect>(origin, height,
      scale, rotation, em.offset, em.epiSize.width, em.epiSize.height);
    epiModels.emplace_back(std::make_unique<GeoEpipolarModel>(em.epiPos,
      sensorMdl, bound, std::move(epiRect), geoGround));
  }

  CheckEpiStereo(epiModels);
  return epiModels;
}

bool EpipolarStereoMarshaller::CheckEpiStereo(
  const std::vector<EpiModelInfo>& epiModels) const {

  return true;
}

bool EpipolarStereoMarshaller::CheckEpiStereo(
  const std::vector<UPIEpipolarModel>& epiModels) const {
  return true;
}
