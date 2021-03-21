/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wießneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: MSG.h
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

#ifndef MSG_H
#define MSG_H
#include <iomanip>
#include <iostream>
#include <string>

// Define control sequence for a newline
#define NL "\r\n"

#define INDENT std::setw(10) << " " // INDENT for console output
#define TAB "    "                  // Tabulator for console output

#define DEFAULT_LOG_LEVEL LOG_LEVEL::WARNING
#define DEFAULT_DEBUG_LEVEL DEBUG::NONE

// Class for all log levels
enum class LOG_LEVEL
{
  ERROR,
  WARNING,
  INFO,
  DEBUG,
};

// Class for all debug levels
enum class DEBUG
{
  NONE,
  FUNCTIONCALL,
  RESULT
};

class MSG
{
public:
  MSG(LOG_LEVEL log_level, DEBUG dbg_level = DEFAULT_DEBUG_LEVEL);
  ~MSG();

  template<typename T>
  MSG& operator<<(T value);

  // Set global message level
  static void setLogLevel(LOG_LEVEL level = DEFAULT_LOG_LEVEL);

  // Set global debug filter
  static void setDebugLevel(DEBUG level = DEFAULT_DEBUG_LEVEL);

  // Return global message level
  static LOG_LEVEL getLogLevel();
  static int getLogLevelAsInt();

  // Return global debug filter
  static DEBUG getDebugLevel();
  static int getDebugLevelAsInt();

private:
  // Flag if current call will pass threshold and produce output
  bool validOutput = false;

  // Object-spcific log level
  LOG_LEVEL logLevel;

  // Object-spcific debug level
  DEBUG debugLevel;

  // Global maximum log level
  static LOG_LEVEL MaxLogLevel;

  // Global maximum debug level
  static DEBUG MaxDebugLevel;
};

// Operator << overload to enable consecutive calls e.g. MSG() << "some" <<
// "more";
template<typename T>
MSG&
MSG::operator<<(T value)
{
  if (logLevel != LOG_LEVEL::DEBUG) {
    if (logLevel <= MaxLogLevel) {
      std::cout << value;
    }
  } else if (logLevel == LOG_LEVEL::DEBUG) {
    if (validOutput) {
      std::cout << value;
    }
  }
  return *this;
}

#endif
