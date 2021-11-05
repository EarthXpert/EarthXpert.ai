/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once 

#include "services/exservices.h"

#include <memory>

namespace earthxpert {
  /**
   *  The interface IService defines behaviours of RESTful API.
   */
  class EXERVICES_EXPORTS IService {
  public:
    virtual ~IService() = default;
  };

  using UPIService = std::unique_ptr<IService>;
  using SPIService = std::shared_ptr<IService>;
} // namespace earthxpert
