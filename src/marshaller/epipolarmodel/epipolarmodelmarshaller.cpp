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
using cvFS = cv::FileStorage;

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

  cvFS fs(epiMdlFile, cvFS::WRITE);
  fs << "origin" << Point3d(Rad2Deg(lr.mOrigin.x), Rad2Deg(lr.mOrigin.y), lr.mHeight) 
     << "scale" << lr.mScale << "rotation" << Rad2Deg(lr.mRotation) 
     << "leftRawSize" << cv::Size2i(leftNumCols, leftNumRows) 
     << "leftMdlFile" << leftMdlFile << "leftStereoPos" << lem->mPos 
     << "leftBound" << lss.str() << "leftOffset" << lr.mOffset 
     << "leftEpiSize" << cv::Size2i(lr.mNumCols, lr.mNumRows)
     << "rightRawSize" << cv::Size2i(rightNumCols, rightNumRows)
     << "rightMdlFile" << rightMdlFile << "rightStereoPos" << rem->mPos 
     << "rightBound" << rss.str() << "rightOffset" << rr.mOffset 
     << "rightEpiSize" << cv::Size2i(rr.mNumCols, rr.mNumRows);

  //auto& lee = *(lem->mGroundPolicy->GetEarthEllipsoid());

  return true;
}

EpiModelPair
EpipolarModelMarshaller::DeserializeFS(const std::string& epiMdlFile) const {
  cvFS fs(epiMdlFile, cvFS::READ);

  return EpiModelPair{};
}
