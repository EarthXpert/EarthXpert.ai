/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/vector/vectorreader.h"
#include "core/datasource/vectorutils.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/typehelper.h"

#include <cassert>
#include <numeric>

using namespace earthxpert;

/**
 *  \brief Construct reader object.
 *
 *  \param [in] vecFile full path of the vector data source
 *  \param [in] layersInfo an array of 1-based layer information
 */
VectorReader::VectorReader(const std::string& vecFile, const std::vector<LayerInfo>& layersInfo)
  : mLayersInfo(layersInfo) {
  auto dataset = GDALOpenEx(vecFile.c_str(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
  if (!dataset) {
    throw IOError("OGR failed to open vector data source for read access.");
  }

  Initialize(static_cast<GDALDataset*>(dataset));
}

/**
 *  \brief Construct reader object.
 *
 *  \param [in] dataset an opened dataset for reading
 *  \param [in] layers an array of 1-based layer indices
 */
VectorReader::VectorReader(GDALDataset* dataset, const std::vector<LayerInfo>& layersInfo)
  : mLayersInfo(layersInfo) {
  assert(dataset);
  Initialize(dataset);
}

/**
 *  \brief Initialize reader object.
 *
 *  \param [in] dataset an opened dataset for reading
 */
void VectorReader::Initialize(GDALDataset* dataset) {
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
    throw IOError("This data source file has no infomation on coordinate system ");
  }
  return spatialRef;
}

/**
 *  \brief Reads the entire vector.
 *
 *  \param [in] layerInfo information of layer to read
 *  \return Returns pointer to layer object holding the entire vector data source
 */
std::unique_ptr<ILayer> VectorReader::ReadLayer(const LayerInfo& layerInfo) {
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
 *  \param [in] layerInfo information of layer to read
 *  \return Returns pointer to layer object holding a vector block
 */
std::unique_ptr<ILayer> VectorReader::ReadLayer(const double minX, const double minY,
  const double width, const double height, const LayerInfo& layerInfo) {
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

void VectorReader::ReadLayer(OgrLayer* const layer) {
}

/**
 *  \brief obtain dataset information.
 */
void VectorReader::ObtainDatasetInfo() {
}
