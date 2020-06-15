/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/istereomodel.h"
#include "core/math/iintersect.h"
#include "core/spatialref/earthellipsoid.h"
#include "core/exceptions/exexcept.h"

#include <cassert>

namespace earthxpert {
class ISensorModel;

/**
 *  The mixin class StereoModel implements the template methods having logic
 *  common to mulitple sensor systems. A concrete sensor model may override
 *  the template method for better perforamnce. In detail, the bundle of
 *  rays from a block of sensor geometry models are used to implement
 *    forward intersection and backward projection in the network.
 *
 *  \todo revist the design for variable number of rays, specifily, 
 *  changing freaquently for matched key points: anohter class VariableStereoModel? 
 */
class EXSENSORMODEL_EXPORTS StereoModel : public IStereoModel {
public:
  /**
   *  Constructor.
   *
   *  \brief Form a stereo model by a pair of sensor models.
   *
   *  \param [in] leftModel left sensor model composing the stereo model
   *  \param [in] rightModel right sensor model composing the stereo model
   */
  StereoModel(ISensorModel const* const leftModel, ISensorModel const* const rightModel) {
    mModels.resize(2);
    mModels[0] = leftModel;
    mModels[1] = rightModel;
    ObtainModelsInfo();
  }

  /**
   *  \brief Form a triplet stereo model by three sensor models.
   *
   *  \param [in] sensorModels triplet sensor models composing the stereo model
   */
  StereoModel(const std::array<ISensorModel const*, 3>& sensorModels) {
    mModels.resize(3);
    std::copy_n(std::begin(sensorModels), 3, std::begin(mModels));
    ObtainModelsInfo();
  }

  /**
   *  \brief form a stereo model by the given two or more sensor models.
   *
   *  \param [in] sensorModels multiple (two or more) sensor models composing
   * the stereo model.
   */
  StereoModel(const std::vector<ISensorModel const*>& sensorModels) {
    assert(2 <= sensorModels.size());
    mModels = sensorModels;
    ObtainModelsInfo();
  }

  StereoModel(const StereoModel&) = delete;
  StereoModel(StereoModel&&) = delete;
  virtual ~StereoModel() {}

  StereoModel& operator=(const StereoModel&) = delete;
  StereoModel& operator=(StereoModel&&) = delete;

  void SetEarthEllipsoid(std::shared_ptr<EarthEllipsoid> earthEllip);

  const auto& GetSensorModels() const noexcept { return mModels; }

  /**
   *  Image-to-ground forward intersection.
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
   * system \return Returns true if the operation succeeds
   *
   *  \details
   */
  virtual bool ImageToGround(const Point2d &leftImage,
    const Point2d &rightImage, Point3d &ground) const override;

  /**
   *  \brief Given a pair of sensor models and associated list of 2d positions
   *  in image coordinate system, do forward intersection of pair-rays, and
   *  output the list of ground positions with respect to the ground coordinate
   * system.
   *
   *  \param [in] leftImages a list of 2d positions (x, y) in left image raster
   * coordinate system 
   *  \param [in] rightImages a list of corresponding 2d position
   * (x, y) in right image raster coordinate system 
   *  \param [in/out] grounds a
   * list of 3d ground positions (X, Y, Z) in ground coordinate system
   */
  virtual size_t ImageToGround(const std::vector<Point2d> &leftImages,
    const std::vector<Point2d> &rightImages, std::vector<Point3d> &grounds) const override;

  /**
   *  \brief Given a triplet of sensor models and associated 2d positions
   *  in image coordinate system, do forward intersection of triple-rays, and
   *  output the ground position with respect to the ground coordinate system.
   *
   *  \param [in] image the corresponding 2d positions (x, y) in imagery raster
   * coordinate systems \param [in/out] ground 3d ground position (X, Y, Z) in
   * ground coordinate system \return Returns true if the operation succeeds
   */
  virtual bool
  ImageToGround(const std::array<Point2d, 3> &image, Point3d &ground) const override;

  /**
   *  \brief Given two or more sensor models and associated 2d positions
   *  in image coordinate system, do forward intersection of rays, and
   *  output the ground position with respect to the ground coordinate system.
   *
   *  \param [in] image the corresponding 2d positions (x, y) in
   * imagery raster coordinate systems, interleaved by sensor models. 
   *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate system
   *  \return Returns true if the operation succeeds
   *
   *  \details
   */
  virtual bool
  ImageToGround(const std::vector<Point2d> &image, Point3d &ground) const override;

  /**
   *  \brief Given two or more sensor models and associated list of 2d positions
   *  in image coordinate system, do forward intersection of rays, and
   *  output the ground positions with respect to the ground coordinate system.
   *
   *  \param [in] images List of corresponding 2d positions (x,
   * y) in imagery raster coordinate systems, interleaved by sensor models.
   *  \param [in/out] grounds List of 3d ground positions (X, Y, Z) in ground
   * coordinate system
   *
   *  \details
   */
  virtual size_t ImageToGround(const std::vector<Point2d> &image,
                std::vector<Point3d> &grounds) const override;

  /**
   *  Ground-to-image backward projection.
   *
   *  \brief Given a 3d ground position with reference to the ground coordinate
   * system and multiple sensor models, output the corresponding 2d sensor
   * positions in the image coordinate system for each sensor model.
   *
   *  \param [in] ground 3d position (X, Y, Z) in ground coordinate system
   *  \param [out] image 2d position (x, y) in sensor raster
   * coordinate system \return Returns true if operation succeeds
   *
   *  \details
   */
  virtual bool GroundToImage(const Point3d &ground,
                std::vector<Point2d> &image) const override;

  /**
   *  \brief Given a list of 3d positions in ground coordinate system and
   *  multiple sensor models, output a list of 2d sensor positions in image
   * coordinate system.
   *
   *  \param [in] ground List of 3d ground positions (X, Y, Z)
   *  \param [in/out] images List of 2d positions (x, y) on
   * image, interleaved by sensor models
   *
   *  \details
   */
  virtual size_t GroundToImage(const std::vector<Point3d> &grounds,
                std::vector<Point2d> &images) const override;

private:
  std::vector<const ISensorModel*> mModels;   // one model per ray
  std::shared_ptr<EarthEllipsoid> mEarthEllip;
  std::unique_ptr<math::IIntersect> mIntersect;

  int mMaxIters{ 8 };
  double mTolZ{ 1.0e-4 };  // [mm]
  double mH{ 10.0 };

  bool ImageToGroundNR(const Point2d& leftImage, const Point2d& rightImage, Point3d& ground) const;

  void ObtainModelsInfo();

  // cache
  double mInvH{ 0.1 };
};
} // namespace earthxpert
