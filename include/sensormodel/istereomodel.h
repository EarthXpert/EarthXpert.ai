/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/exsensormodel.h"
#include "3rdparty/opencv/exopencv.h"

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
  // algorithm of forward intersection of rays
  enum ForwardAlgo {
    eCA_INTERSECT,    // intersect in 3d Cartisian space
    eSM_CERES,        // re-project residual via sensor model by ceres
    eSM_EXAI,         // analytic solution via physical sensor model
    eSM_NR            // re-project residual via sensor model by Newton-Raphson iteration
  };

  virtual ~IStereoModel() = default;

  /**
   *  Image-to-ground backward projection (fixed).
   *
   *  \brief Given a pair of sensor models and associated 2d positions
   *  in image coordinate system, do forward intersection of pair-rays, and
   *  output the ground position with respect to the ground coordinate system.
   *
   *  \param [in] leftImage 2d position (x, y) in left imagery raster coordinate
   * system
   *  \param [in] rightImage the corresponding 2d position (x, y) in right
   * imagery raster coordinate system
   *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate system
     \return Returns true if the operation succeeds
   */
  virtual bool ImageToGround(const Point2d &leftImage,
    const Point2d &rightImage, Point3d &ground) const = 0;

  /**
   *  \brief Given a pair of sensor models and associated list of 2d positions
   *  in image coordinate system, do forward intersection of pair-rays, and
   *  output the list of ground positions with respect to the ground coordinate
   * system.
   *
   *  \param [in] leftImages a list of 2d positions (x, y) in left imagery
   * raster coordinate system 
   *  \param [in] rightImages a list of corresponding
   *  2d position (x, y) in right imagery raster coordinate system 
   *  \param [in/out] grounds a list of 3d ground positions (X, Y, Z) in ground
   * coordinate system
   */
  virtual size_t ImageToGround(const std::vector<Point2d> &leftImages,
    const std::vector<Point2d> &rightImages, std::vector<Point3d> &grounds) const = 0;

  /**
   *  \brief Given a triplet of sensor models and associated 2d positions
   *  in image coordinate system, do forward intersection of triple-rays, and
   *  output the ground position with respect to the ground coordinate system.
   *
   *  \param [in] images the corresponding 2d positions (x, y) in imagery raster
   * coordinate systems \param [in/out] ground 3d ground position (X, Y, Z) in
   * ground coordinate system \return Returns true if the operation succeeds
   */
  virtual bool
  ImageToGround(const std::array<Point2d, 3> &images, Point3d &ground) const = 0;

  /**
   *  \brief Given two or more sensor models and associated 2d positions
   *  in image coordinate system, do forward intersection of rays, and
   *  output the ground position with respect to the ground coordinate system.
   *
   *  \param [in] images the corresponding 2d positions (x, y) in
   * imagery raster coordinate systems, interleaved by sensor models. \param
   * [in/out] ground 3d ground position (X, Y, Z) in ground coordinate system
   *  \return Returns true if the operation succeeds
   */
  virtual bool
  ImageToGround(const std::vector<Point2d> &images, Point3d &ground) const = 0;

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
  virtual bool ImageToGround(const std::vector<const ISensorModel*>& sensorModels,
    const std::vector<Point2d>& images, Point3d& ground) const = 0;

  /**
   *  \brief Given two or more sensor models and associated list of 2d positions
   *  in image coordinate system, do forward intersection of rays, and
   *  output the ground positions with respect to the ground coordinate system.
   *
   *  \param [in] images list of corresponding 2d positions (x, y) 
   *  in imagery raster coordinate systems, interleaved by sensor models.
   *  \param [in/out] ground List of 3d ground positions (X, Y, Z) in ground
   *  coordinate system
   */
  virtual size_t
  ImageToGround(const std::vector<Point2d> &images, std::vector<Point3d> &grounds) const = 0;

  /**
   *  Ground-to-image backward projection (fixed).
   *
   *  \brief Given a 3d ground position with reference to the ground coordinate
   *  system and multiple sensor models, output the corresponding 2d sensor
   *  positions in the image coordinate system for each sensor model.
   *
   *  \param [in] ground 3d position (X, Y, Z) in ground coordinate system
   *  \param [out] images 2d position (x, y) in imagery raster
   *  coordinate system \return Returns true if operation succeeds
   */
  virtual bool
  GroundToImage(const Point3d& ground, std::vector<Point2d>& images) const = 0;

  /**
   *  Ground-to-image backward projection (variable).
   *
   *  \brief Given a 3d ground position with reference to the ground coordinate
   *  system and a variable number of sensor models, output the corresponding 2d sensor
   *  positions in the image coordinate system for each sensor model.
   *
   *  \param [in] sensorModels an array of sensor models.
   *  \param [in] ground 3d position (X, Y, Z) in ground coordinate system
   *  \param [out] images 2d position (x, y) in imagery raster
   * coordinate system \return Returns true if operation succeeds
   */
  virtual bool
  GroundToImage(const std::vector<const ISensorModel*>& sensorModels, 
    const Point3d& ground, std::vector<Point2d>& images) const = 0;

  /**
   *  \brief Given a list of 3d positions in ground coordinate system and
   *  multiple sensor models, output a list of 2d sensor positions in image
   *  coordinate system.
   *
   *  \param [in] ground List of 3d ground positions (X, Y, Z)
   *  \param [in/out] images List of 2d positions (x, y) on
   *  image, interleaved by sensor models
   */
  virtual size_t
  GroundToImage(const std::vector<Point3d> &grounds, std::vector<Point2d> &images) const = 0;
};

using StereoFwdAlgo = IStereoModel::ForwardAlgo;
} // namespace earthxpert
