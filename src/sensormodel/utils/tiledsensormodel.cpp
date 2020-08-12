/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/utils/tiledsensormodel.h"
#include "3rdparty/gettext/exgettext.h"
#include "core/exceptions/exexcept.h"

#include <algorithm>
#include <cassert>
#include <omp.h>

using namespace earthxpert;

TiledSensorModel::TiledSensorModel(std::vector<RasterWindow>&& tiles,
  std::vector<std::unique_ptr<ISensorModel>>&& models)
  : mTiles(std::move(tiles)), mModels(std::move(models)) {
  Initialize();
}

void TiledSensorModel::Initialize() {
  mNumTiles = int(mTiles.size());
  if (mNumTiles != int(mModels.size()))
    throw std::invalid_argument(_("Incompatible layout"));
}

/**
 *  Copy constructor
 *
 *  \param [in] rhs Object to copy from
 */
TiledSensorModel::TiledSensorModel(const TiledSensorModel& rhs) { CopyFrom(rhs); }

/**
 *  Move constructor
 *
 *  \param [in] rhs Object to move from
 */
TiledSensorModel::TiledSensorModel(TiledSensorModel&& rhs) noexcept {
  MoveFrom(std::move(rhs));
}

/**
 *  Assignment operator
 *
 *  \param [in] rhs Object to copy from
 *  \return A reference to the newly modified object
 */
TiledSensorModel& TiledSensorModel::operator=(const TiledSensorModel& rhs) {
  if (this != &rhs) {
    CopyFrom(rhs);
  }
  return *this;
}

/**
 *  Move assignment operator
 *
 *  \param [in] rhs Object to move from
 *  \return A reference to the newly modified object
 */
TiledSensorModel& TiledSensorModel::operator=(TiledSensorModel&& rhs) noexcept {
  if (this != &rhs) {
    MoveFrom(std::move(rhs));
  }
  return *this;
}

/**
 *  Equality operator
 *
 *  \param [in] rhs Object to compare with
 *  \return true if same object or equal, false otherwise.
 */
bool TiledSensorModel::operator==(const TiledSensorModel& rhs) const {
  if (this != &rhs) {
    if (mNumTiles != rhs.GetNumTiles()) return false;

    auto& rhsTiles = rhs.GetTiles();
    auto& rhsModels = rhs.GetModels();
    for (int t = 0; t < mNumTiles; ++t) {
      if (mTiles[t] != rhsTiles[t]) return false;
      if (mModels[t] != rhsModels[t]) return false;
    }
  }
  return true;
}

/**
 *  Test inequality.
 *
 *  \param [in] rhs Object to compare with
 *  \return true if not equal.
 */
bool TiledSensorModel::operator!=(const TiledSensorModel& rhs) const {
  return !(operator==(rhs));
}

/**
 *  \brief Compare two models.
 *
 *  \param [in] rhs The other model to compare
 *  \return Return true if two models represent exactly same geometry
 */
bool TiledSensorModel::EqualTo(const ISensorModel& rhs) const {
  //if (ModelType::eRATIONAL != rhs.GetModelType())
  //  return false;

  auto tiledModel = dynamic_cast<const TiledSensorModel*>(&rhs);
  assert(tiledModel);

  return operator==(*tiledModel);
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
bool TiledSensorModel::ImageToGround(const Point2d& image, Point3d& ground) const {
  return true;
}

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
bool TiledSensorModel::GroundToImage(const Point3d& ground, Point2d& image) const {
  return true;
}

/**
 *  \brief Fetch the ground coordinate system where the sensor model operates.
 *  Use this SRS to construct the coordinate transformations
 *  from results of ImageToGround() or inputs to GroundToImage() methods.
 *
 *  \return Returns pointer to a coordinate system
 */
std::shared_ptr<SpatialReferenceSystem> 
TiledSensorModel::GetCoordinateSystem() const noexcept {
  return {};
}

 /**
  *  \return Returns the middle height
  */
double TiledSensorModel::GetMeanHeight() const noexcept {
  return 0.0;
}

/**
 *  Copy values from another object
 *
 *  \param [in] rhs Object to copy from
 */
void TiledSensorModel::CopyFrom(const TiledSensorModel& rhs) {
}

/**
 *  Move values from another object
 *
 *  \param [in] rhs Object to move from
 */
void TiledSensorModel::MoveFrom(TiledSensorModel&& rhs) noexcept {
  rhs.Clear();
}

void TiledSensorModel::Clear() noexcept {
}
