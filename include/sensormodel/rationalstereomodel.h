/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/utils/stereomodel.h"
#include "core/utils//typehelper.h"
#include "sensormodel/rationalmodel.h"

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
   *  \param [in] leftImage 2d position (x, y) in left image raster CS
   *  \param [in] rightImage the corresponding 2d position (x, y) in right image
   * raster coordinate system
   *  \param [in/out] ground 3d position (X, Y, Z) in ground coordinate system
   *  \return Returns true if the operation succeeds
   */
  bool ImagesToGround(const Point2d& leftImage,
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
  bool ImagesToGround(const std::array<Point2d, 3>& images, Point3d& ground) const override {
    assert(3 == mRatModels.size());
    std::vector<Point2d> imgs(images.cbegin(), images.cend());

    return ImagesToGround(imgs, ground);
  }

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
  bool RationalStereoModel::ImagesToGround(
    const std::vector<Point2d>& images, Point3d& ground) const override {
    return ImagesToGround(mRatModels, images, ground);
  }

  /**
   *  Image-to-ground forward projection (variable).
   *
   *  \brief Given a variable number of sensor models and associated 2d positions
   *  in image coordinate system, do forward intersection of rays, and
   *  output the ground position with respect to the ground coordinate system.
   *
   *  \param [in] sensorModels an array of sensor models.
   *  \param [in] images the corresponding 2d positions (x, y) in
   * imagery raster coordinate systems, interleaved by sensor models. 
   *  \param [in/out] ground 3d position (X, Y, Z) in ground coordinate system
   *  \return Returns true if the operation succeeds
   */
  bool ImagesToGround(const std::vector<const ISensorModel*>& sensorModels,
    const std::vector<Point2d>& images, Point3d& ground) const override {
    std::vector<const RationalModel*> ratModels;
    for (auto sm : sensorModels) {
      ratModels.push_back(dynamic_cast<const RationalModel*>(sm));
    }

    return ImagesToGround(ratModels, images, ground);
  }

  bool ImagesToGround(const std::vector<const RationalModel*>& ratModels,
    const std::vector<Point2d>& images, Point3d& ground) const;

private:
  void Initialize();

  Tuple6d CalcJacobianA(RationalModel const* ratModel,
    const Point3d& ground, Point2d& image) const;
  Tuple6d CalcJacobianNf(RationalModel const* ratModel,
    const Point3d& ground, Point2d& image) const;
  Tuple6d CalcJacobianNc(RationalModel const* ratModel,
    const Point3d& ground, Point2d& image) const;

  template<PartialType pT = ePT_ANADIFF>
    Tuple6d CalcJacobian(RationalModel const* ratModel,
      const Point3d& ground, Point2d& image) const {
      if constexpr (ePT_ANADIFF == pT) {
        return CalcJacobianA(ratModel, ground, image);
      }
      else if constexpr (ePT_NUMDIFF == pT) {
        return CalcJacobianNc(ratModel, ground, image);
      }
      else {
        throw std::invalid_argument("PartialType");
      }
  }

  // cached for convenience/performance
  std::vector<const RationalModel*> mRatModels;   // one model per ray
};

using UPRationalStereoModel = std::unique_ptr<RationalStereoModel>;
using SPRationalStereoModel = std::shared_ptr<RationalStereoModel>;
}
