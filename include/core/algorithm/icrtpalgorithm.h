/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include <memory>

namespace earthxpert {
/**
 *  The interface ICrtpAlgorithm defines the behaviour of algorithms using
 *  Curiously Recurring Template Pattern idiom.
 */
template <typename D> 
class EXCORE_EXPORTS ICrtpAlgorithm {
public:
  virtual ~ICrtpAlgorithm() = default;

  void CrtpMethod() {
    staic_cast<D*>(this)->CrtpMethodImp();
  }
};

//using UPICrtpAlgorithm = std::unique_ptr<ICrtpAlgorithm>;
//using SPICrtpAlgorithm = std::shared_ptr<ICrtpAlgorithm>;
}
