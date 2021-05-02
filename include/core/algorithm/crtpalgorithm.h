/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/algorithm/icrtpalgorithm.h"

namespace earthxpert {
/**
 *  The  CrtpAlgorithm implements the behaviour.
 */
class EXCORE_EXPORTS CrtpAlgorithm : public ICrtpAlgorithm<CrtpAlgorithm> {
public:
  CrtpAlgorithm() = default;

  void CrtpMethodImp();
};

using UPCrtpAlgorithm = std::unique_ptr<CrtpAlgorithm>;
using SPCrtpAlgorithm = std::shared_ptr<CrtpAlgorithm>;
}
