/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/epipolarrect.h"
#include "epipolarmodel/iepipolarmodel.h"
#include "sensormodel/isensormodel.h"

#include <array>

namespace earthxpert {
using SenType = ISensorModel::ModelType;

/**
 *  The class EpipolarModelFactory defines the behaviours for creating
 *  epipolar model from image products with sensor models.
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
   *  \brief create epipolar model from a pair of stereo images.
   */
  std::unique_ptr<IEpipolarModel>
  CreateEpipolarModel(std::shared_ptr<ISensorModel> leftModel,
    std::shared_ptr<ISensorModel> rightModel, int sample, double height=0);

  /**
   *  \brief create epipolar model from a pair of stereo images.
   */
  std::unique_ptr<IEpipolarModel> CreateEpipolarModel(
    const std::array<std::shared_ptr<ISensorModel>, 2> sensorModels,
    int sample, double height=0);

  /**
   *  \brief create epipolar model from 2+ stereo images.
   */
  std::unique_ptr<IEpipolarModel> CreateEpipolarModel(
    const std::vector<std::shared_ptr<ISensorModel>> &sensorModels,
    int sample, double height=0);

  void SetEpipolarOption(const EpipolarOption epiOpt) { mEpiOpt = epiOpt; }

private:
  EpipolarOption mEpiOpt{ EpiOpt::eFULL };

  std::unique_ptr<IEpipolarModel>
    CreateFrameEpiModel(std::shared_ptr<ISensorModel> leftModel,
      std::shared_ptr<ISensorModel> rightModel, int sample, double height);
  std::unique_ptr<IEpipolarModel>
    CreateRationalEpiModel(std::shared_ptr<ISensorModel> leftModel,
      std::shared_ptr<ISensorModel> rightModel, int sample);

  // cached
  SenType mSensorType{ SenType::eUNDEFINED };
};
} // namespace earthxpert
