/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/epipolarrect.h"
#include "epipolarmodel/iepipolarmodel.h"
#include "3rdparty/boost/exboost.h"
#include "sensormodel/rationalmodel.h"

#include <array>
#include <utility>

namespace earthxpert {
class EarthEllipsoid;
using EpiModelPair = std::pair<std::unique_ptr<IEpipolarModel>, std::unique_ptr<IEpipolarModel>>;

/**
 *  The class EpipolarModelFactory defines the behaviours for creating
 *  epipolar models from image products with sensor models.
 *
 *  \remark This implementation requires that the stereo images are of the same sensro type.
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
  EpiModelPair CreateFrameEpiModel(std::shared_ptr<ISensorModel> leftModel,
      std::shared_ptr<ISensorModel> rightModel, int sample, double height);

  EpiModelPair CreateRationalEpiModel(std::shared_ptr<RationalModel> leftModel,
      std::shared_ptr<RationalModel> rightModel, int sample=1);

  /**
   *  \brief create epipolar models from a pair of stereo images.
   */
  EpiModelPair CreateRationalEpiModel(
    const std::array<std::shared_ptr<RationalModel>, 2> sensorModels, int sample=1) {
    return CreateRationalEpiModel(sensorModels[0], sensorModels[1], sample);
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
  static constexpr int kPosTfm{ 16 };   // position transformation to align
  static constexpr double kHgtTol{ 0.01 }; // Height tolerance [1cm]: good enough

  EpipolarOption mEpiOpt{ EpiOpt::eFULL };

  double EpipolarModelFactory::CalcEpiRotation(const ISensorModel* leftModel,
    const ISensorModel* rightModel, const EarthEllipsoid* ee, double height, 
    const Polygon2d& leftFp, const Polygon2d& rightFp) const;
  std::pair<Point2d, Point2d> CalcEpiBound(const Polygon2d& fp,
    double scale, double cosRot, double sinRot) const;

  // cached
};

using FacEpiOpt = EpipolarModelFactory::EpiOpt;
} // namespace earthxpert
