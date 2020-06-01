/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/datasource/utils.h"
#include "3rd_party/laslib/exlaslib.h"
#include "core/datasource/pointtypes.h"
#include "core/exceptions/exexcept.h"

#include <cassert>
#include <map>

namespace earthxpert {
using pdt = thirdparty::PointDataTraits;
using fgt = FieldGroup;

// local details
namespace detail {
  // // \todo lidar related fields, wave
  static const std::map<int, FieldGroup> kPDFFG = {
    {pdt::kPDF0, {fgt::kBITXYZ | fgt::kBITCLA | fgt::kBITINT}},
    {pdt::kPDF1, {fgt::kBITXYZ | fgt::kBITCLA | fgt::kBITINT | fgt::kBITTIM}},
    {pdt::kPDF2, {fgt::kBITXYZ | fgt::kBITCLA | fgt::kBITINT | fgt::kBITRGB}},
    {pdt::kPDF3, {fgt::kBITXYZ | fgt::kBITCLA | fgt::kBITINT | fgt::kBITTIM | fgt::kBITRGB}},
    {pdt::kPDF4, {fgt::kBITXYZ | fgt::kBITCLA | fgt::kBITINT | fgt::kBITTIM}},   
    {pdt::kPDF5, {fgt::kBITXYZ | fgt::kBITCLA | fgt::kBITINT | fgt::kBITTIM | fgt::kBITRGB}},
    {pdt::kPDF6, {fgt::kBITXYZ | fgt::kBITCLA | fgt::kBITINT | fgt::kBITTIM}},
    {pdt::kPDF7, {fgt::kBITXYZ | fgt::kBITCLA | fgt::kBITINT | fgt::kBITTIM | fgt::kBITRGB}},
    {pdt::kPDF8, {fgt::kBITXYZ | fgt::kBITCLA | fgt::kBITINT | fgt::kBITTIM | fgt::kBITRGB | fgt::kBITNIR}},
    {pdt::kPDF9, {fgt::kBITXYZ | fgt::kBITCLA | fgt::kBITINT | fgt::kBITTIM}},
    {pdt::kPDF10, {fgt::kBITXYZ | fgt::kBITCLA | fgt::kBITINT | fgt::kBITTIM | fgt::kBITRGB | fgt::kBITNIR}}
  };
}

/**
 *  \param [in] fmt The point data format.
 *  \return Returns field group corresponding to the format.
 */
FieldGroup FormatToFieldGroup(const int fmt) noexcept {
  assert(pdt::IsValidFormat(fmt));
  return detail::kPDFFG.at(fmt);
}

/**
 *  \brief find the format so that it contains all fields specified in fieldGroup and
 *  the record length is smallest among all valid formats.
 *
 *  \param [in] fieldGroup The field group.
 *  \return Returns the format with minimum record length if successful, pdt::kPDF0 otherwise.
 */
int FieldGroupToFormat(const FieldGroup fieldGroup) noexcept {
  assert(fieldGroup.test(fgt::kPOSXYZ));
  for (auto [pdf, fg] : detail::kPDFFG) {
    if (fieldGroup <= fg)
      return pdf;
  }
  return pdt::kPDF0;
}
}
