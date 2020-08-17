/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/iepipolarmodelmarshaller.h"

#include <memory>

namespace earthxpert {

/**
 *  The class EpipolarStereoMarshaller serialize and deserialize the
 *  epipolar model and links to associated sensor models.
 */
class EXMARSHALLER_EXPORTS EpipolarStereoMarshaller final
    : public IEpipolarStereoMarshaller {
public:
  EpipolarStereoMarshaller(MarshallLib mLib= eFileStorage) : mMLib(mLib) {}
  ~EpipolarStereoMarshaller() = default;

  /**
   *  \brief serialize an epipolar model into a json file
   *
   *  \param [in] epiMdlFile name of the file to be serialized to
   *  \param [in] epiModel the epipolar model to be serialized
   */
  bool Serialize(const std::string& epiMdlFile, 
    const std::vector<EpiModelInfo>& epiModels) const override;

  /**
   *  \brief deserialize a json file into an epipolar model
   *
   *  \param [in] epiMdlFile name of the file to be deserialized
   *  \return the pointer to the epipolar model
   */
  std::vector<UPIEpipolarModel> Deserialize(const std::string & epiMdlFile) const override;

private:
  MarshallLib mMLib{ eFileStorage };

  bool SerializeFS(const std::string& epiMdlFile, const std::vector<EpiModelInfo>& epiModels) const;
  std::vector<UPIEpipolarModel> DeserializeFS(const std::string& epiMdlFile) const;

  bool CheckEpiStereo(const std::vector<EpiModelInfo>& epiModels) const;
  bool CheckEpiStereo(const std::vector<UPIEpipolarModel>& epiModels) const;
};
} // namespace earthxpert
