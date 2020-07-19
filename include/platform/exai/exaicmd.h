/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "platform/exai/exexai.h"
#include "platform/utils/icommand.h"
#include "platform/utils/commandinfo.h"

namespace earthxpert {
/**
 *
 */
class EXEXAI_EXPORTS ExaiCmd : public ICommand {
public:
  ExaiCmd();
  virtual ~ExaiCmd() = default;

  virtual int Execute() = 0;

protected:
};
}
