/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/spatialref/geocoding.h"
#include "core/exceptions/exexcept.h"

using earthxpert::Geocoding;
using earthxpert::Point2d;

Geocoding::Geocoding(const double originX, const double originY, 
  const double pixelSizeX, const double pixelSizeY)
    : mOriginX(originX), mOriginY(originY), mPixelSizeX(pixelSizeX),
  mPixelSizeY(pixelSizeY) {
  if (mPixelSizeX == 0 || mPixelSizeY == 0) {
    throw std::invalid_argument(_("Pixel ground size must be positive."));
  }

  mInvPixX = 1.0 / mPixelSizeX;
  mInvPixY = 1.0 / mPixelSizeY;
}

/**
 *  \brief Performs transformation from image coordinates to map coordinates
 *
 *  \param [in] image Image coordinates (col, row) [pixel]
 *  \return Returns map coordinates (Easting, Northing)
 */
Point2d Geocoding::ImageToMap(const Point2d &image) {
  return { mOriginX + image.x * mPixelSizeX,
           mOriginY + image.y * mPixelSizeY };
}

/**
 *  \brief Performs transformation from map coordinates to image
 * coordinates
 *
 *  \param [in] map Map coordinates (Easting, Northing)
 *  \return Returns image coordinates (col, row) [pixel]
 */
Point2d Geocoding::MapToImage(const cv::Point2d &map) {
  return { (map.x - mOriginX) * mInvPixX,
           (map.y - mOriginY) * mInvPixY };
}

/**
 *  \brief Converts an array of image coordinates to map coordinates
 *
 *  \param [in] images An array of image coordinates
 *  \param [in/out] maps Output an array of map coordinates
 */
void Geocoding::ImageToMap(const std::vector<Point2d> &images,
                              std::vector<Point2d> &maps) {
  if (images.size() != maps.size()) {
    throw;
  }
  for (size_t i = 0; i < images.size(); ++i) {
    maps[i].x = mOriginX + images[i].x * mPixelSizeX;
    maps[i].y = mOriginY + images[i].y * mPixelSizeY;
  }
}

/**
 *  \brief Converts and array of map coordinates to image coordinates
 *
 *  \param [in] maps An array of map coordinates
 *  \param [out] images An array of image coordinates
 */
void Geocoding::MapToImage(const std::vector<Point2d> &maps,
                              std::vector<Point2d> &images) {
  if (images.size() != maps.size()) {
    throw;
  }
  for (size_t i = 0; i < images.size(); ++i) {
    images[i].x = (maps[i].x - mOriginX) * mInvPixX;
    images[i].y = (maps[i].y - mOriginY) * mInvPixY;
  }
}
