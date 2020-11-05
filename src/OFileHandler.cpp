/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wieﬂneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: OFileHandler.cpp
*	Author: Benjamin Wieﬂneth
*	Description: This class serves as a base class for all output file
*	generators. It offers the creation and writing of files.
*
*****************************************************************************
*
*	This file is part of VEC
*	VEC is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published
*	by the Free Software Foundation, either version 3 of the License,
*	or (at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program. If not, see <http://www.gnu.org/licenses/>.
*
\***************************************************************************/

#include "OFileHandler.h"
#include "Config.h"
#include "MSG.h"
#include "tools.h"
#include <stdlib.h>
#include <sys/stat.h>

// ...
OFileHandler::OFileHandler(VHDLEntity sourceEntity,
                           std::string configName,
                           std::string extension,
                           std::string suffix)
  : mSourceEntity(sourceEntity)
  , mConfigName(configName)
  , mExtension(extension)
  , mSuffix(suffix)
{
  MSG(LOG_LEVEL::DEBUG, DEBUG::FUNCTIONCALL)
    << "OFileHandler::OFileHandler(VHDLEntity sourceEntity)";
  // MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT) << "mConfigName = " << mConfigName;
  // MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT) << "mExtension = " << mExtension;
  // MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT) << "mSuffix = " << mSuffix;

  mOutputDirectory = cleanPath(cfg.getString(mConfigName + ".outputPath"));

  // Build file name string
  if (mSuffix.empty())
    mOutputFileName = mSourceEntity.getEntityName() + "." + mExtension;
  else
    mOutputFileName =
      mSourceEntity.getEntityName() + "." + mSuffix + "." + mExtension;

  if (!mOutputDirectory.empty()) {
#ifdef _WIN32
    mOutputFilePath = mOutputDirectory + "\\" + mOutputFileName;
#else
    mOutputFilePath = mOutputDirectory + "/" + mOutputFileName;
#endif
  } else {
#ifdef _WIN32
    mOutputFilePath = mOutputFileName;
#else
    mOutputFilePath = mOutputFileName;
#endif
  }

  createOutputDirectory();

  // MSG(LOG_LEVEL::INFO) << TAB << "filename = " << mOutputFileName;
  // MSG(LOG_LEVEL::INFO) << TAB << "directory = " << mOutputDirectory;
  MSG(LOG_LEVEL::INFO) << TAB << "filepath = " << mOutputFilePath;

  mOutputFile.open((char*)mOutputFilePath.c_str());
}

// Destructor
OFileHandler::~OFileHandler()
{
  MSG(LOG_LEVEL::DEBUG, DEBUG::FUNCTIONCALL) << "OFileHandler::~OFileHandler()";

  mOutputFile.close();
}

int
OFileHandler::createOutputDirectory()
{
  // Add trailing slash to output path if there's none
#ifdef _WIN32
  if (!(mOutputDirectory.empty()) &&
      (mOutputDirectory[mOutputDirectory.length() - 1] != '\\'))
    mOutputDirectory.append("\\");
#else
  if (!(mOutputDirectory.empty()) &&
      (mOutputDirectory[mOutputDirectory.length() - 1] != '/'))
    mOutputDirectory.append("/");
#endif

  // Check if directory already exists
  struct stat st;
  if (stat((char*)(mOutputDirectory).c_str(), &st) == 0) {
    MSG(LOG_LEVEL::DEBUG) << "Output directory already exists";
  } else {
    // Create new directory if required
    if (!mOutputDirectory.empty()) {
      // Build arg string. Suppress output and stderr
#ifdef _WIN32
      std::string cmd = "mkdir \"" + mOutputDirectory + "\" > nul 2> nul";
#else
      std::string cmd =
        "mkdir \"" + mOutputDirectory + "\" >/dev/null 2>/dev/null";
#endif
      MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT)
        << "mkdir " << cmd << " = " << system(cmd.c_str());
    }
  }

  return 0;
}
