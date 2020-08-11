/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/sensormodel.h"

namespace earthxpert {
/**
 *  The class TiledSensorModel implements the multi-tile logic.
 */
class EXSENSORMODEL_EXPORTS TiledSensorModel final : public SensorModel {
public:
  TiledSensorModel();

  TiledSensorModel(const TiledSensorModel& rhs);
  TiledSensorModel(TiledSensorModel&& rhs) noexcept;
  ~TiledSensorModel() = default;

  TiledSensorModel& operator=(const TiledSensorModel& rhs);
  TiledSensorModel& operator=(TiledSensorModel&& rhs) noexcept;

  bool operator==(const TiledSensorModel& rhs) const;
  bool operator!=(const TiledSensorModel& rhs) const;

  std::unique_ptr<ISensorModel> Clone() const override {
    return std::make_unique<TiledSensorModel>(*this);
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
   */
  bool ImageToGround(const Point2d &image, Point3d &ground) const override;

  /**
   *  Ground-to-image backward projection.
   *
   *  \brief Given a 3d ground position with reference to the ground coordinate
   * system, output the 2d image position in the image coordinate system.
   *
   *  \param [in] ground 3d position (X, Y, Z) in ground coordinate system
   *  \param [out] image 2d position (x, y) in imagery raster coordinate system
   *  \return Returns true if operation succeeds
   */
  bool GroundToImage(const Point3d &ground, Point2d &image) const override;

  /**
   * \return Returns type of sensor model
   */
  ModelType GetModelType() const noexcept override {
    return SensorType::eUNDEFINED;
  }

  /**
   *  \brief Fetch the ground coordinate system where the sensor model operates.
   *  Use this SRS to construct the coordinate transformations
   *  from results of ImageToGround() or inputs to GroundToImage() methods.
   *
   *  \return Returns pointer to a coordinate system
   */
  std::shared_ptr<SpatialReferenceSystem> 
  GetCoordinateSystem() const noexcept override;

  /**
   *  \return Returns type of SRS
   */
  SrsType GetSrsType() const noexcept override {
    return SrsType::eUNKNOWN;
  }

  /**
   *  \return Returns raster columns & rows [unit: pixel]
   */
  std::pair<int, int> GetImageSize() const noexcept override {
    return {};
  }

  /**
   *  \return Returns metric width & height [unit: mm]
   */
   //std::pair<int, int> GetSensorSize() const noexcept override;

   /**
    *  \return Returns the middle height
    */
  double GetMeanHeight() const noexcept override;

protected:
  std::vector<std::unique_ptr<ISensorModel>> mModels;

  bool EqualTo(const ISensorModel& rhs) const override;
  void CopyFrom(const TiledSensorModel& rhs);
  void MoveFrom(TiledSensorModel&& rhs) noexcept;
  void Clear() noexcept;

  // cached information
};
} // namespace earthxpert
