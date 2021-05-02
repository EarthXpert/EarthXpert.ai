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

/**
 *  The class EpipolarStereoFactory defines the behaviours for creating
 *  epipolar stereo composed of image products with sensor models.
 *
 *  \remark This implementation requires that the stereo images are of the same sensro type.
 */
class EXEPIPOLARMODEL_EXPORTS EpipolarStereoFactory {
public:
  // the epipolar images always have equal number of rows in height and
  // perhaps different columns in width. This flag specifies the output option
  // of epipolar width. With eFULL option, the average parallax in X direction
  // will be minimized; while with eCOMPACT option, the average X-parallax has
  // to be calculated for have the epipolar images aligned approximately.
  enum struct EpiOutputOption {
    eEO_COMPACT,   // the smallest width for each epipolar image
    eEO_FULL,      // the largest width for a union of epipolar images
    eEO_INTERSECT, // the smallest width for intersection of epipolar images
    eEO_SENSOR     // raw images as epipolar without rectification
  };
  using EpiOpt = EpiOutputOption;

  // direction of largest parallax
  enum EpiDirection {
    eED_LONGEST, // select the longest vector
    eED_OVERALL  // sum all direction vectors
  };

  EpipolarStereoFactory(EpiOutputOption outOpt);
  EpipolarStereoFactory(const std::string& outOpt);
  ~EpipolarStereoFactory() = default;

  /**
   *  \brief create epipolar models from a pair of stereo images.
   */
  EpiModelPair CreateFrameEpiStereo(std::shared_ptr<ISensorModel> leftModel,
      std::shared_ptr<ISensorModel> rightModel, int sample, double height);

  EpiModelPair CreateRationalEpiStereo(std::shared_ptr<RationalModel> leftModel,
    std::shared_ptr<RationalModel> rightModel,
    int sample = 1, float epiHgt = 0, float rotAngle = 0);

  EpiModelPair CreateSensorEpiStereo(std::shared_ptr<ISensorModel> leftModel,
    std::shared_ptr<ISensorModel> rightModel, int sample = 1);

  /**
   *  \brief create epipolar models from a pair of stereo images.
   */
  EpiModelPair CreateRationalEpiStereo(
    const std::array<std::shared_ptr<RationalModel>, 2> sensorModels, int sample=1) {
    return CreateRationalEpiStereo(sensorModels[0], sensorModels[1], sample);
  }

  /**
   *  \brief create epipolar models from 2+ stereo images.
   */
  std::vector<std::unique_ptr<IEpipolarModel>> CreateEpipolarStereo(
    const std::vector<std::shared_ptr<ISensorModel>> &sensorModels,
    int sample, double height=0);

  void SetEpipolarOption(const EpiOutputOption epiOpt) { mOutOpt = epiOpt; }

private:
  static constexpr int kMaxIters{ 5 };
  static constexpr int kPosTfm{ 16 };   // position transformation to align
  static constexpr double kHgtTol{ 0.01 }; // Height tolerance [1cm]: good enough

  EpiOutputOption mOutOpt{ EpiOpt::eEO_FULL };
  EpiDirection mEpiDir{ eED_OVERALL };

  double EpipolarStereoFactory::CalcEpiRotation(const ISensorModel* leftModel,
    const ISensorModel* rightModel, const EarthEllipsoid* ee, double height,
    const Polygon2d& leftFp, const Polygon2d& rightFp) const;
  void ConvertEpiRect(Polygon2d& fp, double scale,
    double cosRot, double sinRot) const;
  void ConvertEpiImage(Polygon2d& fp, double offsetX,
    double offsetY, int numRows) const;

  // cached
};

using FacEpiOpt = EpipolarStereoFactory::EpiOpt;
} // namespace earthxpert
