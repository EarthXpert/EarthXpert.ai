/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "marshaller/epipolarmodelmarshaller.h"
#include "epipolarmodel/epipolarmodel.h"

#include <cereal/archives/json.hpp>
#include <cereal/types/array.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace earthxpert;

/**
 *  \brief serialize an epipolarmodel into a json file
 *
 *  \param [in] modelFile name of the file to be serialized to
 *  \param [in] epiModel the epipolar model to be serialized
 *
 */
bool EpipolarModelMarshaller::Serialize(
    const std::string &modelFile, IEpipolarModel const *const epiModel) const {
  /*
  auto epiCenter = epiModel->GetEpiOrigin();

  EpipolarRectMarshaller leftRect(
      epiModel->GetEpiRect(EpipolarModel::eLEFT));
  EpipolarRectMarshaller rightRect(
      epiModel->GetEpiRect(EpipolarModel::eRIGHT));

  // cast point2d into array
  std::array<double, 2> center{epiCenter.x, epiCenter.y};

  // create write stream
  std::ofstream ofs(modelFile);
  cereal::JSONOutputArchive archive(ofs);

  archive(CEREAL_NVP(center), CEREAL_NVP(leftRect), CEREAL_NVP(rightRect));*/
  return true;
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
  /*
  std::ifstream ifs(modelFile);
  cereal::JSONInputArchive archive(ifs);

  std::array<double, 2> center;
  EpipolarRectMarshaller leftRect;
  EpipolarRectMarshaller rightRect;

  archive(center, leftRect, rightRect);

  Point2d castCenter = Point2d(center[0], center[1]);
  */
  // todo: get sensormodel to create epipolar model

  return std::unique_ptr<IEpipolarModel>();
}
