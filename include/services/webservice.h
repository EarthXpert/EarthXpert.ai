/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once 

#include "services/service.h"

namespace earthxpert {
  /**
   *  The class WebService implements the RESTful API methods.
   */
  class EXERVICES_EXPORTS WebService final : public Service {
  public:
    ~WebService() = default;

    void Get() {}
    void Post() {}
    void Update() {}
    void Delete() {}

  private:
    void Initialize();
  };

  using UPWebService = std::unique_ptr<WebService>;
  using SPWebService = std::shared_ptr<WebService>;
} // namespace earthxpert
