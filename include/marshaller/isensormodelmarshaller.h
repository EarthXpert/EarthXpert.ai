/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/exmarshaller.h"

#include <memory>
#include <string>

namespace earthxpert {
class ISensorModel;

/**
 *  The interface ISensorModelMarshaller defines the behaviours of
 *  serializing and deserializing sensor models.
 */
class EXMARSHALLER_EXPORTS ISensorModelMarshaller {
public:
  virtual ~ISensorModelMarshaller() {}

  /**
   *  \brief
   */
  virtual void Serialize(const std::string &modelFile,
                         const ISensorModel *const sensorModel) const = 0;

  /**
   *  \brief
   */
  virtual std::unique_ptr<ISensorModel> Deserialize(
    const std::string &modelFile) const = 0;
};
} // namespace earthxpert
