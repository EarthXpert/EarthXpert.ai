/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/exmarshaller.h"
#include "epipolarmodel/epipolarmodel.h"

#include <memory>

namespace earthxpert {

/**
 *  The class EpipolarModelMarshaller serialize and deserialize the
 *  epipolar model and links to associated sensor models.
 */
class EXMARSHALLER_EXPORTS IEpipolarModelMarshaller {
public:
  ~IEpipolarModelMarshaller() {}

  /**
   *  \brief
   */
  virtual bool Serialize(const std::string &modelFile,
                         IEpipolarModel const *const epiModel) const = 0;

  /**
   *  \brief
   */
  virtual std::unique_ptr<IEpipolarModel>
  Deserialize(const std::string &modelFile) const = 0;
};
} // namespace earthxpert
