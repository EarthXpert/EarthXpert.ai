/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"

namespace earthxpert {
/**
 *  class template SingletonBase is a generic singleton type.
 *
 *  A singleton candidate must satisfy three requirements:
 *  
 *  1. controls concurrent access to a shared resource.
 *  2. access to the resource will be requested from multiple, disparate parts of the system.
 *  3. there can be only one object.
 * 
 *  A simple logger is the most obvious example of a possibly-valid Singleton, but this can change with more complex logging schemes.
 *
 *  If the proposed Singleton has only one or two of these requirements, a redesign is almost always the correct option.
 *  For example, a printer spooler is unlikely to be called from more than one place (the Print menu), so can use mutexes to solve the concurrent access problem.
 */    
template <class Ty> class SingletonBase {
public:
  SingletonBase(SingletonBase const &) = delete;
  SingletonBase(SingletonBase&&) = delete;
  
  SingletonBase& operator=(SingletonBase const&) = delete;
  SingletonBase& operator=(SingletonBase&&) = delete;

  static Ty& Instance() {
    static Ty single;
    return single;
  }

protected:
  SingletonBase() = default;
};
}
