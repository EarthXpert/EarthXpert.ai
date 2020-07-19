/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/utils/stereomodel.h"
#include "core/utils//typehelper.h"

namespace earthxpert {
class ISensorModel;
class RationalModel;

/**
 *  The class RationalStereoModel implements the dedicated methods having logic
 *  specific to rational sensor systems for better perforamnce. In detail, 
 *  the bundle of rays from a block of sensor geometry models are used to implement
 *  forward intersection and backward projection in the network.
 */
class EXSENSORMODEL_EXPORTS RationalStereoModel final : public StereoModel {
public:
  /**
   *  \brief Form a stereo model by a pair of sensor models.
   *
   *  \param [in] leftModel left sensor model composing the stereo model
   *  \param [in] rightModel right sensor model composing the stereo model
   */
  RationalStereoModel(RationalModel const* leftModel, RationalModel const* rightModel);

  /**
   *  \brief Form a triplet stereo model by three sensor models.
   *
   *  \param [in] sensorModels triplet sensor models composing the stereo model
   */
  //RationalStereoModel(const std::array<RationalModel const*, 3>& sensorModels) {}

  /**
   *  \brief form a stereo model by the given two or more sensor models.
   *
   *  \param [in] sensorModels multiple (two or more) sensor models composing
   * the stereo model.
   */
  //RationalStereoModel(const std::vector<RationalModel const*>& sensorModels) {}

  /**
   *  Image-to-ground backward projection (fixed).
   *
   *  \brief Given a pair of sensor models and associated 2d positions
   *  in image coordinate system, do forward intersection of pair-rays, and
   *  output the ground position with respect to the ground coordinate system.
   *
   *  \param [in] leftImage 2d position (x, y) in left image raster coordinate
   * system
   *  \param [in] rightImage the corresponding 2d position (x, y) in right image
   * raster coordinate system
   *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate
   * system
   *  \return Returns true if the operation succeeds
   */
  bool ImageToGround(const Point2d& leftImage,
    const Point2d& rightImage, Point3d& ground) const override;

  /**
   *  \brief Given a triplet of sensor models and associated 2d positions
   *  in image coordinate system, do forward intersection of triple-rays, and
   *  output the ground position with respect to the ground coordinate system.
   *
   *  \param [in] image the corresponding 2d positions (x, y) in imagery raster
   * coordinate systems
   *  \param [in/out] ground 3d ground position (X, Y, Z) in
   * ground coordinate system \return Returns true if the operation succeeds
   *  \return Returns true if the operation succeeds
   */
  bool ImageToGround(const std::array<Point2d, 3>& images, Point3d& ground) const override;

  /**
   *  \brief Given two or more sensor models and associated 2d positions
   *  in image coordinate system, do forward intersection of rays, and
   *  output the ground position with respect to the ground coordinate system.
   *
   *  \param [in] images the corresponding 2d positions (x, y) in
   * imagery raster coordinate systems, interleaved by sensor models.
   *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate system
   *  \return Returns true if the operation succeeds
   */
  bool RationalStereoModel::ImageToGround(
    const std::vector<Point2d>& images, Point3d& ground) const override;

  /**
   *  Image-to-ground forward projection (variable).
   *
   *  \brief Given a variable number of sensor models and associated 2d positions
   *  in image coordinate system, do forward intersection of rays, and
   *  output the ground position with respect to the ground coordinate system.
   *
   *  \param [in] sensorModels an array of sensor models.
   *  \param [in] images the corresponding 2d positions (x, y) in
   * imagery raster coordinate systems, interleaved by sensor models. \param
   * [in/out] ground 3d ground position (X, Y, Z) in ground coordinate system
   *  \return Returns true if the operation succeeds
   */
  bool ImageToGround(const std::vector<const ISensorModel*>& sensorModels,
    const std::vector<Point2d>& images, Point3d& ground) const override {
    return false;
  }

private:
  static constexpr double kTolD = 1.0e-9; // degree ~= [0.1mm]
  static constexpr double kTolH = 0.0001; // [0.1mm]

  void Initialize();

  Tuple6d CalcJacobian(RationalModel const* ratModel, 
    const Point3d& ground, Point2d& image) const;
    
  // cached for convenience/performance
  std::vector<const RationalModel*> mRatModels;   // one model per ray
};
}
