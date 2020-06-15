/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/math/math.h"

#include <array>
#include <bitset>
 //#include <chrono>
#include <cstdint>
#include <map>

namespace earthxpert {
// field group
struct FieldGroup : std::bitset<8> {
  // position
  static constexpr size_t kPOSXYZ = 0;    // kPDF0 ~ kPDF10
  static constexpr size_t kPOSCLA = 1;
  static constexpr size_t kPOSINT = 2;
  static constexpr size_t kPOSTIM = 3;    // kPDF1, kPDF3, etc
  static constexpr size_t kPOSRGB = 4;    // kPDF2, kPDF5, kPDF7, kPDF8, kPDF10
  static constexpr size_t kPOSNIR = 5;    // kPDF8, kPDF10
  static constexpr std::array<size_t, 6> kPOS = {
    kPOSXYZ, kPOSCLA, kPOSINT, kPOSTIM, kPOSRGB, kPOSNIR };

  // bit mask
  static constexpr size_t kBITXYZ = exp2(kPOSXYZ);
  static constexpr size_t kBITCLA = exp2(kPOSCLA);
  static constexpr size_t kBITINT = exp2(kPOSINT);
  static constexpr size_t kBITTIM = exp2(kPOSTIM);
  static constexpr size_t kBITRGB = exp2(kPOSRGB);
  static constexpr size_t kBITNIR = exp2(kPOSNIR);
  static constexpr std::array<size_t, 6> kBIT = {
    kBITXYZ, kBITCLA, kBITINT, kBITTIM, kBITRGB, kBITNIR };

  // often used bit masks
  static constexpr size_t kBITXC = kBITXYZ | kBITCLA;             // 3
  static constexpr size_t kBITXCI = kBITXYZ | kBITCLA | kBITINT;  // 7
  static constexpr size_t kBITXCR = kBITXYZ | kBITCLA | kBITRGB;  // 19
  static constexpr size_t kBITRN = kBITRGB | kPOSNIR;

  static inline const std::map<size_t, size_t> kPOSBIT = {
    {kPOSXYZ, kBITXYZ}, {kPOSCLA, kBITCLA}, {kPOSINT, kBITINT},
    {kPOSTIM, kBITTIM}, {kPOSNIR, kBITRGB}, {kPOSNIR, kBITNIR} };

  // Constructor
  constexpr FieldGroup() noexcept : std::bitset<8>() {}
  constexpr FieldGroup(size_t mask) noexcept : std::bitset<8>(mask) {}
  constexpr FieldGroup(const FieldGroup& fg) noexcept = default;

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
  constexpr size_t BitToPos(const size_t bit) {
    size_t pos{};
    size_t val{ bit };
    while (val >>= 1) {
      ++pos;
    }

    return pos;
  }

  /**
   *  \brief examine if rhs contains all fields of current field group
   *
   *  \param [in] rhs the other field group to compare with
   *  \return Returns true if all fields are contained in rhs.
   */
  bool operator<=(const FieldGroup& rhs) const {
    bool ret{ true };
    for (auto pos : FieldGroup::kPOS) {
      if (test(pos))
        ret = ret && rhs.test(pos);
    }
    return ret;
  }

  /**
   *  \brief examine if the current field group contains extra fields besides all fields of rhs
   *
   *  \param [in] rhs the other field group to compare with
   *  \return Returns true if being superset of rhs.
   */
  bool operator>(const FieldGroup& rhs) const {
    return !(operator<=(rhs));
  }
};

/**
 *  \brief XYZ field
 *
 *  \param CoordTy the type of point coordinates, such as int, float, double.
 */
template <typename CoordTy>
struct XYZ /*: public cv::Point3_<CoordTy>*/{
  using value_type = CoordTy;   // compatible with cv::Point_<>
  static constexpr int kDim = 3;

  static constexpr size_t kFieldPos = FieldGroup::kPOSXYZ;
  static constexpr size_t kFieldBit = FieldGroup::kBITXYZ;
  static constexpr size_t kFieldBytes = sizeof(CoordTy) * 3;
  CoordTy x{};  // x-coordinate
  CoordTy y{};  // y-coordinate
  CoordTy z{};  // z-coordinate

  /**
   *  \brief Convert to cv point type
   *
   *  \param CoordTy2 the type of cv point coordinates.
   */
  template <typename CoordTy2>
  operator cv::Point3_<CoordTy2>() const {
    return cv::Point3_<CoordTy2>(x, y, z);
  }
};

/**
 *  \brief classification field
 *
 *  The structure Classfication stores feature category value.
 */
struct Classification {
  static constexpr size_t kFieldPos = FieldGroup::kPOSCLA;
  static constexpr size_t kFieldBit = FieldGroup::kBITCLA;
  static constexpr size_t kFieldBytes = sizeof(uint8_t);
  uint8_t c{}; // feature catagory
};

/**
 *  \brief intensity field
 *
 *  The structure Intensity stores lidar intensity value.
 */
struct Intensity {
  static constexpr size_t kFieldPos = FieldGroup::kPOSINT;
  static constexpr size_t kFieldBit = FieldGroup::kBITINT;
  static constexpr size_t kFieldBytes = sizeof(uint16_t);
  uint16_t i{}; // laser intensity
};

/**
 *  \brief RGB field
 *
 *  The structure RGB stores the group of color fields.
 *
 *  \param ColorTy the type of point color, such as uchar, ushort.
 */
template <typename ColorTy>
struct RGB {
  using value_type = ColorTy;

  static constexpr size_t kFieldPos = FieldGroup::kPOSRGB;
  static constexpr size_t kFieldBit = FieldGroup::kBITRGB;
  static constexpr size_t kFieldBytes = sizeof(ColorTy) * 3;
  ColorTy b{};  // blue
  ColorTy g{};  // green
  ColorTy r{};  // red
};

/**
 *  The structure RGBN stores the group of color fields.
 *
 *  \param ColorTy the type of point color, such as uchar, ushort.
 */
template <typename ColorTy>
struct RGBN : RGB<ColorTy> {
  static constexpr size_t kFieldBit = FieldGroup::kBITRGB + FieldGroup::kBITNIR;
  static constexpr size_t kFieldBytes = sizeof(ColorTy) * 4;
  ColorTy n{};  // NIR
};

/**
 *  \brief time field
 *
 *  The structure GPSTime stores time stamp.
 */
struct GPSTime {
  static constexpr size_t kFieldPos = FieldGroup::kPOSTIM;
  static constexpr size_t kFieldBit = FieldGroup::kBITTIM;
  static constexpr size_t kFieldBytes = sizeof(double);
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
  static constexpr int kNumFields = sizeof...(Fields);

  static_assert(0 < kNumFields, "Empty fields");
  static constexpr size_t kPointFieldBits = (... + Fields::kFieldBit);
  //static constexpr size_t kPointFieldBytes = (... + Fields::kFieldBytes);   // align()
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
using XYZD = XYZ<double>;
using XYZI = XYZ<int32_t>;
using RGB16 = RGB<uint16_t>;

using PointF = Point<float>;
using PointI = Point<int32_t>;
using CPointF = CPoint<float>;
using CPointI = CPoint<int32_t>;

// float coordinates, 16bit color
using CRGBPointF = CRGBPoint<float, uint16_t>;

// integer coordinates, 16bit color
using CRGBPointI = CRGBPoint<int32_t, uint16_t>;
}
