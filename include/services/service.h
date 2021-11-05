/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once 

#include "services/iservice.h"

namespace earthxpert {
/**
 *  The class Service implements the template methods.
 */
class EXERVICES_EXPORTS Service : public IService {
public:
  virtual ~Service() = default;

private:
  void Initialize();
};

using UPService = std::unique_ptr<Service>;
using SPService = std::shared_ptr<Service>;
} // namespace earthxpert
