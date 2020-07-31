/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/iepipolarmodelmarshaller.h"

#include <memory>

namespace earthxpert {

/**
 *  The class EpipolarModelMarshaller serialize and deserialize the
 *  epipolar model and links to associated sensor models.
 */
class EXMARSHALLER_EXPORTS EpipolarModelMarshaller
    : public IEpipolarModelMarshaller {
public:
  ~EpipolarModelMarshaller() = default;

  /**
   *  \brief serialize an epipolarmodel into a json file
   *
   *  \param [in] modelFile name of the file to be serialized to
   *  \param [in] epiModel the epipolar model to be serialized
   */
  bool Serialize(const std::string &modelFile,
                 IEpipolarModel const* epiModel) const override;

  /**
   *  \brief deserialize a json file into an epipolarmodel
   *
   *  \param [in] modelFile name of the file to be deserialized
   *  \return the pointer to the epipolar model
   */
  std::unique_ptr<IEpipolarModel>
  Deserialize(const std::string &modelFile) const override;

private:
};
} // namespace earthxpert
