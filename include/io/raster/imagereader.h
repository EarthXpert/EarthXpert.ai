/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/raster/iimagereader.h"
#include "3rd_party/gdal/wxgdal.h"

#include <memory>

namespace watxtract {
class Geocoding;
class ImageWindow;

/**
 *  Raster reader interface. Read the raster file into ImageWindow containing
 * image data.
 */
class WXIO_EXPORTS ImageReader : public IImageReader {
public:
  /**
   *  \brief Open the image file to read
   *
   *  \param [in] imageFile Full path of the image file
   */
  ImageReader(const std::string &imageFile);
  ImageReader &operator=(const ImageReader &) = delete;
  ImageReader(const ImageReader &) = delete;
  virtual ~ImageReader() {};

  /**
   *  \brief Set the margin value and type for read
   *  \param [in] marginCols margin value in X axis
   *  \param [in] marginRows margin value in Y axis
   *  \param [in] marginType the type of margin applying to read
   */
  void SetMargin(int16_t marginCols, int16_t marginRows,
                 MarginType marginType) override;

  /**
   *  \return Return Data type
   */
  GDALDataType GetDataType() const;

  /**
   *  \brief Reads a image block defined by offset and size
   *
   *  \param [in] colOffset starting column (0,0 is upper left corner)
   *  \param [in] rowOffset starting row (0,0 is upper left corner)
   *  \param [in] numCols width  to read
   *  \param [in] numRows height to reade
   *  \return Returns pointer to image window
   *
   *  \details Function will throw, exception hierarchy to be defined
   */
  std::unique_ptr<ImageWindow> ReadRaster(const int32_t colOffset,
                                          const int32_t rowOffset,
                                          const int32_t numCols,
                                          const int32_t numRows) override;

  /**
   *  \brief Reads and populates a given image window
   *
   *  \param [in] imageWindow Pre-allocated image window whose content has to be
   *  populated with data
   */
  void ReadRaster(std::unique_ptr<ImageWindow> &imageWindow) override;

  /**
   *  \brief File geocoding
   *
   *  \return Returns pointer to file geocoding
   */
  std::unique_ptr<Geocoding> GetGeocoding() const override;

  /**
   *  \brief Returns file size
   *
   *  \param [out] numCols number of columns
   *  \param [out] numRows number of rows
   *  \return Return image size
   */
  void GetSize(int32_t &numCols, int32_t &numRows) const override;

  /**
   *  \return Returns number of bands in image
   */
  int GetNumBands() const override {
    return mDataset->GetRasterCount();
  }

  /**
   *  \brief Coordinate system
   *
   *  \return Returns the coordinate system
   *
   *  \details https://www.gdal.org/classOGRSpatialReference.html
   */
  OGRSpatialReference GetCoordinateSystem() const override;

protected:
  ImageReader(GDALDataset *dataset);
  GDALDatasetUniquePtr mDataset;

private:
  int16_t mMarginCols{0};
  int16_t mMarginRows{0};
  MarginType mMarginType{eZERO};
  GDALDataType mDataType{GDT_Byte};

  /**
   *  \brief internal reads a image block defined by offset and size
   *
   *  \param [in] colOffset starting column (0,0 is upper left corner)
   *  \param [in] rowOffset starting row (0,0 is upper left corner)
   *  \param [in] numCols width  to read
   *  \param [in] numRows height to reade
   *  \param [in] marginCols margin value in X axis
   *  \param [in] marginRows margin value in Y axis
   *  \return Returns pointer to image window
   *
   *  \details Function will throw, exception hierarchy to be defined
   */
  std::unique_ptr<uint8_t[]> Read(const int32_t colOffset,
                                  const int32_t rowOffset,
                                  const int32_t numCols, const int32_t numRows,
                                  const int16_t marginCols,
                                  const int16_t marginRows);
};
} // namespace watxtract
