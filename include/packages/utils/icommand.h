/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "packages/expackages.h"
#include "3rdparty/boost/exboost.h"
#include <boost/program_options.hpp>
#include <memory>

namespace bpo = boost::program_options;

namespace earthxpert {
/**
 *
 */
class EXPLATFORM_EXPORTS ICommand {
public:
  virtual ~ICommand() = default;

  /**
   *  Entry point of a command
   */
  virtual int Execute() = 0;

private:
  virtual void AddOptions(bpo::options_description& optDesc) = 0;
  virtual int ExecuteImp() = 0;
};

using UPICommand = std::unique_ptr<ICommand>;
using SPICommand = std::shared_ptr<ICommand>;
}
