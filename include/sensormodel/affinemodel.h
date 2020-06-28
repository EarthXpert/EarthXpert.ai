/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/sensormodel.h"
#include "core/spatialref/geocoding.h"

#include <array>

namespace earthxpert {
/**
 *  The class AffineModel implements the geocding model for ortho.
 */
class EXSENSORMODEL_EXPORTS AffineModel final : public SensorModel {
public:
  ~AffineModel() {}

  /**
   *  \brief Construct an affine geometry model.
   *
   *  \param [in] numCols Image width [pixel].
   *  \param [in] numRows Image height [pixel].
   *  \param [in] geocoding the geocoding info.
   *  \param [in] groundSRS ground coordinate reference system
   */
  AffineModel(int numCols, int numRows, std::unique_ptr<Geocoding>&& geocoding,
    std::shared_ptr<SpatialReferenceSystem> groundSRS);

  AffineModel(const AffineModel& rhs);
  AffineModel(AffineModel&& rhs) noexcept = default;

  AffineModel& operator=(const AffineModel& rhs);
  AffineModel& operator=(AffineModel&& rhs) noexcept = default;

  bool operator==(const AffineModel& rhs) const;
  bool operator!=(const AffineModel& rhs) const;

  /**
   *  Image-to-ground forward projection.
   *
   *  \brief Given a 2d position in image coordinate system 
   *  in the ground vertical coordinate system, output the ground horizontal
   *  position with respect to the ground coordinate system.
   *
   *  \param [in] image 2d position (x, y) in imagery raster coordinate system
   *  \param [out] ground 3d ground position (X, Y, Z) in ground coordinate system 
   *  \return Returns true if the operation succeeds
   */
  bool ImageToGround(const Point2d& image, Point3d& ground) const override {
    ground = Point2To3<double>(mGeocoding->ImageToMap(image));
    return true;
  }

  /**
   *  Ground-to-image backward projection.
   *
   *  \brief Given a 2d ground position with reference to the ground coordinate
   * system, output the 2d image position in the image coordinate system.
   *
   *  \param [in] ground 3d ground position (X, Y, Z)
   *  \param [out] image 2d image position (x, y)
   *  \return Returns true if operation succeeds
   */
  bool GroundToImage(const Point3d& ground, Point2d& image) const override {
    image = mGeocoding->MapToImage(Point2d(ground.x, ground.y));
    return true;
  }

  /**
   * @return type of sensor model
   */
  ModelType GetModelType() const noexcept override {
    return ModelType::eAFFINE;
  }

  /**
   *  \return Return raster size of image (numCols, numRows) [unit: pixel]
   */
  std::pair<int, int> GetImageSize() const noexcept override {
    return {0, 0};
  }

private:
  int mNumCols{}; // number of columns of the image product
  int mNumRows{}; // number of rows of the image product
  std::unique_ptr<Geocoding> mGeocoding;
  std::shared_ptr<SpatialReferenceSystem> mGroundSRS{}; // Ground coordinate system

  bool EqualTo(const ISensorModel& rhs) const override;
  void CopyFrom(const AffineModel& rhs);

  // cached calculation for better performance
};
} // namespace earthxpert
