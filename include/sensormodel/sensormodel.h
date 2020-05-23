/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/isensormodel.h"

namespace earthxpert {
/**
 *  The class SensorModel implements the template methods having logic
 *  common to mulitple sensor systems. A concrete sensor model may override
 *  the template method for better perforamnce.
 */
class WXSENSORMODEL_EXPORTS SensorModel : public ISensorModel {
public:
  virtual ~SensorModel() {}

  /**
   *  \brief compare the equality of two models.
   *
   *  \param [in] rhs the other model to compare with
   *  \return Return true if two models represent same geometry.
   */
  bool operator==(const ISensorModel &rhs) const override {
    return EqualTo(rhs);
  }

  /**
   *  \param [in] rhs the other model to compare with
   *  \return Return true if two models represent different geometry.
   */
  bool operator!=(const ISensorModel &rhs) const override {
    return !EqualTo(rhs);
  }

  /**
   *  Image-to-ground forward projection.
   *
   *  \brief Given a 2d position in image coordinate system and a height
   *  in the ground vertical coordinate system, output the ground horizontal
   *  position with respect to the ground coordinate system.
   *
   *  \param [in] image 2d position (x, y) in imagery raster coordinate system
   *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate
   * system \return Returns true if the operation succeeds
   *
   *  \details
   */
  virtual bool ImageToGround(const Point2d &image, Point3d &ground) const;

  bool operator()(const Point2d& image, Point3d& ground) const override {
    return ImageToGround(image, ground);
  }

  /**
   *  \brief Given a 2d position in image coordinate system and a list of
   *  heights, output ground horizontal positions in ground coordinate system.
   *
   *  \param [in] image 2d position (x, y) in imagery raster coordinate system
   *  \param [in/out] ground 3d ground positions (X, Y, Z) in ground coordinate
   * system
   *
   *  \details More details
   */
  virtual size_t ImageToGround(const Point2d &image,
                               std::vector<Point3d> &ground) const override;

  /**
   *  \brief Given a list of 2d positions in image coordinate system and a
   * corresponding list of heights, output the ground horizontal positions in
   * ground coordinate system.
   *
   *  \param [in] image List of 2d positions (x, y) on image
   *  \param [in/out] ground List of 3d ground positions (X, Y, Z)
   *
   *  \details More details
   */
  virtual size_t ImageToGround(const std::vector<Point2d> &image,
                               std::vector<Point3d> &ground) const override;

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
  virtual bool GroundToImage(const Point3d &ground, Point2d &image) const;

  bool operator()(const Point3d& ground, Point2d& image) const override {
    return GroundToImage(ground, image);
  }

  /**
   *  \brief Given a list of 3d positions in ground coordinate system,
   *  output a list of 2d sensor positions in image coordinate system.
   *
   *  \param [in] ground List of 3d ground positions (X, Y, Z)
   *  \param [in/out] image List of 2d positions (x, y) on image
   *
   *  \details More details
   */
  virtual size_t GroundToImage(const std::vector<Point3d> &ground,
                               std::vector<Point2d> &image) const override;

protected:
  virtual bool EqualTo(const ISensorModel &rhs) const = 0;
};
} // namespace watxtract