/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/wxcore.h"

#include <iostream>
#include <memory>
#include <vector>

namespace watxtract {
/**
 *  Internal structure to store the point data
 */
struct WXCORE_EXPORTS Point {
  double x;
  double y;
  double z;
  uint16_t intensity;
  uint8_t classification;
  uint16_t r;
  uint16_t g;
  uint16_t b;

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
   *
   */
  Point(double x, double y, double z, uint16_t intensity,
        uint8_t classification, uint16_t r, uint16_t g, uint16_t b);
};

/**
 *  Internal structure to store the header information for las file
 */
struct WXCORE_EXPORTS PointsHeader {
  double maxX;
  double minX;
  double maxY;
  double minY;
  double maxZ;
  double minZ;
  double scaleX;
  double scaleY;
  double scaleZ;
  double offsetX;
  double offsetY;
  double offsetZ;

  /**
   *  \brief the constructor
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
  PointsHeader(double maxX, double minX, double maxY,
               double minY, double maxZ, double minZ,
               double scaleX, double scaleY, double scaleZ,
               double offsetX, double offsetY, double offsetZ);
};

/**
 *  class to store point cloud data including header infortmation
 */
class WXCORE_EXPORTS PointsWindow {
public:
  PointsWindow(double maxX, double minX, double maxY, double minY, double maxZ,
               double minZ, double margin, std::vector<Point> &&data);
  ~PointsWindow() = default;

  /**
   *  \brief Get the information in the block
   *
   *  \param [out] maxX the greatest value of X
   *  \param [out] minX the smallest value of X
   *  \param [out] maxY the greatest value of Y
   *  \param [out] minY the smallest value of Y
   *  \param [out] margin the margin of the point cloud block
   */
  void GetBlockInfo(double &maxX, double &minX, double &maxY, double &minY,
                    double &margin) const;

  /**
   *  \brief Get data for the point cloud data
   *
   *  \return Returns reference to points vector
   */
  const std::vector<Point> &GetPointsData() const {
    return mData;
  }

  /**
   *  \return Returns Z range
   */
  std::pair<double, double> GetRangeZ() const {
    return {mMaxZ, mMinZ};
  }

private:
  double mMaxX{};
  double mMinX{};
  double mMaxY{};
  double mMinY{};
  double mMaxZ{};
  double mMinZ{};
  double mMargin{};
  std::vector<Point> mData;

  // cached 
  int64_t mNumPoints{};
};
} // namespace watxtract
