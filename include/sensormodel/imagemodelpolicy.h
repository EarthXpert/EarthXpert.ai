/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/iimagemodelpolicy.h"
#include "sensormodel/imagemodel.h"

#include <memory>

namespace earthxpert {
/**
 *  The class DigitalImageModel offers the transformation
 *  between the focal plane and the raster imagery.
 */
class EXSENSORMODEL_EXPORTS DigitalImageModel final : public IImageModelPolicy {
public:
  DigitalImageModel() {}

  /**
   *  \brief Construct a imagery model.
   *
   *  \param [in] imgModel image model defining raster vs focal plane
   * relationship
   */
  DigitalImageModel(std::shared_ptr<DigitalImageModelImp> imgModel);

  DigitalImageModel(const DigitalImageModel &rhs);
  DigitalImageModel(DigitalImageModel &&rhs) noexcept;
  ~DigitalImageModel() {}

  DigitalImageModel &operator=(const DigitalImageModel &rhs);
  DigitalImageModel &operator=(DigitalImageModel &&rhs) noexcept;

  bool operator==(const DigitalImageModel &rhs) const;
  bool operator!=(const DigitalImageModel &rhs) const;

  /**
   *  \brief Given a 2d position in image coordinate system,
   *  output the 2d position in sensor focal plane coordinate system.
   *
   *  \param [in] image 2d position (col, row) in imagery coordinate system
   *  \return Return 2d position (x, y) in sensor focal plane coordinate if the operation succeeds
   */
  Point2d ImageToSensor(const Point2d &image) const override;

  /**
   *  \brief Given a 2d position in sensor focal plane coordinate system,
   *  output the 2d position in image coordinate system.
   *
   *  \param [in] sensor 2d position (x, y) in sensor focal plane coordinate
   * system
   *  \return Return 2d position (col, row) in imagery coordinate if the operation succeeds
   */
  Point2d SensorToImage(const Point2d &sensor) const override;

  /**
   * \return Return type of image model policy
   */
  ImageModelPolicyType GetImageModelPolicyType() const noexcept override {
    return impt::eDIGITAL;
  }

  std::pair<int, int> GetImageSize() const noexcept override;

private:
  std::shared_ptr<DigitalImageModelImp>
      mImgModel{}; // image model defining raster vs focal plane relationship

  void CopyFrom(const DigitalImageModel &rhs);
  void MoveFrom(DigitalImageModel &&rhs) noexcept;
};

  /**
 *  The class FilmImageModel offers the transformation
 *  between the focal plane and the raster imagery.
 */
class EXSENSORMODEL_EXPORTS FilmImageModel final : public IImageModelPolicy {
public:
  FilmImageModel() {}

  /**
   *  \brief Construct a imagery model.
   *
   *  \param [in] imgModel image model defining raster vs focal plane
   * relationship
   */
  FilmImageModel(std::unique_ptr<FilmImageModelImp> &&imgModel);

  FilmImageModel(const FilmImageModel &rhs);
  FilmImageModel(FilmImageModel &&rhs) noexcept;
  ~FilmImageModel() {}

  FilmImageModel &operator=(const FilmImageModel &rhs);
  FilmImageModel &operator=(FilmImageModel &&rhs) noexcept;

  bool operator==(const FilmImageModel &rhs) const;
  bool operator!=(const FilmImageModel &rhs) const;

  /**
   *  \brief Given a 2d position in image coordinate system,
   *  output the 2d position in sensor focal plane coordinate system.
   *
   *  \param [in] image 2d position (col, row) in imagery coordinate system
   *  \return Return 2d position (x, y) in sensor focal plane coordinate if the operation succeeds
   */
  Point2d ImageToSensor(const Point2d &image) const override;

  /**
   *  \brief Given a 2d position in sensor focal plane coordinate system,
   *  output the 2d position in image coordinate system.
   *
   *  \param [in] sensor 2d position (x, y) in sensor focal plane coordinate
   * system
   * system \return Return 2d position (col, row) in imagery coordinate if the operation succeeds
   */
  Point2d SensorToImage(const Point2d &sensor) const override;

  /**
   * \return Return type of image model policy
   */
  ImageModelPolicyType GetImageModelPolicyType() const noexcept override {
    return impt::eFILM;
  }

  std::pair<int, int> GetImageSize() const noexcept override;

private:
  std::unique_ptr<FilmImageModelImp>
      mImgModel{}; // image model defining raster vs focal plane relationship

  void CopyFrom(const FilmImageModel &rhs);
  void MoveFrom(FilmImageModel &&rhs) noexcept;
};
} // namespace earthxpert
