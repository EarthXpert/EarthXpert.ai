/* ----------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/formats/support/rfmsupportdata.h"

using namespace earthxpert;

/**
 *  resolve the model filename from the image filename. possibly,
 *
 *    "image_basename" + "_rpc.txt"   // plain text
 *    "image_basename" + ".rpb"       // xml
 *    "image_basename" + ".rpc"       // same as "_rpc.txt"
 */
std::string RfmSupportData::ResolveModelFile(const std::string& imgFile) const {
  std::string mdlFile;
  return mdlFile;
}
