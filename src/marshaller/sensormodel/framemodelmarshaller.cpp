/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "marshaller/sensormodel/framemodelmarshaller.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/stringutils.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace earthxpert;
using utils::CompareI;

/**
 *  \brief read from a bingo directory into a framemodel
 *  \param[in] modelFileIP the model file about intrinsic parameters
 *  \param[in] modelFileEP the model file about extrinsic parameters
 *
 *  \return the list of all the available photo id in bingo
 */
std::vector<std::string>
FrameModelMarshaller::Deserialize(const std::string &modelFileIP,
                                  const std::string &modelFileEP) {
  if (CompareI(modelFileIP.substr(modelFileIP.length() - 9),
                          "geoin.dat")) {
    ReadGeoin(modelFileIP);
  } 
  else {
    throw;
  }

  if (CompareI(modelFileEP.substr(modelFileIP.length() - 9),
                          "itera.dat")) {
    ReadItera(modelFileEP);
  }
  else if (CompareI(modelFileEP.substr(modelFileIP.length() - 11),
                                 "gps-imu.dat")) {
  } 
  else {
    throw;
  }

  // get all the keys stored in cache
  std::vector<std::string> keys;
  keys.reserve(mCacheEP.size());
  for (auto k : mCacheEP) {
    keys.push_back(k.first);
  }
  return keys;
}

/**
 *  \brief get model corrisponding to the photoID
 *  \param[in] photoID for the photo id
 *
 *  \return the framemodel for the id
 */
UPISensorModel
FrameModelMarshaller::GetModel(const std::string &photoID) {
  auto iterator = mCacheEP.find(photoID);
  if (iterator != mCacheEP.end()) {
    // spitialite system
    auto groundSRS = std::make_shared<OGRSpatialRef>();

    // build frameep from cache
    auto frameep = std::make_unique<FrameEP>(
        iterator->second.position, iterator->second.rotation, groundSRS);

    // build default image model
    DigitalImageModel digitalimagemodel(std::make_shared<DigitalImageModelImp>(1, 1, 1.0, 1.0));

    return std::make_unique<DigitalFrameModel>(std::move(digitalimagemodel),
                                               mCacheIP, std::move(frameep));
  }
  // throw while id not exist, add execption handling later
  throw;
}

/**
 *  \breif read from itera.dat file and storing the ep info to cache
 *  \param[in] modelFile model file name
 */
void FrameModelMarshaller::ReadItera(const std::string &modelFile) {
  std::ifstream ifs(modelFile, std::ifstream::binary);

  // parameters to read
  std::string photoID;
  std::array<double, 3> position;
  std::array<double, 3> rotation;

  for (std::string line; std::getline(ifs, line);) {
    std::istringstream iss(line);

    std::string entryName;
    iss >> entryName;

    if (entryName == "ORIT") {
      iss >> photoID;
      iss >> position[0] >> position[1] >> position[2];
      iss >> rotation[0] >> rotation[1] >> rotation[2];

      mCacheEP[photoID] = {photoID, position, rotation};
    }
  }
}

/**
 *  \breif read from geoin.dat into a frameIP object
 *  \param[in] modelFile model file name
 */
void FrameModelMarshaller::ReadGeoin(const std::string &modelFile) {
  std::ifstream ifs(modelFile, std::ifstream::binary);

  std::string camName;
  double focalLength = 0;
  std::array<double, 2> ppa = {0, 0};

  for (std::string line; std::getline(ifs, line);) {
    std::istringstream iss(line);

    std::string entryName;
    iss >> entryName;

    if (entryName == "CAPA") {
      // Camera name
      camName = line.substr(9, 13);

      // focalLength
      if (line.substr(23, 10).find_first_not_of(' ') != std::string::npos) {
        focalLength = std::stod(line.substr(23, 10));
      }

      // PPA
      if (line.substr(33, 10).find_first_not_of(' ') != std::string::npos &&
          line.substr(43, 10).find_first_not_of(' ') != std::string::npos) {
        ppa = {std::stod(line.substr(33, 10)), std::stod(line.substr(43, 10))};
      }
    }
  }
  mCacheIP = std::make_shared<FrameIP>(camName, focalLength, ppa);
}

/**
 *  \brief read from gps-imu.dat
 *  \param[in] modelFile model file name
 */
void FrameModelMarshaller::ReadGpsImu(const std::string &modelFile) {
  std::ifstream ifs(modelFile, std::ifstream::binary);

  // parameters to read
  std::string photoID;
  std::array<double, 3> position;
  std::array<double, 3> rotation;

  for (std::string line; std::getline(ifs, line);) {
    std::istringstream iss(line);
    std::string entryName;
    iss >> entryName;

    if (line.front() == ' ' && entryName != "LINE" && entryName != "#") {
      photoID = entryName;
      iss >> position[0] >> position[1] >> position[2];
      iss >> rotation[0] >> rotation[1] >> rotation[2];

      mCacheEP[photoID] = {photoID, position, rotation};
    }
  }
}
