/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/spatialref/geocoding.h"
#include "core/exceptions/exexcept.h"

using earthxpert::Geocoding;
using earthxpert::Point2d;

Geocoding::Geocoding(const Point2d& origin, const Point2d& pixSize) 
  : mOrigin(origin), mPixSize(pixSize) {
  if (0 == mPixSize.x || 0 == mPixSize.y)
    throw std::invalid_argument(_("Pixel ground size must be positive."));

  mInvPixX = 1.0 / mPixSize.x;
  mInvPixY = 1.0 / mPixSize.y;
}

Geocoding::Geocoding(const double originX, const double originY, 
  const double pixSizeX, const double pixSizeY)
    : Geocoding(Point2d(originX, originY), Point2d(pixSizeX, pixSizeY)) {
}

/**
 *  \brief Performs transformation from image coordinates to map coordinates
 *
 *  \param [in] image Image coordinates (col, row) [pixel]
 *  \return Returns map coordinates (Easting, Northing)
 */
Point2d Geocoding::ImageToMap(const Point2d &image) {
  return { mOrigin.x + image.x * mPixSize.x,
           mOrigin.y + image.y * mPixSize.y };
}

/**
 *  \brief Performs transformation from map coordinates to image
 * coordinates
 *
 *  \param [in] map Map coordinates (Easting, Northing)
 *  \return Returns image coordinates (col, row) [pixel]
 */
Point2d Geocoding::MapToImage(const cv::Point2d &map) {
  return { (map.x - mOrigin.x) * mInvPixX,
           (map.y - mOrigin.y) * mInvPixY };
}

/**
 *  \brief Converts an array of image coordinates to map coordinates
 *
 *  \param [in] images An array of image coordinates
 *  \param [in/out] maps Output an array of map coordinates
 */
void Geocoding::ImageToMap(const std::vector<Point2d> &images,
                              std::vector<Point2d> &maps) {
  if (images.size() != maps.size()) 
    throw std::invalid_argument(_("ImageToMap: vector size must be equal"));

  for (size_t i = 0; i < images.size(); ++i) {
    maps[i].x = mOrigin.x + images[i].x * mPixSize.x;
    maps[i].y = mOrigin.y + images[i].y * mPixSize.y;
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
  if (images.size() != maps.size()) 
    throw std::invalid_argument(_("MapToImage: vector size must be equal"));

  for (size_t i = 0; i < images.size(); ++i) {
    images[i].x = (maps[i].x - mOrigin.x) * mInvPixX;
    images[i].y = (maps[i].y - mOrigin.y) * mInvPixY;
  }
}

void Geocoding::Swap(Geocoding& rhs) noexcept {
  std::swap(mOrigin, rhs.mOrigin);
  std::swap(mPixSize, rhs.mPixSize);
  std::swap(mInvPixX, rhs.mInvPixX);
  std::swap(mInvPixY, rhs.mInvPixY);
}
