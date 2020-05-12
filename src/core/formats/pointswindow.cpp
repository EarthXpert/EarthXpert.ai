/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/formats/pointswindow.h"

using watxtract::Point;
using watxtract::PointsHeader;
using watxtract::PointsWindow;

/**
 *  \brief the constructor of point
 *  \param [in] x
 *  \param [in] y
 *  \param [in] z
 *  \param [in] intensity
 *  \param [in] classification
 *  \param [in] r
 *  \param [in] g
 *  \param [in] b
 */
Point::Point(double x, double y, double z, uint16_t intensity,
             uint8_t classification, uint16_t r, uint16_t g, uint16_t b)
    : x(x), y(y), z(z), intensity(intensity), classification(classification),
      r(r), g(g), b(b) {}

/**
 *  \brief the constructor of point
 *
 *  \param [in] maxX max value of X
 *  \param [in] minX min value of X
 *  \param [in] maxY max value of Y
 *  \param [in] minY min value of Y
 *  \param [in] maxZ max value of Z
 *  \param [in] maxZ max value of Z
 *  \param [in] scaleX the multiplier of X
 *  \param [in] scaleY the multiplier of Y
 *  \param [in] scaleZ the multiplier of Z
 *  \param [in] offsetX offset value of X
 *  \param [in] offsetY offset value of Y
 *  \param [in] offsetZ offset value of Z
 */
PointsHeader::PointsHeader(const double maxX, const double minX,
                           const double maxY, const double minY,
                           const double maxZ, const double minZ,
                           const double scaleX, const double scaleY,
                           const double scaleZ, const double offsetX,
                           const double offsetY, const double offsetZ)
    : maxX(maxX), minX(minX), maxY(maxY), minY(minY), maxZ(maxZ), minZ(minZ),
      scaleX(scaleX), scaleY(scaleY), scaleZ(scaleZ), offsetX(offsetX),
      offsetY(offsetY), offsetZ(offsetZ) {}

/**
 *  \brief the constructor of pointswindow
 */
PointsWindow::PointsWindow(double maxX, double minX, double maxY, double minY,
  double maxZ, double minZ, double margin, std::vector<Point> &&data)
    : mMaxX(maxX), mMinX(minX), mMaxY(maxY), mMinY(minY), mMaxZ(maxZ), mMinZ(minZ), 
      mMargin(margin),  mData(std::move(data)) {
  mNumPoints = static_cast<int64_t>(mData.size());
}

/**
 *  \brief Get the information in the window
 *
 *  \param [out] maxX the greatest value of X
 *  \param [out] minX the smallest value of X
 *  \param [out] maxY the greatest value of Y
 *  \param [out] minY the smallest value of Y
 *  \param [out] margin the margin of the point cloud block
 */
void PointsWindow::GetBlockInfo(double &maxX, double &minX, double &maxY,
                                double &minY, double &margin) const {
  maxX = mMaxX;
  minX = mMinX;
  maxY = mMaxY;
  minY = mMinY;
  margin = mMargin;
}
