/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
 /******************************************************************************
 * Copyright (c) 2013, Howard Butler (hobu.inc@gmail.com)
 * Copyright (c) 2014-2015, Bradley J Chambers (brad.chambers@gmail.com)
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following
 * conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
 *       names of its contributors may be used to endorse or promote
 *       products derived from this software without specific prior
 *       written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 ****************************************************************************/
#pragma once 

#include "packages/pdal/expdal.h"
#include "3rdparty/pdal/expdal.h"
#include "core/parallel/ijob.h"
#include <pdal/Log.hpp>
#include <pdal/util/ProgramArgs.hpp>

#include <iostream>
#include <string>

using pdal::LogLevel;
using pdal::ProgramArgs;
using pdal::StringList;

namespace earthxpert {
/**
 *  \brief the PDAL tools pdal is a full featured CLI, and is copied here 
 *  for easier customization. The coding style is un-touched for upgrading.
 */
class EXPDAL_EXPORTS PdalJob final : public IJob {
public:
  PdalJob(std::ostream& out = std::cout);
  PdalJob(const PdalJob&) = delete;
  PdalJob(PdalJob&&) = delete;
  ~PdalJob() = default;

  PdalJob& operator=(const PdalJob&) = delete;
  PdalJob& operator=(PdalJob&&) = delete;

  int SetCmdArgs(int argc, char** argv);

  int Run() override;

  int operator()(int argc, char** argv) {
    SetCmdArgs(argc, argv);
    return Run();
  }

private:
  std::ostream& mOut;

  StringList mCmdArgs;
  ProgramArgs mArgs;
  
  bool mHelp{};
  bool mDebug{};
  bool mShowVersion{};
  bool mShowCommands{};
  bool mShowDrivers{};
  bool mShowJSON{};
  bool mLogtiming{};

  LogLevel mLogLevel{ LogLevel::Error };
  pdal::LogPtr mLogger;

  std::string mCommand;
  std::string mShowOptions;
  std::string mLog;
  std::string mHeadline;

  void AddCmdArgs();
  void Help();
  void HelpVersion();
  void HelpCommands(const std::string& leader);
  void HelpDrivers();
  void HelpOptions();
  void HelpOptions(const std::string& stageName, std::ostream& strm);
};
} // namespace earthxpert::thirdparty
