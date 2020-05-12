/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/imagemodelpolicy.h"

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <type_traits>

using watxtract::DigitalImageModel;
using watxtract::FilmImageModel;
using watxtract::DigitalImageModelImp;
using watxtract::FilmImageModelImp;
using watxtract::Point2d;
using watxtract::Point3d;

/**
 *  Constructor.
 *
 *  \brief Construct a digital imagery model.
 *
 *  \param [in] imgModel image model defining raster vs focal plane relationship
 */
DigitalImageModel::DigitalImageModel(std::shared_ptr<DigitalImageModelImp> imgModel)
    : mImgModel(imgModel) {
    if (!mImgModel)
      throw std::invalid_argument("Parameter is empty: imgModel");
}

DigitalImageModel::DigitalImageModel(const DigitalImageModel &rhs) {}

DigitalImageModel::DigitalImageModel(DigitalImageModel &&rhs) noexcept {}

DigitalImageModel &DigitalImageModel::operator=(const DigitalImageModel &rhs) {
  return *this;
}

DigitalImageModel &DigitalImageModel::
operator=(DigitalImageModel &&rhs) noexcept {
  return *this;
}

bool DigitalImageModel::operator==(const DigitalImageModel &rhs) const {
  return true;
}

bool DigitalImageModel::operator!=(const DigitalImageModel &rhs) const {
  return !(operator==(rhs));
}

/**
 *  \brief Given a 2d position in image coordinate system,
 *  output the 2d position in sensor focal plane coordinate system.
 *
 *  \param [in] image 2d position (col, row) in imagery coordinate system
 *  \return Return 2d position (x, y) in sensor focal plane coordinate 
 */
Point2d DigitalImageModel::ImageToSensor(const Point2d &image) const {
  return mImgModel->CRToXY(image);
}

/**
 *  \brief Given a 2d position in sensor focal plane coordinate system,
 *  output the 2d position in image coordinate system.
 *
 *  \param [in] sensor 2d position (x, y) in sensor focal plane coordinate
 * system 
 *  \return Return 2d position (col, row) in imagery coordinate system 
 */
Point2d DigitalImageModel::SensorToImage(const Point2d &sensor) const {
  return mImgModel->XYToCR(sensor);
}

/**
 *  \return Return raster size of image (numCols, numRows) [unit: pixel]
 */
std::pair<int, int> DigitalImageModel::GetImageSize() const
    noexcept {
  return mImgModel->GetImageSize();
}

/**
 *  \brief helper of copy constructor & assignment.
 */
void DigitalImageModel::CopyFrom(const DigitalImageModel &rhs) {
  mImgModel = rhs.mImgModel;
}

/**
 *  \brief helper of move constructor & assignment.
 */
void DigitalImageModel::MoveFrom(DigitalImageModel &&rhs) noexcept {
  mImgModel = std::move(rhs.mImgModel);
}

/**
 *  Constructor.
 *
 *  \brief Construct a film imagery model.
 *
 *  \param [in] imgModel image model defining raster vs focal plane relationship
 */
FilmImageModel::FilmImageModel(std::unique_ptr<FilmImageModelImp> &&imgModel)
  : mImgModel(std::move(imgModel)) {
  if (!mImgModel)
    throw std::invalid_argument("Parameter is empty: imgModel");
}

FilmImageModel::FilmImageModel(const FilmImageModel &rhs) {}

FilmImageModel::FilmImageModel(FilmImageModel &&rhs) noexcept {}

FilmImageModel &FilmImageModel::operator=(const FilmImageModel &rhs) {
  return *this;
}

FilmImageModel &FilmImageModel::operator=(FilmImageModel &&rhs) noexcept {
  return *this;
}

bool FilmImageModel::operator==(const FilmImageModel &rhs) const {
  return true;
}

bool FilmImageModel::operator!=(const FilmImageModel &rhs) const {
  return !(operator==(rhs));
}

/**
 *  \brief Given a 2d position in image coordinate system,
 *  output the 2d position in sensor focal plane coordinate system.
 *
 *  \param [in] image 2d position (col, row) in imagery coordinate system
 *  \return Return 2d position (x, y) in sensor focal plane coordinate
 */
Point2d FilmImageModel::ImageToSensor(const Point2d &image) const {
  return mImgModel->CRToXY(image);
}

/**
 *  \brief Given a 2d position in sensor focal plane coordinate system,
 *  output the 2d position in image coordinate system.
 *
 *  \param [in] sensor 2d position (x, y) in sensor focal plane coordinate
 * system
 *  \return Return 2d position (col, row) in imagery coordinate system
 */
Point2d FilmImageModel::SensorToImage(const Point2d &sensor) const {
  return mImgModel->XYToCR(sensor);
}

/**
 *  \return Return raster size of image (numCols, numRows) [unit: pixel]
 */
std::pair<int, int> FilmImageModel::GetImageSize() const noexcept {
  return mImgModel->GetImageSize();
}

/**
 *  \brief helper of copy constructor & assignment.
 */
void FilmImageModel::CopyFrom(const FilmImageModel &rhs) {
  mImgModel = rhs.mImgModel->Clone();
}

/**
 *  \brief helper of move constructor & assignment.
 */
void FilmImageModel::MoveFrom(FilmImageModel &&rhs) noexcept {
  mImgModel = std::move(rhs.mImgModel);
}
