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

#include "3rdparty/pdal/expdal.h"
#include <pdal/Log.hpp>
#include <pdal/util/ProgramArgs.hpp>

#include <iostream>
#include <string>

namespace earthxpert::thirdparty {
/**
 *  \brief the PDAL tools pdal is a full featured CLI, and is copied and
 *  refactored here for easier customization.
 */
class EX3RDPARTY_EXPORTS PdalApp {
public:
  PdalApp() : m_out(std::cout) {}
  ~PdalApp() = default;

  int execute(pdal::StringList& cmdArgs, pdal::LogPtr& log);

private:
  std::ostream& m_out;

  std::string m_command;
  bool m_help;
  bool m_debug;
  bool m_showVersion;
  bool m_showCommands;
  bool m_showDrivers;
  bool m_showJSON;
  bool m_logtiming;
  pdal::LogLevel m_logLevel;
  std::string m_showOptions;
  std::string m_log;

  void outputVersion();
  void outputHelp(const pdal::ProgramArgs& args);
  void outputCommands(const std::string& leader);
  void outputDrivers();
  void outputOptions();
  void outputOptions(const std::string& stageName, std::ostream& strm);
  void addArgs(pdal::ProgramArgs& args);
};
} // namespace earthxpert::thirdparty
