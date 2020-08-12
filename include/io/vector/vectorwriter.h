/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/vector/ivectorwriter.h"
#include "io/vector/vectorreader.h"

#include <shared_mutex>

namespace earthxpert {
/**
 *  The class VectorWriter provides functios to write into a vector data source
 *  with OGR as backend, and serialize vector block in an ILayer object
 *  having specific configurations, such as geometry, attributes, etc.
 *  Each VectorWriter instance is connected and thus bound to specific layers.
 *  Therefore a new instance is needed to write into different subsets of layers.
 *
 *  \todo revisit the design for use with SpatiaLite, postgreSQL/PostGIS
 */
class EXIO_EXPORTS VectorWriter final : public IVectorWriter, public IVectorReader {
public:
  VectorWriter(const std::string& vecFile, const std::vector<LayerInfo>& layersInfo = {});
  VectorWriter(const fs::path& vecFile, const std::vector<LayerInfo>& layersInfo = {});
  VectorWriter(const std::string& vecFile, const std::string format,
    const std::vector<LayerInfo>& layersInfo);
  VectorWriter(const fs::path& vecFile, const std::string format,
    const std::vector<LayerInfo>& layersInfo);
  VectorWriter(GDALDataset* dataset, const std::vector<LayerInfo>& layersInfo = {});
  VectorWriter(const VectorWriter&) = delete;
  VectorWriter(VectorWriter&&) = delete;
  ~VectorWriter() = default;

  VectorWriter& operator=(const VectorWriter&) = delete;
  VectorWriter& operator=(VectorWriter&&) = delete;

  /**
   *  \return Returns the backend GDALDataset object. VectorWriter
   *  exports partially the OGR functions. The caller could exploit
   *  the full OGR capabilities via the returned object pointer.
   */
  GDALDataset* GetDataset() const noexcept {
    return mDataset;
  }

  /**
   *  \return Returns name of file format.
   */
  std::string GetFileFormat() const {
    return mDataset->GetDriverName();
  }

  /**
   *  \brief Inject metadata tags
   *
   *  \param [in] metaData the metadata tags
   */
  void SetMetadata(const VectorMetadata* metaData) override;

  /**
   *  \brief Inject the ground coordinate system where the image file operates
   *
   *  \param [in] spatialref the coordinate system
   */
  void SetCoordinateSystem(const SpatialReferenceSystem* srs) override;

  /**
   *  \brief Write the vector block contained in the layer object into vector data source.
   *
   *  \param [in] layer the layer object to be written into vector data source on disk
   */
  void WriteLayer(ILayer* const layer) override;

  /**
   *  \brief Generate levels of detail overviews.
   */
  void BuildLods() override;

  /**
   *  \return Returns metadata tags
   */
  std::unique_ptr<VectorMetadata> GetMetadata() const override {
    std::shared_lock<std::shared_mutex> lock(mMutex);
    return mReader->GetMetadata();
  }

  /**
   *  \return Return the number of layers contained in the vector data source
   */
  int GetNumLayers() const noexcept override {
    return 0;
  }

  /**
   *  \brief Fetch the ground coordinate system where the image file operates.
   *
   *  \return Returns pointer to a coordinate system
   */
  std::unique_ptr<SpatialReferenceSystem> GetCoordinateSystem() const override {
    std::shared_lock<std::shared_mutex> lock(mMutex);
    return mReader->GetCoordinateSystem();
  }

  /**
   *  \brief Reads the entire vector.
   *
   *  \param [in] layerInfo information of layer to read
   *  \return Returns pointer to layer object holding the entire vector data source
   */
  std::unique_ptr<ILayer> ReadLayer(const LayerInfo& layerInfo) override {
    std::shared_lock<std::shared_mutex> lock(mMutex);
    return mReader->ReadLayer(layerInfo);
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
  std::unique_ptr<ILayer> ReadLayer(double minX, double minY,
    double width, double height, const LayerInfo& layerInfo) override {
    std::shared_lock<std::shared_mutex> lock(mMutex);
    return mReader->ReadLayer(minX, minY, width, height, layerInfo);
  }

  /**
   *  \brief Reads a vector block defined by the passed vector object, populates
   *  the associated buffer.
   *
   *  \param [in/out] layer the layer object to be populated with the vector block.
   */
  void ReadLayer(ILayer* const layer) override {
    std::shared_lock<std::shared_mutex> lock(mMutex);
    mReader->ReadLayer(layer);
  }

  /**
   *  \return Returns an ILodReader object for accessing varied levels of details of vector.
   */
   //std::unique_ptr<ILodReader> GetLodReader() override;

private:
  void Initialize(GDALDataset* dataset, const std::vector<LayerInfo>& layersInfo = {});

  std::unique_ptr<VectorReader> mReader;  // pointer to reader implementation

  mutable std::shared_mutex mMutex;

  // cached information
  GDALDataset* mDataset{ nullptr };  // non-owning pointer to GDALDataset object

  void ObtainDatasetInfo();
};
} // namespace earthxpert
