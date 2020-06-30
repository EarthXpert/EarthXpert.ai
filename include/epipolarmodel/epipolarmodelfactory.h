/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/epipolarrect.h"
#include "epipolarmodel/iepipolarmodel.h"
#include "sensormodel/isensormodel.h"

#include <array>
#include <utility>

namespace earthxpert {
using EpiModelPair = std::pair<std::unique_ptr<IEpipolarModel>, std::unique_ptr<IEpipolarModel>>;

/**
 *  The class EpipolarModelFactory defines the behaviours for creating
 *  epipolar models from image products with sensor models.
 */
class EXEPIPOLARMODEL_EXPORTS EpipolarModelFactory {
public:
  // the epipolar images always have equal number of rows in height and
  // perhaps different columns in width. This flag specifies the option
  // of epipolar width. With eFULL option, the average parallax in X direction
  // will be minimized; while with eCOMPACT option, the average X-parallax has
  // to be calculated for have the epipolar images aligned approximately.
  enum class EpipolarOption {
    eFULL,     // the largest width for a union of epipolar images
    eCOMPACT,  // the smallest width for each epipolar image
    eINTERSECT // the smallest width for intersection of epipolar images
  };
  using EpiOpt = EpipolarOption;

  EpipolarModelFactory(EpipolarOption epiOpt);
  ~EpipolarModelFactory() = default;

  /**
   *  \brief create epipolar models from a pair of stereo images.
   */
  EpiModelPair CreateEpipolarModel(std::shared_ptr<ISensorModel> leftModel,
    std::shared_ptr<ISensorModel> rightModel, int sample, double height=0);

  /**
   *  \brief create epipolar models from a pair of stereo images.
   */
  EpiModelPair CreateEpipolarModel(
    const std::array<std::shared_ptr<ISensorModel>, 2> sensorModels,
    int sample, double height = 0) {
    return CreateEpipolarModel(sensorModels[0], sensorModels[1], sample, height);
  }

  /**
   *  \brief create epipolar models from 2+ stereo images.
   */
  std::vector<std::unique_ptr<IEpipolarModel>> CreateEpipolarModel(
    const std::vector<std::shared_ptr<ISensorModel>> &sensorModels,
    int sample, double height=0);

  void SetEpipolarOption(const EpipolarOption epiOpt) { mEpiOpt = epiOpt; }

private:
  EpipolarOption mEpiOpt{ EpiOpt::eFULL };

  EpiModelPair CreateFrameEpiModel(std::shared_ptr<ISensorModel> leftModel,
      std::shared_ptr<ISensorModel> rightModel, int sample, double height);
  EpiModelPair CreateRationalEpiModel(std::shared_ptr<ISensorModel> leftModel,
      std::shared_ptr<ISensorModel> rightModel, int sample);

  // cached
  SensorType mSensorType{ SensorType::eUNDEFINED };
};

using FacEpiOpt = EpipolarModelFactory::EpiOpt;
} // namespace earthxpert
