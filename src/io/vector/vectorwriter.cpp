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
 *  \brief Construct writer object for existing vector data source.
 *
 *  \param [in] vecFile full path of the vector data source
 *  \param [in] layersInfo an array of 1-based layer information
 */
VectorWriter::VectorWriter(const std::string& vecFile, const std::vector<LayerInfo>& layersInfo) {
  mDataset = static_cast<GDALDataset*>(GDALOpenEx(vecFile.c_str(), GDAL_OF_UPDATE, nullptr, nullptr, nullptr));
  if (!mDataset) 
    throw IOError(_("OGR failed to open vector data source for write acess."));
  Initialize(mDataset, layersInfo);
}

VectorWriter::VectorWriter(const fs::path& vecFile, const std::vector<LayerInfo>& layersInfo) 
  : VectorWriter(vecFile.string(), layersInfo) {}

/**
 *  \brief Construct writer object for newly created vector data source.
 *
 *  \param [in] vecFile path and name of vector data source to create
 *  \param [in] format format of vector data source
 *  \param [in] layersInfo an array of 1-based layer information
 */
VectorWriter::VectorWriter(const std::string& vecFile, const std::string format,
  const std::vector<LayerInfo>& layersInfo) {
  mDataset = thirdparty::CreateVectorDataset(vecFile, format, layersInfo);

  Initialize(mDataset, {});
}

VectorWriter::VectorWriter(const fs::path& vecFile, const std::string format,
  const std::vector<LayerInfo>& layersInfo) 
  : VectorWriter(vecFile.string(), format, layersInfo) {}

/**
 *  \brief Initialize both reader and writer components for existing vector data source.
 *
 *  \param [in] dataset an opened dataset for writing
 *  \param [in] layers an array of 1-based layer indices
 */
void VectorWriter::Initialize(GDALDataset* dataset, const std::vector<LayerInfo>& layersInfo) {
  std::vector<int> layers;
  mReader = std::make_unique<VectorReader>(dataset, layers);
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
void VectorWriter::SetCoordinateSystem(const OGRSpatialRef* srs) {
  std::unique_lock<std::shared_mutex> lock(mMutex);
  char* wkt{nullptr};
  srs->exportToWkt(&wkt);
  mDataset->SetProjection(wkt);
  CPLFree(wkt);
}

/**
 *  \brief Write the vector block contained in the layer object into vector data source.
 *
 *  \param [in] layer the layer object to be written into vector data source on disk
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
