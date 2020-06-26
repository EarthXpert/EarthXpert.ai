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

#include "platform/pdal/pdaljob.h"

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

PdalJob::PdalJob(std::ostream& out) : mOut(out) {
  mHeadline = std::string(Utils::screenWidth(), '-');
  AddArgs();
  mLogger.reset( new pdal::Log("PDAL", "stderr") );
}

int PdalJob::SetArgs(const StringList& cmdArgs) {
  mCmdArgs = cmdArgs;

  try {
    mArgs.parseSimple(mCmdArgs);
  }
  catch (arg_val_error const& e) {
    Utils::printError(e.what());
    return -1;
  }

  mLogger.reset(new Log("PDAL", mLog, mLogtiming));
  if (mLogLevel != LogLevel::None)
    mLogger->setLevel(mLogLevel);
  else if (mDebug)
    mLogger->setLevel(LogLevel::Debug);

  mLogger->get(LogLevel::Debug) << "Debugging..." << std::endl;
  PluginManager<Stage>::setLog(mLogger);
  PluginManager<Kernel>::setLog(mLogger);

#ifndef _WIN32
  if (mDebug) {
    signal(SIGSEGV, [](int sig) {
      mLogger->get(LogLevel::Debug) << "Segmentation fault (signal 11)\n";
      StringList lines = Utils::backtrace();

      for (const auto& l : lines)
        mLogger->get(LogLevel::Debug) << l << std::endl;
      exit(1); });
  }
#endif
  return 0;
}

int PdalJob::Run() {
  mCommand = Utils::tolower(mCommand);
  if (!mCommand.empty()) {
    int ret = 0;
    std::string name("kernels." + mCommand);

    std::unique_ptr<Kernel> kernel{ PluginManager<Kernel>::createObject(name) };
    if (kernel) {
      if (mHelp)
        mCmdArgs.push_back("--help");

      // This shouldn't throw.  If it does, it's something awful, so
      // not cleaning up seems inconsequential.
      mLogger->setLeader("pdal " + mCommand);
      ret = kernel->run(mCmdArgs, mLogger);

      // IMPORTANT - The kernel must be destroyed before GDAL
      //  drivers are unregistered or GDAL will attempt to destroy
      //  resources more than once, resulting in a crash.
      gdal::unregisterDrivers();
    }
    else
      mLogger->get(LogLevel::Error) << "Command '" << mCommand <<
      "' not recognized" << std::endl << std::endl;
    return ret;
  }

  if (mShowVersion)
    OutputVersion();
  else if (mShowDrivers)
    OutputDrivers();
  else if (mShowOptions.size()) {
    if (mShowOptions == "all")
      OutputOptions();
    else
      OutputOptions(mShowOptions, mOut);
  }
  else
    OutputHelp();
  return 0;
}

void PdalJob::AddArgs() {
  mArgs.add("command", "The PDAL command", mCommand).setPositional();
  mArgs.add("debug", "Sets the output level to 3 (option deprecated)",
    mDebug);
  mArgs.add("verbose,v", "Sets the output level (0-8)", mLogLevel,
    LogLevel::None);
  mArgs.add("drivers", "List available drivers", mShowDrivers);
  mArgs.add("help,h", "Display help text", mHelp);
  mArgs.add("list-commands", "List available commands", mShowCommands);
  mArgs.add("version", "Show program version", mShowVersion);
  mArgs.add("options", "Show options for specified driver (or 'all')",
    mShowOptions);
  mArgs.add("log", "Log filename (accepts stderr, stdout, stdlog, devnull"
    " as special cases)", mLog, "stderr");
  mArgs.add("logtiming", "Turn on timing for log messages", mLogtiming);
  Arg& json = mArgs.add("showjson", "List options or drivers as JSON output",
    mShowJSON);
  json.setHidden();
}

void PdalJob::OutputVersion() {
  mOut << mHeadline << std::endl;
  mOut << "pdal " << Config::fullVersionString() << std::endl;
  mOut << mHeadline << std::endl;
  mOut << std::endl;
}

void PdalJob::OutputHelp() {
  mOut << "Usage: pdal <options>" << std::endl;
  mOut << "       pdal <command> <command options>" << std::endl;

  mArgs.dump(mOut, 2, 80);
  mOut << std::endl;

  mOut << "Commands:" << std::endl;

  // Load all kernels so that we can report the names.
  StageFactory f;
  PluginManager<Kernel>::loadAll();
  OutputCommands("    ");
  mOut << std::endl;
  mOut << "See http://pdal.io/apps/ for more detail" << std::endl;
}

void PdalJob::OutputCommands(const std::string& leader) {
  StageFactory f;
  PluginManager<Kernel>::loadAll();
  std::string kernelbase("kernels.");

  for (auto name : PluginManager<Kernel>::names()) {
    if (Utils::startsWith(name, kernelbase))
      name = name.substr(kernelbase.size());
    mOut << leader << name << std::endl;
  }
}

void PdalJob::OutputDrivers() {
  // Force plugin loading.
  StageFactory f;
  PluginManager<Stage>::loadAll();
  StringList stages = PluginManager<Stage>::names();

  if (!mShowJSON) {
    int nameColLen(28);
    int descripColLen(Utils::screenWidth() - nameColLen - 1);

    std::string tablehead(std::string(nameColLen, '=') + ' ' +
      std::string(descripColLen, '='));

    mOut << std::endl;
    mOut << tablehead << std::endl;
    mOut << std::left << std::setw(nameColLen) << "Name" <<
      " Description" << std::endl;
    mOut << tablehead << std::endl;
    mOut << std::left;

    for (auto name : stages) {
      std::string descrip = PluginManager<Stage>::description(name);
      StringList lines = Utils::wordWrap(descrip, descripColLen - 1);
      for (size_t i = 0; i < lines.size(); ++i) {
        mOut << std::setw(nameColLen) << name << " " <<
          lines[i] << std::endl;
        name.clear();
      }
    }

    mOut << tablehead << std::endl << std::endl;
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
    mOut << j;
  }
}

void PdalJob::OutputOptions(std::string const& stageName, std::ostream& strm) {
  // Force plugin loading.
  StageFactory f(false);

  Stage* s = f.createStage(stageName);
  if (!s) {
    std::cerr << "Unable to create stage " << stageName << "\n";
    return;
  }

  s->addAllArgs(mArgs);

  if (!mShowJSON) {
    strm << stageName << " -- " << PluginManager<Stage>::link(stageName) <<
      std::endl;
    strm << mHeadline << std::endl;

    mArgs.dump2(strm, 2, 6, mHeadline.size());
  }
  else {
    std::ostringstream ostr;
    mArgs.dump3(ostr);

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

void PdalJob::OutputOptions() {
  // Force plugin loading.
  StageFactory f(false);
  StringList nv = PluginManager<Stage>::names();

  if (!mShowJSON) {
    for (auto const& n : nv) {
      OutputOptions(n, mOut);
      mOut << std::endl;
    }
  }
  else {
    std::stringstream strm;
    nlohmann::json options;
    for (auto const& n : nv) {
      OutputOptions(n, strm);
      nlohmann::json j;
      try {
        strm >> j;
      }
      catch (nlohmann::json::parse_error&) {
      }
      options.push_back(j);
      strm.str("");
    }
    mOut << options;
  }
}
