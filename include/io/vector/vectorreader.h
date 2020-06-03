/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/vector/ivectorreader.h"

#include <mutex>
#include <tuple>
#include <vector>

namespace earthxpert {
/**
 *  The class VectorReader provides format in-dependent functios to read from
 *  a vector file with OGR as backend, and deserialize entities in an ILayer object
 *  having specific configurations, such as geometry, attributes, etc.
 *  Each VectorReader instance is connected and thus bound to specific layers.
 *  Therefore a new instance is needed to read from different subsets of available layers.
 */
class EXIO_EXPORTS VectorReader final : public IVectorReader {
public:
  VectorReader(const std::string& vecFile, const std::vector<int>& layers = {});
  VectorReader(GDALDataset* dataset, const std::vector<int>& layers = {});
  VectorReader(const VectorReader&) = delete;
  VectorReader(VectorReader&&) = delete;
  ~VectorReader() = default;

  VectorReader& operator=(const VectorReader&) = delete;
  VectorReader& operator=(VectorReader&&) = delete;

  /**
   *  \return Returns the backend GDALDataset object. VectorReader
   *  exports partially the OGR functions. The caller could exploit
   *  the full OGR capabilities via the returned object pointer.
   */
  GDALDataset* GetDataset() const noexcept {
    return mDataset.get();
  }

  /**
   *  \return Returns name of file format.
   */
  std::string GetFileFormat() const {
    return mDataset->GetDriverName();
  }

  /**
   *  \return Returns metadata tags
   */
  std::unique_ptr<VectorMetadata> GetMetadata() const override;

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
  std::unique_ptr<SpatialReferenceSystem> GetCoordinateSystem() const override;

  /**
   *  \brief Reads the entire vector.
   *
   *  \param [in] layerType type of data structure
   *  \return Returns pointer to layer object holding the entire vector
   */
  std::unique_ptr<ILayer> ReadLayer(LayerType layerType = LayerType::ePOINT) override;

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
    double width, double height, LayerType layerType = LayerType::ePOINT) override;

  /**
   *  \brief Reads a vector block defined by the passed vector object, populates
   *  the associated buffer.
   *
   *  \param [in/out] layer the layer object to be populated with the vector block.
   */
  void ReadLayer(ILayer* const layer) override;

  /**
   *  \return Returns an ILodReader object for accessing varied levels of details of vector.
   */
   //std::unique_ptr<ILodReader> GetLodReader() override;

private:
  GDALDatasetUniquePtr mDataset;  // pointer to GDALDataset object
  std::vector<int> mLayers;  // list of layers to read from

  mutable std::mutex mMutex;

  // cached information & metadata

  void ObtainDatasetInfo();
};
} // namespace earthxpert
