/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/wxcore.h"
#include "3rd_party/opencv/wxopencv.h"
#include "core/math/math.h"

#include <array>
#include <bitset>
//#include <chrono>
#include <cstdint>
#include <map>

namespace watxtract {
// field group
struct FieldGroup : std::bitset<sizeof(size_t)> {
  // position
  static constexpr size_t kPOSXYZ = 0;
  static constexpr size_t kPOSCLA = 1;
  static constexpr size_t kPOSINT = 2;
  static constexpr size_t kPOSRGB = 3;
  static constexpr size_t kPOSNIR = 4;
  static constexpr size_t kPOSTIM = 5;
  static constexpr std::array<size_t, 6> kPOS = { 
    kPOSXYZ, kPOSCLA, kPOSINT, kPOSRGB, kPOSNIR, kPOSTIM };

  // bit mask
  static constexpr size_t kBITXYZ = exp2(kPOSXYZ);
  static constexpr size_t kBITCLA = exp2(kPOSCLA);
  static constexpr size_t kBITINT = exp2(kPOSINT);
  static constexpr size_t kBITRGB = exp2(kPOSRGB);
  static constexpr size_t kBITNIR = exp2(kPOSNIR);
  static constexpr size_t kBITTIM = exp2(kPOSTIM);
  static constexpr std::array<size_t, 6> kBIT = {
    kBITXYZ, kBITCLA, kBITINT, kBITRGB, kBITNIR, kBITTIM };

  static inline const std::map<size_t, size_t> kPOSBIT = {
    {kPOSXYZ, kBITXYZ}, {kPOSCLA, kBITCLA}, {kPOSINT, kBITINT}, 
    {kPOSNIR, kBITRGB}, {kPOSNIR, kBITNIR}, {kPOSTIM, kBITTIM} };

  /**
   *  \param [in] pos The field group position.
   *  \return Returns field group bit mask.
   */
  constexpr size_t PosToBit(const size_t pos) const noexcept {
    return exp2(pos);
  }

  /**
   *  \param [in] bit The field group bit mask.
   *  \return Returns field group position.
   *  \todo validate bit mask
   */
  constexpr size_t Bit2Pos(const size_t bit) {
    size_t pos{};
    size_t val{ bit };
    while (val >>= 1) {
      ++pos;
    }

    return pos;
  }
};

/**
 *  \param CoordTy the type of point coordinates, such as int, float, double.
 */
template <typename CoordTy>
struct XYZ /*: public cv::Point3_<CoordTy>*/{
  static constexpr size_t kFieldGroup = FieldGroup::kBITXYZ;
  CoordTy x{};  // x-coordinate
  CoordTy y{};  // y-coordinate
  CoordTy z{};  // z-coordinate
};

/**
 *  The structure Classfication stores feature category value.
 */
struct Classification {
  static constexpr size_t kFieldGroup = FieldGroup::kBITCLA;
  uint8_t c{}; // feature catagory
};

/**
 *  The structure Intensity stores lidar intensity value.
 */
struct Intensity {
  static constexpr size_t kFieldGroup = FieldGroup::kBITINT;
  uint16_t i{}; // laser intensity
};

/**
 *  The structure RGB stores the group of color fields.
 *
 *  \param ColorTy the type of point color, such as uchar, ushort.
 */
template <typename ColorTy>
struct RGB {
  static constexpr size_t kFieldGroup = FieldGroup::kBITRGB;
  ColorTy r{};  // red
  ColorTy g{};  // green
  ColorTy b{};  // blue
};

/**
 *  The structure RGBN stores the group of color fields.
 *
 *  \param ColorTy the type of point color, such as uchar, ushort.
 */
template <typename ColorTy>
struct RGBN : RGB<ColorTy> {
  static constexpr size_t kFieldGroup = FieldGroup::kBITRGB + FieldGroup::kBITNIR;
  ColorTy n{};  // NIR
};

/**
 *  The structure GPSTime stores time stamp.
 */
struct GPSTime {
  static constexpr size_t kFieldGroup = FieldGroup::kBITTIM;
  double time{};
};

/**
 *  The structure WxPoint stores 3D coordinates, classification, and
 *  optional relevant fields of laser point record.
 *
 *  \param Fields the pack of fields group.
 */
template <typename... Fields>
struct WxPoint : Fields... {
  static_assert(0 < sizeof...(Fields), "Empty fields");
  static constexpr size_t kPointFieldGroup = (... + Fields::kFieldGroup);
};

// XYZ
template <typename CoordTy>
using Point = WxPoint<XYZ<CoordTy>>;

// XYZ, classification
template <typename CoordTy>
using CPoint = WxPoint<XYZ<CoordTy>, Classification>;

// XYZ, intensity, classification
template <typename CoordTy>
using CIPoint = WxPoint<XYZ<CoordTy>, Intensity, Classification>;

// XYZ, RGB, classification
template <typename CoordTy, typename ColorTy>
using CRGBPoint = WxPoint<XYZ<CoordTy>, RGB<ColorTy>, Classification>;

// XYZ, RGBN, classification
template <typename CoordTy, typename ColorTy>
using CRGBNPoint = WxPoint<XYZ<CoordTy>, RGBN<ColorTy>, Classification>;

// time, XYZ
template <typename CoordTy>
using PointT = WxPoint<GPSTime, XYZ<CoordTy>>;

// typical usages
using XYZF = XYZ<float>;
using XYZI = XYZ<int32_t>;
using PointF = Point<float>;
using PointI = Point<int32_t>;
using CPointF = CPoint<float>;
using CPointI = CPoint<int32_t>;

// float coordinates, 16bit color
using CRGBPointF = CRGBPoint<float, uint16_t>;

// integer coordinates, 16bit color
using CRGBPointI = CRGBPoint<int32_t, uint16_t>;
}
