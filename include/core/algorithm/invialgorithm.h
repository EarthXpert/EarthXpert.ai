/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include <memory>

namespace earthxpert {
/**
 *  The interface INviAlgorithm defines the behaviour of algorithms using
 *  Non-virtual interface idiom by Herb S.
 */
class EXCORE_EXPORTS INviAlgorithm {
public:
  virtual ~INviAlgorithm() = default;

  void NviMethod() {
    PreCondition();
    NviMethodImp();
    PostCondition();      
  }

protected:
  virtual void PreCondition() {}
  virtual void PostCondition() {}

private:
  virtual void MviMethodImp() = 0;  
};

using UPINviAlgorithm = std::unique_ptr<INviAlgorithm>;
using SPINviAlgorithm = std::shared_ptr<INviAlgorithm>;
}
