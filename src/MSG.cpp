/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wießneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: MSG.cpp
*	Author: Benjamin Wießneth
*	Description: Message method to create console output
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

#include "MSG.h"
#include "VEC.h"
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// Setup static members of the MSG class
LOG_LEVEL MSG::MaxLogLevel = DEFAULT_LOG_LEVEL;
DEBUG MSG::MaxDebugLevel = DEFAULT_DEBUG_LEVEL;

MSG::~MSG()
{
  if (validOutput)
    std::cout << std::endl << std::flush;
}

MSG::MSG(LOG_LEVEL log_level, DEBUG dbg_level)
  : logLevel(log_level)
  , debugLevel(dbg_level)
{
  // Check if current log level passes threshold
  switch (logLevel) {
    case LOG_LEVEL::ERROR:
      // Always show errors
      std::cout << std::setw(10) << std::left << "\033[31m[Error]\033[0m ";
      validOutput = true;
      break;
    case LOG_LEVEL::WARNING:
      if (MaxLogLevel != LOG_LEVEL::ERROR) {
        std::cout << std::setw(10) << std::left << "\033[33m[Warning]\033[0m ";
        validOutput = true;
      }
      break;
    case LOG_LEVEL::INFO:
      if (MaxLogLevel == LOG_LEVEL::DEBUG || MaxLogLevel == LOG_LEVEL::INFO) {
        std::cout << std::setw(10) << std::left << "\033[32m[Info]\033[0m ";
        validOutput = true;
      }
      break;
    case LOG_LEVEL::DEBUG:
      if (MaxLogLevel == LOG_LEVEL::DEBUG) {
        if (MaxDebugLevel != DEBUG::NONE || debugLevel == DEBUG::NONE) {
          std::cout << std::setw(10) << std::left << "\033[36m[Debug]\033[0m ";
          validOutput = true;
        }
      }
      break;
  }
}

// Set global message level
void
MSG::setLogLevel(LOG_LEVEL level)
{
  // Set global log level threshold
  MaxLogLevel = level;
}

// Set global debug filter
void
MSG::setDebugLevel(DEBUG level)
{
  // Set global debug level filter
  MaxDebugLevel = level;
}

// Return global message level
LOG_LEVEL
MSG::getLogLevel()
{
  return MaxLogLevel;
}

int
MSG::getLogLevelAsInt()
{
  switch (MaxLogLevel) {
    case LOG_LEVEL::ERROR:
      return 0;
    case LOG_LEVEL::WARNING:
      return 1;
    case LOG_LEVEL::INFO:
      return 2;
    case LOG_LEVEL::DEBUG:
      return 3;
  }
  return -1;
}

// Return global debug filter
DEBUG
MSG::getDebugLevel()
{
  return MaxDebugLevel;
}

int
MSG::getDebugLevelAsInt()
{
  switch (MaxDebugLevel) {
    case DEBUG::NONE:
      return 0;
    case DEBUG::FUNCTIONCALL:
      return 1;
    case DEBUG::RESULT:
      return 2;
  }
  return -1;
}
