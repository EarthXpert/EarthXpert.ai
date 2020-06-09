/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/datasource/pointcloudutils.h"
#include "3rdparty/laslib/exlaslib.h"
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
    {pdt::kPDF0, {fgt::kBITXCI}},
    {pdt::kPDF1, {fgt::kBITXCI | fgt::kBITTIM}},
    {pdt::kPDF2, {fgt::kBITXCI | fgt::kBITRGB}},
    {pdt::kPDF3, {fgt::kBITXCI | fgt::kBITTIM | fgt::kBITRGB}},
    {pdt::kPDF4, {fgt::kBITXCI | fgt::kBITTIM}},
    {pdt::kPDF5, {fgt::kBITXCI | fgt::kBITTIM | fgt::kBITRGB}},
    {pdt::kPDF6, {fgt::kBITXCI | fgt::kBITTIM}},
    {pdt::kPDF7, {fgt::kBITXCI | fgt::kBITTIM | fgt::kBITRGB}},
    {pdt::kPDF8, {fgt::kBITXCI | fgt::kBITTIM | fgt::kBITRN}},
    {pdt::kPDF9, {fgt::kBITXCI | fgt::kBITTIM}},
    {pdt::kPDF10, {fgt::kBITXCI | fgt::kBITTIM | fgt::kBITRN}}
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
