/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/exmarshaller.h"
#include "marshaller/marshalltypes.h"
#include "epipolarmodel/epipolarmodel.h"

#include <memory>

namespace earthxpert {
struct EpiModelInfo {
  std::string sensorMdlFile;    // path to sensor model file
  std::string epiImgFile;       // path to the epipolar image file
  IEpipolarModel const* epiModel{}; // raw pointer to epipolar model object
};

/**
 *  The class EpipolarModelMarshaller serialize and deserialize the
 *  epipolar model and links to associated sensor models.
 */
class EXMARSHALLER_EXPORTS IEpipolarStereoMarshaller {
public:
  ~IEpipolarStereoMarshaller() = default;

  /**
   *  \brief seralize an epipolar model into a txt file
   */
  virtual bool Serialize(const std::string& epiMdlFile, 
    const std::vector<EpiModelInfo>& epiModels) const = 0;

  /**
   *  \brief deserialize epipolar model from a txt file
   */
  virtual std::vector<UPIEpipolarModel> Deserialize(const std::string &modelFile) const = 0;
};
} // namespace earthxpert
