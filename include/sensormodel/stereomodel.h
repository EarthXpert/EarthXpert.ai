/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/istereomodel.h"

namespace watxtract {
class ISensorModel;

/**
 *  The mixin class StereoModel implements the template methods having logic
 *  common to mulitple sensor systems. A concrete sensor model may override
 *  the template method for better perforamnce. In detail, the bundle of
 *  rays from a block of sensor geometry models are used to implement
 *    forward intersection and backward projection in the network.
 */
class WXSENSORMODEL_EXPORTS StereoModel : public IStereoModel {
public:
  StereoModel();
  virtual ~StereoModel() {}

  /**
   *  Image-to-ground forward intersection.
   *
   *  \brief Given a pair of sensor models and associated 2d positions
   *  in image coordinate system, do forward intersection of pair-rays, and
   *  output the ground position with respect to the ground coordinate system.
   *
   *  \param [in] leftModel left sensor model composing the stereo model
   *  \param [in] leftImage 2d position (x, y) in left image raster coordinate
   * system 
   *  \param [in] rightModel right sensor model composing the stereo model
   *  \param [in] rightImage the corresponding 2d position (x, y) in right image
   * raster coordinate system 
   *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate 
   * system \return Returns true if the operation succeeds
   *
   *  \details
   */
  virtual bool ImageToGround(ISensorModel const *const leftModel,
                             const Point2d &leftImage,
                             ISensorModel const *const rightModel,
                             const Point2d &rightImage, Point3d &ground) const override;

  /**
   *  \brief Given a pair of sensor models and associated list of 2d positions
   *  in image coordinate system, do forward intersection of pair-rays, and
   *  output the list of ground positions with respect to the ground coordinate
   * system.
   *
   *  \param [in] leftModel left sensor model composing the stereo model
   *  \param [in] leftImages a list of 2d positions (x, y) in left image raster
   * coordinate system \param [in] rightModel right sensor model composing the
   * stereo model \param [in] rightImages a list of corresponding 2d position
   * (x, y) in right image raster coordinate system \param [in/out] grounds a
   * list of 3d ground positions (X, Y, Z) in ground coordinate system
   *
   *  \details
   */
  virtual size_t ImageToGround(ISensorModel const *const leftModel,
                               const std::vector<Point2d> &leftImages,
                               ISensorModel const *const rightModel,
                               const std::vector<Point2d> &rightImages,
                               std::vector<Point3d> &grounds) const override;

  /**
   *  \brief Given a triplet of sensor models and associated 2d positions
   *  in image coordinate system, do forward intersection of triple-rays, and
   *  output the ground position with respect to the ground coordinate system.
   *
   *  \param [in] sensorModels triplet sensor models composing the stereo model
   *  \param [in] image the corresponding 2d positions (x, y) in imagery raster
   * coordinate systems \param [in/out] ground 3d ground position (X, Y, Z) in
   * ground coordinate system \return Returns true if the operation succeeds
   *
   *  \details
   */
  virtual bool
  ImageToGround(const std::array<ISensorModel const *, 3> &sensorModels,
                const std::array<Point2d, 3> &image, Point3d &ground) const override;

  /**
   *  \brief Given a triplet of sensor models and associated list of triplet 2d
   * positions in image coordinate system, do forward intersection of
   * triple-rays, and output the ground position with respect to the ground
   * coordinate system.
   *
   *  \param [in] sensorModels triplet sensor models composing the stereo model
   *  \param [in] images the corresponding 2d positions (x, y) in imagery raster
   * coordinate systems, interleaved by sensor models. \param [in/out] grounds
   * 3d ground position (X, Y, Z) in ground coordinate system
   *
   *  \details
   */
  virtual size_t
  ImageToGround(const std::array<ISensorModel const *, 3> &sensorModels,
                const std::vector<Point2d> &images,
                std::vector<Point3d> &grounds) const override;

  /**
   *  \brief Given two or more sensor models and associated 2d positions
   *  in image coordinate system, do forward intersection of rays, and
   *  output the ground position with respect to the ground coordinate system.
   *
   *  \param [in] sensorModels multiple (two or more) sensor models composing
   * the stereo model \param [in] image the corresponding 2d positions (x, y) in
   * imagery raster coordinate systems, interleaved by sensor models. \param
   * [in/out] ground 3d ground position (X, Y, Z) in ground coordinate system
   *  \return Returns true if the operation succeeds
   *
   *  \details
   */
  virtual bool
  ImageToGround(const std::vector<ISensorModel const *> &sensorModels,
                const std::vector<Point2d> &image, Point3d &ground) const override;

  /**
   *  \brief Given two or more sensor models and associated list of 2d positions
   *  in image coordinate system, do forward intersection of rays, and
   *  output the ground positions with respect to the ground coordinate system.
   *
   *  \param [in] sensorModels multiple (two or more) sensor models composing
   * the stereo model \param [in] images List of corresponding 2d positions (x,
   * y) in imagery raster coordinate systems, interleaved by sensor models.
   *  \param [in/out] grounds List of 3d ground positions (X, Y, Z) in ground
   * coordinate system
   *
   *  \details
   */
  virtual size_t
  ImageToGround(const std::vector<ISensorModel const *> &sensorModels,
                const std::vector<Point2d> &image,
                std::vector<Point3d> &grounds) const override;

  /**
   *  Ground-to-image backward projection.
   *
   *  \brief Given a 3d ground position with reference to the ground coordinate
   * system and multiple sensor models, output the corresponding 2d sensor
   * positions in the image coordinate system for each sensor model.
   *
   *  \param [in] ground 3d position (X, Y, Z) in ground coordinate system
   *  \param [in] sensorModels multiple (two or more) sensor models composing
   * the stereo model \param [out] image 2d position (x, y) in sensor raster
   * coordinate system \return Returns true if operation succeeds
   *
   *  \details
   */
  virtual bool
  GroundToImage(const Point3d &ground,
                const std::vector<ISensorModel const *> &sensorModels,
                std::vector<Point2d> &image) const override;

  /**
   *  \brief Given a list of 3d positions in ground coordinate system and
   *  multiple sensor models, output a list of 2d sensor positions in image
   * coordinate system.
   *
   *  \param [in] ground List of 3d ground positions (X, Y, Z)
   *  \param [in] sensorModels multiple (two or more) sensor models composing
   * the stereo model \param [in/out] images List of 2d positions (x, y) on
   * image, interleaved by sensor models
   *
   *  \details
   */
  virtual size_t
  GroundToImage(const std::vector<Point3d> &grounds,
                const std::vector<ISensorModel const *> &sensorModels,
                std::vector<Point2d> &images) const override;
};
} // namespace watxtract
