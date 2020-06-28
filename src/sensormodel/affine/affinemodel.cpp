/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/affinemodel.h"

using earthxpert::AffineModel;

/**
 *  \brief Construct an affine geometry model.
 *
 *  \param [in] numCols Image width [pixel].
 *  \param [in] numRows Image height [pixel].
 *  \param [in] geocoding the geocoding info.
 *  \param [in] groundSRS ground coordinate reference system
 */
AffineModel::AffineModel(const int numCols, const int numRows, 
  std::unique_ptr<Geocoding>&& geocoding, 
  std::shared_ptr<SpatialReferenceSystem> groundSRS) 
  : mNumCols(numCols), mNumRows(numRows), 
    mGeocoding(std::move(geocoding)), mGroundSRS(groundSRS) {}

/**
 *  Copy constructor
 *
 *  \param [in] rhs Object to copy from
 */
AffineModel::AffineModel(const AffineModel& rhs) { CopyFrom(rhs); }

/**
 *  Assignment operator
 *
 *  \param [in] rhs Object to copy from
 *  \return A reference to the newly modified object
 */
AffineModel& AffineModel::operator=(const AffineModel& rhs) {
  if (this != &rhs) {
    CopyFrom(rhs);
  }
  return *this;
}

/**
 *  Equality operator
 *
 *  \param [in] rhs Object to compare with
 *  \return true if same object or equal, false otherwise.
 */
bool AffineModel::operator==(const AffineModel& rhs) const {
  if (this != &rhs) {
    if (mNumCols != rhs.mNumCols || mNumRows != rhs.mNumRows)
      return false;
  }
  return true;
}

/**
 *  Test inequality.
 *
 *  \param [in] rhs Object to compare with
 *  \return true if not equal.
 */
bool AffineModel::operator!=(const AffineModel& rhs) const {
  return !(operator==(rhs));
}

bool AffineModel::EqualTo(const ISensorModel& rhs) const {
  if (ISensorModel::ModelType::eADS != rhs.GetModelType())
    return false;

  auto affModel = dynamic_cast<const AffineModel*>(&rhs);
  assert(affModel);

  return operator==(*affModel);
}

/**
 *  Copy values from another object
 *
 *  \param [in] rhs Object to copy from
 */
void AffineModel::CopyFrom(const AffineModel& rhs) {
  mNumCols = rhs.mNumCols;
  mNumRows = rhs.mNumRows;
  mGeocoding = rhs.mGeocoding->Clone();
  mGroundSRS = rhs.mGroundSRS;
}
