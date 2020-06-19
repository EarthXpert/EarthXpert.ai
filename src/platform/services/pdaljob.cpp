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

#include "platform/services/pdaljob.h"

#include <pdal/GDALUtils.hpp>
#include <pdal/Kernel.hpp>
#include <pdal/pdal_config.hpp>
#include <pdal/PluginManager.hpp>
#include <pdal/StageFactory.hpp>
#include <pdal/util/Backtrace.hpp>

#include <nlohmann/json.hpp>
#include <iomanip>
#include <memory>
#include <sstream>
#include <vector>

#ifndef _WIN32
#include <csignal>
#include <unistd.h>
#endif

using earthxpert::PdalJob;
using namespace pdal;

PdalJob::PdalJob(std::ostream& out) : m_out(out) {
  m_headline = std::string(Utils::screenWidth(), '-');
  addArgs();
  m_logger.reset( new pdal::Log("PDAL", "stderr") );
}

int PdalJob::SetArgs(const StringList& cmdArgs) {
  m_cmdArgs = cmdArgs;

  try {
    m_args.parseSimple(m_cmdArgs);
  }
  catch (arg_val_error const& e) {
    Utils::printError(e.what());
    return -1;
  }

  m_logger.reset(new Log("PDAL", m_log, m_logtiming));
  if (m_logLevel != LogLevel::None)
    m_logger->setLevel(m_logLevel);
  else if (m_debug)
    m_logger->setLevel(LogLevel::Debug);

  m_logger->get(LogLevel::Debug) << "Debugging..." << std::endl;
  PluginManager<Stage>::setLog(m_logger);
  PluginManager<Kernel>::setLog(m_logger);

#ifndef _WIN32
  if (m_debug) {
    signal(SIGSEGV, [](int sig) {
      m_logger->get(LogLevel::Debug) << "Segmentation fault (signal 11)\n";
      StringList lines = Utils::backtrace();

      for (const auto& l : lines)
        m_logger->get(LogLevel::Debug) << l << std::endl;
      exit(1); });
  }
#endif
  return 0;
}

int PdalJob::Run() {
  m_command = Utils::tolower(m_command);
  if (!m_command.empty()) {
    int ret = 0;
    std::string name("kernels." + m_command);

    std::unique_ptr<Kernel> kernel{ PluginManager<Kernel>::createObject(name) };
    if (kernel) {
      if (m_help)
        m_cmdArgs.push_back("--help");

      // This shouldn't throw.  If it does, it's something awful, so
      // not cleaning up seems inconsequential.
      m_logger->setLeader("pdal " + m_command);
      ret = kernel->run(m_cmdArgs, m_logger);

      // IMPORTANT - The kernel must be destroyed before GDAL
      //  drivers are unregistered or GDAL will attempt to destroy
      //  resources more than once, resulting in a crash.
      gdal::unregisterDrivers();
    }
    else
      m_logger->get(LogLevel::Error) << "Command '" << m_command <<
      "' not recognized" << std::endl << std::endl;
    return ret;
  }

  if (m_showVersion)
    outputVersion();
  else if (m_showDrivers)
    outputDrivers();
  else if (m_showOptions.size()) {
    if (m_showOptions == "all")
      outputOptions();
    else
      outputOptions(m_showOptions, m_out);
  }
  else
    outputHelp();
  return 0;
}

void PdalJob::addArgs() {
  m_args.add("command", "The PDAL command", m_command).setPositional();
  m_args.add("debug", "Sets the output level to 3 (option deprecated)",
    m_debug);
  m_args.add("verbose,v", "Sets the output level (0-8)", m_logLevel,
    LogLevel::None);
  m_args.add("drivers", "List available drivers", m_showDrivers);
  m_args.add("help,h", "Display help text", m_help);
  m_args.add("list-commands", "List available commands", m_showCommands);
  m_args.add("version", "Show program version", m_showVersion);
  m_args.add("options", "Show options for specified driver (or 'all')",
    m_showOptions);
  m_args.add("log", "Log filename (accepts stderr, stdout, stdlog, devnull"
    " as special cases)", m_log, "stderr");
  m_args.add("logtiming", "Turn on timing for log messages", m_logtiming);
  Arg& json = m_args.add("showjson", "List options or drivers as JSON output",
    m_showJSON);
  json.setHidden();
}

void PdalJob::outputVersion() {
  m_out << m_headline << std::endl;
  m_out << "pdal " << Config::fullVersionString() << std::endl;
  m_out << m_headline << std::endl;
  m_out << std::endl;
}

void PdalJob::outputHelp() {
  m_out << "Usage:" << std::endl;
  m_out << "  pdal <options>" << std::endl;
  m_out << "  pdal <command> <command options>" << std::endl;

  m_args.dump(m_out, 2, 80);
  m_out << std::endl;

  m_out << "The following commands are available:" << std::endl;

  // Load all kernels so that we can report the names.
  StageFactory f;
  PluginManager<Kernel>::loadAll();
  outputCommands("  - ");
  m_out << std::endl;
  m_out << "See http://pdal.io/apps/ for more detail" << std::endl;
}

void PdalJob::outputCommands(const std::string& leader) {
  StageFactory f;
  PluginManager<Kernel>::loadAll();
  std::string kernelbase("kernels.");

  for (auto name : PluginManager<Kernel>::names()) {
    if (Utils::startsWith(name, kernelbase))
      name = name.substr(kernelbase.size());
    m_out << leader << name << std::endl;
  }
}

void PdalJob::outputDrivers() {
  // Force plugin loading.
  StageFactory f;
  PluginManager<Stage>::loadAll();
  StringList stages = PluginManager<Stage>::names();

  if (!m_showJSON) {
    int nameColLen(28);
    int descripColLen(Utils::screenWidth() - nameColLen - 1);

    std::string tablehead(std::string(nameColLen, '=') + ' ' +
      std::string(descripColLen, '='));

    m_out << std::endl;
    m_out << tablehead << std::endl;
    m_out << std::left << std::setw(nameColLen) << "Name" <<
      " Description" << std::endl;
    m_out << tablehead << std::endl;
    m_out << std::left;

    for (auto name : stages) {
      std::string descrip = PluginManager<Stage>::description(name);
      StringList lines = Utils::wordWrap(descrip, descripColLen - 1);
      for (size_t i = 0; i < lines.size(); ++i) {
        m_out << std::setw(nameColLen) << name << " " <<
          lines[i] << std::endl;
        name.clear();
      }
    }

    m_out << tablehead << std::endl << std::endl;
  }
  else {
    nlohmann::json j;
    for (auto& name : stages) {
      std::string description = PluginManager<Stage>::description(name);
      std::string link = PluginManager<Stage>::link(name);
      j.push_back({ {"name", name},
                    {"description", description},
                    {"link", link} });
    }
    m_out << j;
  }
}

void PdalJob::outputOptions(std::string const& stageName, std::ostream& strm) {
  // Force plugin loading.
  StageFactory f(false);

  Stage* s = f.createStage(stageName);
  if (!s) {
    std::cerr << "Unable to create stage " << stageName << "\n";
    return;
  }

  s->addAllArgs(m_args);

  if (!m_showJSON) {
    strm << stageName << " -- " << PluginManager<Stage>::link(stageName) <<
      std::endl;
    strm << m_headline << std::endl;

    m_args.dump2(strm, 2, 6, m_headline.size());
  }
  else {
    std::ostringstream ostr;
    m_args.dump3(ostr);

    nlohmann::json array;
    try {
      array = nlohmann::json::parse(ostr.str());
    }
    catch (nlohmann::json::parse_error&) {
    }

    nlohmann::json object = { stageName, array };
    strm << object;
  }
}

void PdalJob::outputOptions() {
  // Force plugin loading.
  StageFactory f(false);
  StringList nv = PluginManager<Stage>::names();

  if (!m_showJSON) {
    for (auto const& n : nv) {
      outputOptions(n, m_out);
      m_out << std::endl;
    }
  }
  else {
    std::stringstream strm;
    nlohmann::json options;
    for (auto const& n : nv) {
      outputOptions(n, strm);
      nlohmann::json j;
      try {
        strm >> j;
      }
      catch (nlohmann::json::parse_error&) {
      }
      options.push_back(j);
      strm.str("");
    }
    m_out << options;
  }
}
