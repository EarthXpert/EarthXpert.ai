/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/opencv/exopencv.h"

namespace earthxpert {

class EXCORE_EXPORTS Geocoding {
public:
  double mOriginX{};
  double mOriginY{};
  double mPixelSizeX{};
  double mPixelSizeY{};

  Geocoding(double originX, double originY, double pixelSizeX, double pixelSizeY);
  ~Geocoding() = default;

  std::unique_ptr<Geocoding> Clone() const {
    return std::make_unique<Geocoding>(*this);
  }

  /**
   *  \brief Performs transformation from image coordinates to map coordinates
   *
   *  \param [in] image Image coordinates (col, row) [pixel]
   *  \return Returns map coordinates (Easting, Northing)
   */
  Point2d ImageToMap(const Point2d &image);

  /**
   *  \brief Performs transformation from map coordinates to image
   * coordinates
   *
   *  \param [in] map Map coordinates (Easting, Northing)
   *  \return Returns image coordinates (col, row) [pixel]
   */
  Point2d MapToImage(const Point2d &map);

  /**
   *  \brief Converts an array of image coordinates to map coordinates
   *
   *  \param [in] images An array of image coordinates
   *  \param [in/out] maps Output an array of map coordinates
   */
  void ImageToMap(const std::vector<Point2d> &images,
                     std::vector<Point2d> &maps);

  /**
   *  \brief Converts and array of map coordinates to image coordinates
   *
   *  \param [in] maps An array of map coordinates
   *  \param [out] images An array of image coordinates
   */
  void MapToImage(const std::vector<Point2d> &maps,
                     std::vector<Point2d> &images);

  void Swap(Geocoding& rhs) noexcept;

private:
  double mInvPixX{ 1.0 };
  double mInvPixY{ 1.0 };
};
} // namespace earthxpert

// overload std::swap
namespace std {
inline void swap(earthxpert::Geocoding& lhs,
  earthxpert::Geocoding& rhs) noexcept {
  lhs.Swap(rhs);
}
}
