/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/exmarshaller.h"
#include "sensormodel/framemodel.h"

#include <unordered_map>

namespace earthxpert {
class ISensorModel;

struct EpData {
  std::string photoID;
  std::array<double, 3> position{};
  std::array<double, 3> rotation{};
};

/**
 *  The class FrameModelMarshaller serialize and deserialize frame models.
 */
class EXMARSHALLER_EXPORTS FrameModelMarshaller {
public:
  ~FrameModelMarshaller() = default;

  /**
   *  \brief read from a bingo directory into a framemodel
   *  \param[in] modelFileIP the model file about intrinsic parameters
   *  \param[in] modelFileEP the model file about extrinsic parameters
   *
   *  \return the list of all the available photo id in bingo
   */
  std::vector<std::string> Deserialize(const std::string &modelFileIP,
                                       const std::string &modelFileEP);

  /**
   *  \brief get model corrisponding to the photoID
   *  \param[in] photoID for the photo id
   *
   *  \return the framemodel for the id
   */
  std::unique_ptr<ISensorModel> GetModel(const std::string &photoID);

private:
  std::unordered_map<std::string, EpData>
      mCacheEP;                      // cache for storing previously read EP
  std::shared_ptr<FrameIP> mCacheIP; // cache for storing read IP

  /**
   *  \brief read from itera.dat file and storing the ep info to cache
   *  \param[in] modelFile model file name
   */
  void ReadItera(const std::string &modelFile);

  /**
   *  \brief read from geoin.dat into a frameIP object
   *  \param[in] modelFile model file name
   */
  void ReadGeoin(const std::string &modelFile);

  /**
   *  \brief read from gps-imu.dat
   *  \param[in] modelFile model file name
   */
  void ReadGpsImu(const std::string &modelFile);
};
} // namespace earthxpert
