/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/vector/ivectorwriter.h"
#include "io/vector/vectorreader.h"

#include <mutex>
#include <shared_mutex>

namespace earthxpert {
/**
 *  The class VectorWriter provides functios to write into a vector file
 *  with OGR as backend, and serialize vector block in an ILayer object
 *  having specific configurations, such as geometry, attributes, etc.
 *  Each VectorWriter instance is connected and thus bound to specific layers.
 *  Therefore a new instance is needed to write into different subsets of layers.
 *
 *  \remark will revisit for a better design.
 */
class EXIO_EXPORTS VectorWriter final : public IVectorWriter, public IVectorReader {
public:
  VectorWriter(const std::string& vecFile, const std::vector<int>& layers = {});
  VectorWriter(const std::string& vecFile, const std::string format,
    GDALDataType dataType, int numLayers);
  VectorWriter(GDALDataset* dataset, const std::vector<int>& layers = {});
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
   *  \brief Write the vector block contained in the layer object into vector file.
   *
   *  \param [in] layer the layer object to be written into vector file on disk
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
    return mReader->GetMetadata();
  }

  /**
   *  \return Return the number of layers contained in the vector file
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
    return mReader->GetCoordinateSystem();
  }

  /**
   *  \brief Reads the entire vector.
   *
   *  \param [in] layerType type of data structure
   *  \return Returns pointer to layer object holding the entire vector
   */
  std::unique_ptr<ILayer> ReadLayer(LayerType layerType = LayerType::ePOINT) override {
    return mReader->ReadLayer(layerType);
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
  std::unique_ptr<ILayer> ReadLayer(double minX, double minY,
    double width, double height, LayerType layerType = LayerType::ePOINT) override {
    return mReader->ReadLayer(minX, minY, width, height, layerType);
  }

  /**
   *  \brief Reads a vector block defined by the passed vector object, populates
   *  the associated buffer.
   *
   *  \param [in/out] layer the layer object to be populated with the vector block.
   */
  void ReadLayer(ILayer* const layer) override {
    mReader->ReadLayer(layer);
  }

  /**
   *  \return Returns an ILodReader object for accessing varied levels of details of vector.
   */
   //std::unique_ptr<ILodReader> GetLodReader() override;

private:
  std::unique_ptr<VectorReader> mReader;  // pointer to reader implementation

  mutable std::shared_mutex mMutex;

  // cached information
  GDALDataset* mDataset{ nullptr };  // non-owning pointer to GDALDataset object

  void ObtainDatasetInfo();
};
} // namespace earthxpert
