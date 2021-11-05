/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/exmarshaller.h"

namespace earthxpert {
enum MarshallLib {
  eCUSTOM = -1,
  eFileStorage,   // OpenCV file storage
  eFLATBUT,       // flat buffer
  ePROTOBUF,      // protocol buffer
};
}
