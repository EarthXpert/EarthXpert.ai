/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/exmarshaller.h"
#include "marshaller/marshalltypes.h"
#include "epipolarmodel/epipolarmodel.h"

namespace earthxpert {
struct EpiModelInfo {
  std::string sensorMdlFile;    // path to sensor model file
  std::string epiImgFile;       // path to the epipolar image file
  std::string epiRpcFile;       // path to the epipolar image RPC file
  IEpipolarModel const* epiModel{}; // raw pointer to epipolar model object
};

/**
 *  The class EpipolarStereoMarshaller serialize and deserialize the
 *  epipolar model and links to associated sensor models.
 */
class EXMARSHALLER_EXPORTS EpipolarStereoMarshaller {
public:
  explicit EpipolarStereoMarshaller(MarshallLib mLib = eFileStorage) : mMLib(mLib) {}
  ~EpipolarStereoMarshaller() = default;

  /**
   *  \brief serialize an epipolar model into a json file
   *
   *  \param [in] epiMdlFile name of the file to be serialized to
   *  \param [in] epiModel the epipolar model to be serialized
   */
  bool Serialize(const std::string& epiMdlFile, 
    const std::vector<EpiModelInfo>& epiModels) const;

  /**
   *  \brief deserialize a json file into an epipolar model
   *
   *  \param [in] epiMdlFile name of the file to be deserialized
   *  \return the pointer to the epipolar model
   */
  std::vector<UPIEpipolarModel> Deserialize(const std::string & epiMdlFile) const;

private:
  MarshallLib mMLib{ eFileStorage };

  bool SerializeFS(const std::string& epiMdlFile, const std::vector<EpiModelInfo>& epiModels) const;
  std::vector<UPIEpipolarModel> DeserializeFS(const std::string& epiMdlFile) const;

  bool CheckEpiStereo(const std::vector<EpiModelInfo>& epiModels) const;
  bool CheckEpiStereo(const std::vector<UPIEpipolarModel>& epiModels) const;
};

using UPEpipolarStereoMarshaller = std::unique_ptr<EpipolarStereoMarshaller>;
using SPEpipolarStereoMarshaller = std::shared_ptr<EpipolarStereoMarshaller>;
} // namespace earthxpert
