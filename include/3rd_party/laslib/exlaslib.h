/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rd_party/ex3rd_party.h"

#include "LASlib/laszip.hpp"
#include <map>
#include <string>
#include <tuple>

namespace earthxpert::thirdparty {
// .LAS point data traits
struct PointDataTraits {
  // format
  static constexpr int kPDF0 = 0;
  static constexpr int kPDF1 = 1;
  static constexpr int kPDF2 = 2;
  static constexpr int kPDF3 = 3;
  static constexpr int kPDF4 = 4;
  static constexpr int kPDF5 = 5;
  static constexpr int kPDF6 = 6;
  static constexpr int kPDF7 = 7;
  static constexpr int kPDF8 = 8;
  static constexpr int kPDF9 = 9;
  static constexpr int kPDF10 = 10;

  static constexpr int kNumPDF = 11;  // total number of formats

  // record length
  static constexpr int ePDRL0 = 20;
  static constexpr int ePDRL1 = 28;
  static constexpr int ePDRL2 = 26;
  static constexpr int ePDRL3 = 34;
  static constexpr int ePDRL4 = 57;
  static constexpr int ePDRL5 = 63;
  static constexpr int ePDRL6 = 30;
  static constexpr int ePDRL7 = 36;
  static constexpr int ePDRL8 = 38;
  static constexpr int ePDRL9 = 59;
  static constexpr int ePDRL10 = 67;

  static inline const std::map<int, int> kFRL = {
    {kPDF0, ePDRL0}, {kPDF1, ePDRL1}, {kPDF2, ePDRL2}, {kPDF3, ePDRL3}, 
    {kPDF4, ePDRL4}, {kPDF5, ePDRL5}, {kPDF6, ePDRL6}, {kPDF7, ePDRL7}, 
    {kPDF8, ePDRL8}, {kPDF9, ePDRL9}, {kPDF10, ePDRL10} };

  /**
   *  \return Returns true if valid, otherwise false.
   */
  constexpr bool IsValidFormat(const int fmt) const noexcept {
    return (kPDF0 <= fmt && fmt <= kPDF10);
  }

  /**
   *  \return Returns true if valid, otherwise false.
   */
  static bool IsValidRecordLength(const int rl) noexcept {
    for (const auto frl : kFRL) {
      if (frl.second == rl)
        return true;
    }
    return false;
  }

  /**
   *  \param [in] fmt point data format.
   *  \return Returns point data record length if valid, otherwise 0.
   */
  static int FormatToRecLen(const int fmt) noexcept {
    const auto it = kFRL.find(fmt);
    return it != kFRL.end() ? it->second : 0;
  }

  /**
   *  \param [in] rl point data record length.
   *  \return Returns point data format if valid, otherwise -1.
   */
  static int RecLenToFormat(const int rl) noexcept {
    for (const auto frl : kFRL) {
      if (frl.second == rl)
        return frl.first;
    }
    return -1;
  }
};
 
void EX3RDPARTY_EXPORTS CreateLAX(const std::string &lasFile);
} // namespace earthxpert::thirdparty
