/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "platform/exai/exaiblockadjust.h"
#include "3rdparty/boost/exboost.h"
#include "3rdparty/gdal/exgdal.h"
#include "blockadjust/adjustframe.h"
#include "core/exceptions/exexcept.h"
#include "marshaller/p4dprjmarshaller.h"

#include <libxml/xmlreader.h>
#include <iostream>

using namespace earthxpert;

ExaiBlockAdjust::ExaiBlockAdjust() {
  LIBXML_TEST_VERSION;
  xmlInitParser();
}

ExaiBlockAdjust::~ExaiBlockAdjust() {
  xmlCleanupParser();
}

/**
 *  \brief 
 */
int ExaiBlockAdjust::Execute() {
  auto framePrj = P4dPrjMarshaller::Deserialize("");
  const auto ret = AdjustFrame(framePrj.get());
  return ret;
}

void ExaiBlockAdjust::Help() {
  printf("\nUsage: exai blockadjust\n");
}
