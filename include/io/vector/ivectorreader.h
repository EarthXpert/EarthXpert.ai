/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/exio.h"
#include "3rd_party/gdal/exgdal.h"
#include "core/datasource/ilayer.h"
#include "core/datasource/metadata.h"

namespace earthxpert {
class Geocoding;

/**
 *  The interface IVectorReader defines the behaviors for reading an image file
 *  and deserialize pixle block in an IRaster object.
 */
class EXIO_EXPORTS IVectorReader {
public:
  virtual ~IVectorReader() {}

  /**
   *  \return Returns metadata tags
   */
  virtual std::unique_ptr<VectorMetadata> GetMetadata() const = 0;

  /**
   *  \return Return the number of layers contained in the vector file
   */
  virtual int GetNumLayers() const noexcept = 0;

  /**
   *  \brief Fetch the ground coordinate system where the image file operates.
   *
   *  \return Returns pointer to a coordinate system
   */
  virtual std::unique_ptr<SpatialReferenceSystem> GetCoordinateSystem() const = 0;

  /**
   *  \brief Reads the entire vector.
   *
 *  \param [in] layerInfo information of layer to read
 *  \return Returns pointer to layer object holding the entire vector data source
   */
  virtual std::unique_ptr<ILayer> ReadLayer(const LayerInfo& layerInfo) = 0;

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
  virtual std::unique_ptr<ILayer> ReadLayer(double minX, double minY,
    double width, double height, const LayerInfo& layerInfo) = 0;

  /**
   *  \brief Reads a vector block defined by the passed vector object, populates
   *  the associated buffer.
   *
   *  \param [in/out] layer the layer object to be populated with the vector block.
   */
  virtual void ReadLayer(ILayer* const layer) = 0;

  /**
   *  \return Returns an ILodReader object for accessing varied levels of details of vector.
   */
  //virtual std::unique_ptr<ILodReader> GetLodReader() = 0;
};
} // namespace earthxpert
