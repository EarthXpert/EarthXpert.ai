/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "marshaller/project/bgoprjmarshaller.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/stlutils.h"

using namespace earthxpert;

/**
 *  \brief write or update a bingo directory containing geoin.dat
 */
void BgoPrjMarshaller::Serialize(const std::string& bgoFile,
  FrameProject const* const bgoPrj) {

}

/**
 *  \brief read from a bingo directory containing geoin.dat
 */
std::unique_ptr<FrameProject> 
BgoPrjMarshaller::Deserialize(const std::string& bgoFile) {
  return {};
}
