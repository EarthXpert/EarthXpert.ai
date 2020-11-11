/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "packages/exai/exexai.h"
#include "packages/utils/icommand.h"
#include "packages/utils/commandinfo.h"

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
