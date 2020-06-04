/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */

#include "io/vector/vectorwriter.h"
#include "core/datasource/vectorutils.h"
#include "core/exceptions/exexcept.h"

#include <cassert>

using namespace earthxpert;

/**
 *  \brief Construct writer object for existing vector file.
 *
 *  \param [in] vecFile full path of the vector file
 *  \param [in] layers an array of 1-based layer indices
 */
VectorWriter::VectorWriter(const std::string& vecFile, const std::vector<int>& layers) {
  ObtainDatasetInfo();
}

/**
 *  \brief Construct writer object for newly created vector file.
 *
 *  \param [in] vecFile path and name of vector file to create
 *  \param [in] format format of vector file
 *  \param [in] dataType datatype for the vector
 *  \param [in] numLayers number of layers in the vector
 */
VectorWriter::VectorWriter(const std::string& vecFile, const std::string format,
  const GDALDataType dataType, const int numLayers) {
  ObtainDatasetInfo();
}

/**
 *  \brief Construct writer object for existing vector file.
 *
 *  \param [in] dataset an opened dataset for writing
 *  \param [in] layers an array of 1-based layer indices
 */
VectorWriter::VectorWriter(GDALDataset* dataset, const std::vector<int>& layers) {
  ObtainDatasetInfo();
}

/**
 *  \brief Inject metadata tags
 *
 *  \param [in] metaData the metadata tags
 */
void VectorWriter::SetMetadata(const VectorMetadata* metaData) {
  std::unique_lock<std::shared_mutex> lock(mMutex);
}

/**
 *  \brief Inject the ground coordinate system where the image file operates
 *
 *  \param [in] spatialref the coordinate system
 */
void VectorWriter::SetCoordinateSystem(const SpatialReferenceSystem* srs) {
  std::unique_lock<std::shared_mutex> lock(mMutex);
  char* wkt{nullptr};
  srs->exportToWkt(&wkt);
  mDataset->SetProjection(wkt);
  CPLFree(wkt);
}

/**
 *  \brief Write the vector block contained in the layer object into vector file.
 *
 *  \param [in] layer the layer object to be written into vector file on disk
 */
void VectorWriter::WriteLayer(ILayer* const layer) {
}

/**
 *  \brief Generate levels of detail overviews.
 */
void VectorWriter::BuildLods() {

}

/**
 *  \brief obtain dataset information.
 */
void VectorWriter::ObtainDatasetInfo() {
  assert(mDataset);
  assert(mReader);
}
