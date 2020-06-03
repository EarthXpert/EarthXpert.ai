/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/vector/vectorreader.h"
#include "core/datasource/vectorutils.h"
#include "core/exceptions/exexcept.h"
#include "core/spatialref/geocoding.h"
#include "core/utils/typehelper.h"

#include <numeric>

using namespace earthxpert;

/**
 *  \brief Construct reader object.
 *
 *  \param [in] vecFile full path of the vector file
 *  \param [in] layers an array of 1-based layer indices
 */
VectorReader::VectorReader(const std::string& vecFile, const std::vector<int>& layers)
  : mLayers(layers) {

  ObtainDatasetInfo();
}

/**
 *  \brief Construct reader object.
 *
 *  \param [in] dataset an opened dataset for reading
 *  \param [in] layers an array of 1-based layer indices
 */
VectorReader::VectorReader(GDALDataset* dataset, const std::vector<int>& layers)
  : mLayers(layers) {
  assert(dataset);
  mDataset.reset(dataset);

  ObtainDatasetInfo();
}

/**
 *  \return Returns metadata tags
 */
std::unique_ptr<VectorMetadata> VectorReader::GetMetadata() const {
  return {};
}

/**
 *  \return Returns pointer to the spatial coordinate system
 */
std::unique_ptr<SpatialReferenceSystem> VectorReader::GetCoordinateSystem() const {
  auto spatialRef = std::make_unique<SpatialReferenceSystem>(mDataset->GetProjectionRef());
  if (!spatialRef) {
    throw IOError("This image file has no infomation on coordinate system ");
  }
  return spatialRef;
}

/**
 *  \brief Reads the entire vector.
 *
 *  \param [in] layerType type of data structure
 *  \return Returns pointer to layer object holding the entire vector
 */
std::unique_ptr<ILayer> VectorReader::ReadLayer(LayerType layerType) {
  return {};
}

/**
 *  \brief Reads a vector block defined by offsets, size.
 *  The offsets are relative to the lower-left corner in Cartician coordinate system.
 *
 *  \param [in] minX
 *  \param [in] minY
 *  \param [in] width horizontal width
 *  \param [in] height vertical height
 *  \return Returns pointer to raster object holding a pixel block
 */
std::unique_ptr<ILayer> VectorReader::ReadLayer(const double minX, const double minY,
  const double width, const double height, const LayerType layerType) {
  return {};
}

/**
 *  \brief Reads a vector block defined by the passed vector object, populates
 *  the associated buffer.
 *
 *  \param [in/out] layer the layer object to be populated with the vector block.
 */
void VectorReader::ReadLayer(ILayer* const layer) {
}

/**
 *  \brief obtain dataset information.
 */
void VectorReader::ObtainDatasetInfo() {
}
