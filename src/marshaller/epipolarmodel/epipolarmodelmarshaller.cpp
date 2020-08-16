/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "marshaller/epipolarmodelmarshaller.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/exceptions/exexcept.h"
#include "epipolarmodel/epipolarmodel.h"

using namespace earthxpert;
using cvFS = cv::FileStorage;

/**
 *  \brief serialize an epipolarmodel into a json file
 *
 *  \param [in] modelFile name of the file to be serialized to
 *  \param [in] epiModel the epipolar model to be serialized
 *
 */
bool EpipolarModelMarshaller::Serialize(const std::string &modelFile, 
    IEpipolarModel const *const epiModel) const {
  switch (mMLib) {
  case eFileStorage:
    return SerializeFS(modelFile, epiModel);

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
std::unique_ptr<IEpipolarModel>
EpipolarModelMarshaller::Deserialize(const std::string &modelFile) const {
  switch (mMLib) {
  case eFileStorage:
    return DeserializeFS(modelFile);

  default:
    return std::unique_ptr<IEpipolarModel>();
  }
}

bool EpipolarModelMarshaller::SerializeFS(const std::string& modelFile,
  IEpipolarModel const* epiModel) const {
  auto em = dynamic_cast<GeoEpipolarModel const*>(epiModel);
  if (!em) {
    throw std::invalid_argument(_("GeoEpipolarModel expected"));
  }

  cvFS fs(modelFile, cvFS::WRITE);

  auto& rt = *(em->mEpiRect);
  fs << "origin" << rt.mOrigin << "height" << rt.mHeight << "scale" << rt.mScale 
     << "rotation" << rt.mRotation << "offset" << rt.mOffset 
     << "numCols" << rt.mNumCols << "numRows" << rt.mNumRows;

  //auto& ee = *(em->mGroundPolicy->GetEarthEllipsoid());

  return true;
}

std::unique_ptr<IEpipolarModel>
EpipolarModelMarshaller::DeserializeFS(const std::string& modelFile) const {
  return std::unique_ptr<IEpipolarModel>();
}
