/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rd_party/opencv/exopencv.h"
#include "sensormodel/exsensormodel.h"

#include <array>
#include <vector>

namespace earthxpert {
class ISensorModel;

/**
 *  The interface IStereoModel defines the geometric relationship
 *  in a stereo model that is composed of a block of sensor geometry models.
 *
 *  The stereo model gives the relationship between the 3d ground
 *  position (X, Y, Z) and a set of 2d sensor positions (x1, y1) (x2, y2) ...
 *
 *  The 3d ground position is measured in the coordinate system defined in
 *  ground space, such as cartesian coordinates (X, Y, Z) [unit: meter] in LSR,
 *  geographic coodinates (Longtitude, Latitude, height) [unit: degree/meter]
 *  in WGS84, or mapping coordinates (Easting, Northing, Height) [unit: meter]
 *  in UTM, etc.

 *  The 2d image position is measured in the coordinate system defined in
 *  imagery raster space, such as image coordinates (column, row) [unit: pixel]
 *  w.r.t. the upper-left corner of the upper-left pixel in a raster image.
 */
class EXSENSORMODEL_EXPORTS IStereoModel {
public:
  virtual ~IStereoModel() {}

  /**
   *  Image-to-ground forward intersection.
   *
   *  \brief Given a pair of sensor models and associated 2d positions
   *  in image coordinate system, do forward intersection of pair-rays, and
   *  output the ground position with respect to the ground coordinate system.
   *
   *  \param [in] leftModel left sensor model composing the stereo model
   *  \param [in] leftImage 2d position (x, y) in left imagery raster coordinate
   * system
   *  \param [in] rightModel right sensor model composing the stereo model
   *  \param [in] rightImage the corresponding 2d position (x, y) in right
   * imagery raster coordinate system
   *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate system
     \return Returns true if the operation succeeds
   *
   *  \details
   */
  virtual bool ImageToGround(ISensorModel const *const leftModel,
                             const Point2d &leftImage,
                             ISensorModel const *const rightModel,
                             const Point2d &rightImage,
                             Point3d &ground) const = 0;

  /**
   *  \brief Given a pair of sensor models and associated list of 2d positions
   *  in image coordinate system, do forward intersection of pair-rays, and
   *  output the list of ground positions with respect to the ground coordinate
   * system.
   *
   *  \param [in] leftModel left sensor model composing the stereo model
   *  \param [in] leftImages a list of 2d positions (x, y) in left imagery
   * raster coordinate system \param [in] rightModel right sensor model
   * composing the stereo model \param [in] rightImages a list of corresponding
   * 2d position (x, y) in right imagery raster coordinate system \param
   * [in/out] grounds a list of 3d ground positions (X, Y, Z) in ground
   * coordinate system
   *
   *  \details
   */
  virtual size_t ImageToGround(ISensorModel const *const leftModel,
                               const std::vector<Point2d> &leftImages,
                               ISensorModel const *const rightModel,
                               const std::vector<Point2d> &rightImages,
                               std::vector<Point3d> &grounds) const = 0;

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
                const std::array<Point2d, 3> &image, Point3d &ground) const = 0;

  /**
   *  \brief Given a triplet of sensor models and associated list of triplet 2d
   * positions in image coordinate system, do forward intersection of
   * triple-rays, and output the ground position with respect to the ground
   * coordinate system.
   *
   *  \param [in] sensorModels triplet sensor models composing the stereo model
   *  \param [in] images list of corresponding 2d positions (x, y) in imagery
   * raster coordinate systems, interleaved by sensor models. \param [in/out]
   * grounds 3d list of ground positions (X, Y, Z) in ground coordinate system
   *
   *  \details
   */
  virtual size_t
  ImageToGround(const std::array<ISensorModel const *, 3> &sensorModels,
                const std::vector<Point2d> &images,
                std::vector<Point3d> &grounds) const = 0;

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
                const std::vector<Point2d> &image, Point3d &ground) const = 0;

  /**
   *  \brief Given two or more sensor models and associated list of 2d positions
   *  in image coordinate system, do forward intersection of rays, and
   *  output the ground positions with respect to the ground coordinate system.
   *
   *  \param [in] sensorModels multiple (two or more) sensor models composing
   * the stereo model \param [in] images list of corresponding 2d positions (x,
   * y) in imagery raster coordinate systems, interleaved by sensor models.
   *  \param [in/out] ground List of 3d ground positions (X, Y, Z) in ground
   * coordinate system
   *
   *  \details
   */
  virtual size_t
  ImageToGround(const std::vector<ISensorModel const *> &sensorModels,
                const std::vector<Point2d> &images,
                std::vector<Point3d> &grounds) const = 0;

  /**
   *  Ground-to-image backward projection.
   *
   *  \brief Given a 3d ground position with reference to the ground coordinate
   * system and multiple sensor models, output the corresponding 2d sensor
   * positions in the image coordinate system for each sensor model.
   *
   *  \param [in] ground 3d position (X, Y, Z) in ground coordinate system
   *  \param [in] sensorModels multiple (two or more) sensor models composing
   * the stereo model \param [out] image 2d position (x, y) in imagery raster
   * coordinate system \return Returns true if operation succeeds
   *
   *  \details
   */
  virtual bool
  GroundToImage(const Point3d &ground,
                const std::vector<ISensorModel const *> &sensorModels,
                std::vector<Point2d> &image) const = 0;

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
                std::vector<Point2d> &images) const = 0;
};
} // namespace earthxpert
