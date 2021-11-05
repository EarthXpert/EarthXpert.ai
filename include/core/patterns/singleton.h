/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/patterns/singletonbase.h"

namespace earthxpert {
/**
 */    
class EXCORE_EXPORTS Singleton : public SingletonBase<Singleton> {
public:
  void SingletonMethod();
  
private:
  friend class SingletonBase<Singleton>;

  Singleton() {}
  ~Singleton() {}
};
}
