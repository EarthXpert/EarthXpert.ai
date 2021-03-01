/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "dem/idem.h"
#include "sensormodel/isensormodel.h"

namespace earthxpert {
/**
 *
 */
class EXDEM_EXPORTS ImageToDem {
public:
  ImageToDem(ISensorModel const* model, IDem const* dem);
  ~ImageToDem() = default;

private:
  const ISensorModel* mModel{};
  const IDem* mDem{};

  void Initialize();
};

using UPImageToDem = std::unique_ptr<ImageToDem>;
using SPImageToDem = std::shared_ptr<ImageToDem>;
}
