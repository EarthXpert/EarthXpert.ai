/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/exsensormodel.h"
#include "3rdparty/gdal/exgdal.h"
#include "3rdparty/opencv/exopencv.h"

#include <memory>
#include <vector>
#include <utility>

namespace earthxpert {
/**
 *  The interface ISensorModel defines the geometric behaviours of an
 *  imaging sensor system.
 *
 *  The senosr geometry model gives the relationship between the 3d ground
 *  position (X, Y, Z) and the 2d image position (x, y).
 *
 *  The 3d ground position is measured in the coordinate system defined in
 *  ground space, such as cartesian coordinates (X, Y, Z) [unit: meter] in LSR,
 *  geographic coodinates (Longtitude, Latitude, height) [unit: degree/meter]
 *  in WGS84, or mapping coordinates (Easting, Northing, Height) [unit: meter]
 *  in UTM, etc.

 *  The 2d senor position is measured in the coordinate system defined in
 *  sensor space, such as image coordinates (column, row) [unit: pixel]
 *  w.r.t. the upper-left corner of the upper-left pixel in a raster image.
 */
class EXSENSORMODEL_EXPORTS ISensorModel {
public:
  virtual ~ISensorModel() = default;

  enum class ModelType {
    eUNDEFINED = -1,
    // physical sensor models
    eADS,       ///< Indicates an aerial push-broom sensor
    eFRAME,     ///< Indicates a frame based pin-hole camera
    ePUSHBROOM, ///< Indicates a push-broom sensor
    eROLLING,   ///< Indicates an aerial push-broom sensor
    // replacement sensor models
    eAFFINE,   ///< Indicates an affine model
    eNETWORK,   ///< Indicates a neural network model
    eRATIONAL, ///< Indicates a rational polynomial model
    NumSensorTypes
  };
  using mt = ModelType;

  virtual bool operator==(const ISensorModel &rhs) const = 0;
  virtual bool operator!=(const ISensorModel &rhs) const = 0;

  virtual std::unique_ptr<ISensorModel> Clone() const = 0;

  /**
   *  Image-to-ground forward projection.
   *
   *  \brief Given a 2d position in image coordinate system and a height
   *  in the ground vertical coordinate system, output the ground horizontal
   *  position with respect to the ground coordinate system.
   *
   *  \param [in] image 2d position (x, y) in imagery raster coordinate system
   *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate
   * system, among which Z's value is known.
   *  \return Returns true if the operation succeeds
   *
   *  \details
   */
  virtual bool ImageToGround(const Point2d &image, Point3d &ground) const = 0;

  virtual bool operator()(const Point2d& image, Point3d& ground) const = 0;

  /**
   *  \brief Given a 2d position in image coordinate system and a list of
   *  heights, output ground horizontal positions in ground coordinate system.
   *
   *  \param [in] image 2d position (x, y) in imagery raster coordinate system
   *  \param [in/out] ground 3d ground positions (X, Y, Z) in ground coordinate
   * system, among which Z values are known.
   *  \return Returns number of successful operations.
   *
   *  \details
   */
  virtual size_t ImageToGround(const Point2d &image,
                               std::vector<Point3d> &ground) const = 0;

  /**
   *  \brief Given a list of 2d positions in image coordinate system and a
   * corresponding list of heights, output the ground horizontal positions in
   * ground coordinate system.
   *
   *  \param [in] image List of 2d positions (x, y) on image
   *  \param [in/out] ground List of 3d ground positions (X, Y, Z), among which Z values are known.
   *  \return Returns number of successful operations.
   *
   *  \details
   */
  virtual size_t ImageToGround(const std::vector<Point2d> &image,
                               std::vector<Point3d> &ground) const = 0;

  /**
   *  Ground-to-image backward projection.
   *
   *  \brief Given a 3d ground position with reference to the ground coordinate
   * system, output the 2d image position in the image coordinate system.
   *
   *  \param [in] ground 3d position (X, Y, Z) in ground coordinate system
   *  \param [out] image 2d position (x, y) in imagery raster coordinate system
   *  \return Returns true if operation succeeds
   *
   *  \details
   */
  virtual bool GroundToImage(const Point3d &ground, Point2d &image) const = 0;

  virtual bool operator()(const Point3d& ground, Point2d& image) const = 0;

  /**
   *  \brief Given a list of 3d positions in ground coordinate system,
   *  output a list of 2d sensor positions in image coordinate system.
   *
   *  \param [in] ground List of 3d ground positions (X, Y, Z)
   *  \param [in/out] image List of 2d positions (x, y) on image
   *
   *  \details
   */
  virtual size_t GroundToImage(const std::vector<Point3d> &ground,
                               std::vector<Point2d> &image) const = 0;

  /**
   * \return Returns type of sensor model
   */
  virtual ModelType GetModelType() const noexcept = 0;

  /**
   *  \brief Fetch the ground coordinate system where the sensor model operates.
   *  Use this SRS to construct the coordinate transformations
   *  from results of ImageToGround() or inputs to GroundToImage() methods.
   *
   *  \return Returns pointer to a coordinate system
   */
  virtual std::shared_ptr<SpatialReferenceSystem>
  GetCoordinateSystem() const noexcept = 0;

  /**
   *  \return Returns raster columns & rows [unit: pixel]
   */
  virtual std::pair<int, int> GetImageSize() const noexcept = 0;

  /**
   *  \return Returns metric width & height [unit: mm]
   */
  //virtual std::pair<int, int> GetSensorSize() const noexcept = 0;
};
} // namespace earthxpert
