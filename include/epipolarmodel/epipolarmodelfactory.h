/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/epipolarrect.h"
#include "epipolarmodel/iepipolarmodel.h"
#include "3rdparty/boost/exboost.h"
#include "sensormodel/isensormodel.h"

#include <array>
#include <utility>

namespace earthxpert {
class EarthEllipsoid;
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
    eCOMPACT,  // the smallest width for each epipolar image
    eFULL,     // the largest width for a union of epipolar images
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
  static constexpr int kMaxIters{ 5 };
  static constexpr double kHgtTol{ 0.01 }; // Height tolerance [1cm]: good enough

  EpipolarOption mEpiOpt{ EpiOpt::eFULL };

  EpiModelPair CreateFrameEpiModel(std::shared_ptr<ISensorModel> leftModel,
      std::shared_ptr<ISensorModel> rightModel, int sample, double height);
  EpiModelPair CreateRationalEpiModel(std::shared_ptr<ISensorModel> leftModel,
      std::shared_ptr<ISensorModel> rightModel, int sample);

  double EpipolarModelFactory::CalcEpiRotation(const ISensorModel* leftModel,
    const ISensorModel* rightModel, const EarthEllipsoid* ee, double height, 
    const Polygon2d& leftFp, const Polygon2d& rightFp) const;
  std::pair<Point2d, Point2d> CalcEpiBound(const Polygon2d& fp,
    double scale, double cosRot, double sinRot) const;

  // cached
  SensorType mSensorType{ SensorType::eUNDEFINED };
};

using FacEpiOpt = EpipolarModelFactory::EpiOpt;
} // namespace earthxpert
