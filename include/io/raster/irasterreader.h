/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/exio.h"
#include "3rd_party/gdal/exgdal.h"
#include "core/datasource/iraster.h"
#include "core/datasource/metadata.h"

namespace earthxpert {
class Geocoding;
//class IPyramidReader;
using RasterType = IRaster::RasterType;

/**
 *  The interface IRasterReader defines the behaviors for reading an image file
 *  and deserialize pixle block in an IRaster object.
 */
class EXIO_EXPORTS IRasterReader {
public:
  virtual ~IRasterReader() {}

  /**
   *  \return Returns metadata tags
   */
  virtual std::unique_ptr<RasterMetadata> GetMetadata() const = 0;

  /**
   *  \return Returns width and height of image file [unit: pixels]
   */
  virtual std::pair<int, int> GetSize() const noexcept = 0;

  /**
   *  \return Return the number of channels contained in the image file
   */
  virtual int GetNumBands() const noexcept = 0;

  /**
   *  \brief Set the margin size and kind.
   *
   *  \param [in] marginCols the horizontal margin size
   *  \param [in] marginRows the vertical margin size
   *  \param [in] marginKind the margin kind.
   */
  virtual void SetMargins(int marginCols, int marginRows, MarginKind marginKind) = 0;

  /**
   *  \return Returns margin size.
   */
  virtual std::pair<int, int> GetMargins() const noexcept = 0;

  /**
   *  \return Returns the margin kind.
   */
  virtual MarginKind GetMarginKind() const noexcept = 0;

  /**
   *  \brief Fetch the ground coordinate system where the image file operates.
   *
   *  \return Returns pointer to a coordinate system
   */
  virtual std::unique_ptr<SpatialReferenceSystem> GetCoordinateSystem() const = 0;

  /**
   *  \brief Fetch the geocoding information of the image file.
   */
  virtual std::unique_ptr<Geocoding> GetGeocoding() const = 0;

  /**
   *  \brief Reads a pixel block defined by offsets, size, internal
   *  margin size, and kind. the offsets are relative to the upper-left corner
   *  pixel whose coordinates are (0, 0).
   *
   *  \param [in] colOffset starting column
   *  \param [in] rowOffset starting row
   *  \param [in] numCols horizontal width
   *  \param [in] numRows vertical height
   *  \return Returns pointer to raster object holding a pixel block
   */
  virtual std::unique_ptr<IRaster> ReadRaster(int colOffset, int rowOffset,
    int numCols, int numRows, RasterType rasterType = RasterType::eCVMAT) = 0;

  /**
   *  \brief Reads a pixel block defined by the passed raster object, populates
   *  the associated buffer.
   *
   *  \param [in/out] raster the raster object to be populated with the pixel block.
   */
  virtual void ReadRaster(IRaster* const raster) = 0;

  /**
   *  \return Returns an IPyramidReader object for accessing downsampled image overviews.
   */
  //virtual std::unique_ptr<IPyramidReader> GetPyramidReader() = 0;
};
} // namespace earthxpert
