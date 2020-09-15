/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wieﬂneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: Config.h
*	Author: Benjamin Wieﬂneth
*	Description: Read user configuration
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

#ifndef CONFIG_H
#define CONFIG_H

#include <algorithm> // transform()
#include <fstream>   // File input
#include <iostream>  // Generic IO
#include <map>       // map type
#include <sstream>   // File content buffer
#include <string>    // std::string

class Config
{
public:
  // Creates a config reader with given filename
  Config(std::string fileName);

  // Reads bool value from given key.
  bool getBool(std::string key);

  // Reads integer value from given key.
  int getInt(std::string key);

  // Reads double value from given key.
  double getDouble(std::string key);

  // Reads float value from given key.
  float getFloat(std::string key);

  // Reads string value from given key.
  std::string getString(std::string key);

  // Adds or updates the value of the requested key
  void setString(std::string key, std::string value);

private:
  // Buffer for the config file
  std::stringstream cF;

  // Define data type for the entries
  typedef std::pair<std::string, std::string> dataPair;

  // The map which will hold all config keys/values
  std::map<std::string, std::string> configMap;

  // Iterator to navigate within configMap
  std::map<std::string, std::string>::iterator mIt;

  // Adds all default keys and values to configMap
  void setDefaultConfigValues();

  // Reads custom keys and values from VEC.conf and updates the keys in
  // configMap
  void readConfigValues();

  // Reads a single value from the requested key
  std::string readValueString(std::string key);

  // 2-dim array to hold all default keys and their values
  static std::string configurationKeys[][2];
};

// Declare global Config object
extern Config cfg;

#endif
