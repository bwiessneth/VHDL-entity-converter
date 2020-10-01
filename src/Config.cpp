/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wieﬂneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: Config.cpp
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

#include "Config.h"
#include "MSG.h"

// Initialize a 2-dim string array which includes all configuration keys and
// their default value
std::string Config::configurationKeys[][2] = {
  { "HIGHActiveSuffix", "_pi" },
  { "LOWActiveSuffix", "_ni" },

  { "resetName", "rst_ni" },
  { "clockName", "clk_i" },

  { "default_label", "" },

  { "vertical_Margin", "0.5" },
  { "horizontal_Margin", "0.5" },
  { "fixedWidth", "0" },

  { "label.fontFamily", "Courier New" },
  { "label.fontSize", "10" },
  { "label.fontWeight", "normal" },
  { "label.stroke", "none" },
  { "label.strokeWidth", "0.00cm" },
  { "label.strokeColor", "#000000" },
  { "label.fill", "none" },
  { "label.fillColor", "#FFFFFF" },

  { "entity.fontFamily", "Arial" },
  { "entity.fontSize", "12" },
  { "entity.fontWeight", "bold" },
  { "entity.stroke", "solid" },
  { "entity.strokeWidth", "0.03cm" },
  { "entity.strokeColor", "#000000" },
  { "entity.fill", "solid" },
  { "entity.fillColor", "#FFFFFF" },

  { "port.fontFamily", "Arial" },
  { "port.fontSize", "12" },
  { "port.fontWeight", "normal" },
  { "port.stroke", "none" },
  { "port.strokeWidth", "0.01cm" },
  { "port.strokeColor", "#00CC00" },
  { "port.fill", "none" },
  { "port.fillColor", "#FFFFFF" },

  { "connector.stroke", "solid" },
  { "connector.strokeWidth", "0.01cm" },
  { "connector.strokeColor", "#000000" },
  { "connector.fill", "solid" },
  { "connector.fillColor", "#FFFFFF" },

  { "generics.stroke", "solid" },
  { "generics.strokeWidth", "0.02cm" },
  { "generics.strokeColor", "#BFBFBF" },
  { "generics.fill", "solid" },
  { "generics.fillColor", "#F2F2F2" },

  { "vector.fontFamily", "Arial" },
  { "vector.fontSize", "10" },
  { "vector.fontWeight", "normal" },

  { "genericSignal.fontFamily", "Arial" },
  { "genericSignal.fontSize", "10" },
  { "genericSignal.fontWeight", "normal" },
  { "genericSignal.stroke", "none" },
  { "genericSignal.strokeWidth", "0.00cm" },
  { "genericSignal.strokeColor", "#FFFFFF" },
  { "genericSignal.fill", "none" },
  { "genericSignal.fillColor", "#FFFFFF" },

  { "DokuWiki.enableExport", "1" },
  { "DokuWiki.outputPath", "DokuWiki" },

  { "Markdown.enableExport", "1" },
  { "Markdown.outputPath", "MD" },

  { "LaTeX.enableExport", "1" },
  { "LaTeX.outputPath", "LaTeX" },
  { "LaTeX.addTable", "1" },
  { "LaTeX.centering", "1" },
  { "LaTeX.caption", "1" },
  { "LaTeX.label", "1" },

  { "Table.exportType", "1" },
  { "Table.exportDirection", "1" },
  { "Table.exportPolarity", "1" },
  { "Table.exportDescription", "1" },
  { "Table.exportBlank1", "0" },
  { "Table.exportBlank2", "0" },
  { "Table.exportGenerics", "1" },
  { "Table.combineNameAndType", "0" },
  { "Table.arrayNotation", "1" },
  { "Table.showArrayLength", "1" },
  { "Table.boldHeadings", "1" },
  { "Table.centeredHeadings", "1" },
  { "Table.centeredName", "0" },
  { "Table.centeredType", "0" },
  { "Table.centeredDirection", "0" },
  { "Table.centeredPolarity", "0" },
  { "Table.centeredDescription", "0" },
  { "Table.centeredBlank", "0" },
  { "Table.centeredGenericName", "0" },
  { "Table.centeredGenericType", "0" },
  { "Table.centeredGenericDefaultValue", "0" },

  { "Table.Name_heading", "Name" },
  { "Table.Type_heading", "Type" },
  { "Table.Direction_heading", "Direction" },
  { "Table.Polarity_heading", "Polarity" },
  { "Table.Description_heading", "Description" },
  { "Table.Blank1_heading", "Blank1" },
  { "Table.Blank2_heading", "Blank2" },
  { "Table.GenericName", "Name" },
  { "Table.GenericType", "Type" },
  { "Table.GenericDefaultValue", "Default value" },

  { "Table.caption_IN", "IN" },
  { "Table.caption_OUT", "OUT" },
  { "Table.caption_INOUT", "INOUT" },
  { "Table.caption_BUFFER", "BUFFER" },
  { "Table.caption_LINKAGE", "LINKAGE" },
  { "Table.caption_HIGHactive", "H" },
  { "Table.caption_LOWactive", "L" },

  { "PATH.soffice",
    "C:\\Program Files (x86)\\LibreOffice 4\\program\\soffice.exe" },

  { "FODG.enableExport", "1" },
  { "FODG.outputPath", "FODG" },

  { "PNG.enableExport", "0" },
  { "PNG.outputPath", "PNG" },

  { "SVG.enableExport", "0" },
  { "SVG.outputPath", "SVG" }
};

// Initialize global Config object
// First look for config in the work directory
// Then in the home directory
// Then in the AppImage directory
// Then in the /etc/vec directory
Config cfg(std::vector<std::string>({ "vec.conf",
                                      "~/.config/vec.conf",
                                      "$APPDIR/usr/share/vec/vec.conf",
                                      "/etc/vec/vec.conf" }));

Config::Config(std::string fileName)
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "Config(std::string fileName)";

  // Fill configMap with all default keys and their values
  setDefaultConfigValues();

  // Create ifstream and open the requested file
  std::ifstream f(fileName.c_str());
  if (f) {
    // Copy contents of ifstream to the buffer cF
    cF << f.rdbuf();
    // Close ifstream
    f.close();
  }

  // Check if buffer cF is valid
  if (cF.fail() || cF.bad() || cF.str().empty()) {
    MSG(GROUP::WARNING) << "Can't open config file at \"" << fileName
                        << "\". Using default values.";
  } else {
    // Update the default values with the values found in the config file
    readConfigValues();
    cF.clear();
  }
}

Config::Config(std::vector<std::string> fileNameList)
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "Config(std::string fileName)";

  // Fill configMap with all default keys and their values
  setDefaultConfigValues();

  // Load APPDIR env variable in order to properly run AppImages
  const char* rawEnvAppDir = std::getenv("APPDIR");
  std::string envAppDir;
  if (rawEnvAppDir == NULL)
    envAppDir = std::string("");
  else
    envAppDir = std::string(rawEnvAppDir);

  for (std::string fileName : fileNameList) {

    std::size_t appDirPos = fileName.find("$APPDIR");
    if (appDirPos != std::string::npos) {
      fileName.replace(appDirPos, 7, envAppDir);
    }

    // Create ifstream and open the requested file
    std::ifstream f(fileName.c_str());
    if (f) {
      // Copy contents of ifstream to the buffer cF
      cF << f.rdbuf();
      // Close ifstream
      f.close();
    }

    // Check if buffer cF is valid
    if (!(cF.fail() || cF.bad() || cF.str().empty())) {
      // Update the default values with the values found in the config file
      readConfigValues();
      cF.clear();

      return;
    }
  }
  MSG(GROUP::WARNING) << "Couldn't open config files at:";
  for (std::string fileName : fileNameList) {
    std::cout << " > " << fileName << std::endl;
  }
  MSG(GROUP::WARNING) << "Using default values.";
}

// Create configuration keys and assign the default values
void
Config::setDefaultConfigValues()
{
  MSG(GROUP::DEBUG) << "Config::setDefaultConfigValues()";

  // Get the number of elements of keys to create
  int numberOfElements =
    sizeof(configurationKeys) / sizeof(configurationKeys[0]);

  // Loop through all keys
  for (int i = 0; i < numberOfElements; i++) {
    // Insert new entries into the configuratin map using the current key and
    // the default value
    configMap.insert(
      dataPair(configurationKeys[i][0], configurationKeys[i][1]));
  }

  return;
}

// Get requested string value from configMap
std::string
Config::getString(std::string key)
{
  // Update iterator to point to requested key
  mIt = configMap.find(key);

  // Check if configMap contains requested key
  if (mIt == configMap.end()) {
    // Requested key doesn't exist in configMap. Display an Error and return an
    // empty string
    std::cout << "Unknown key: " << key << " Returning >(string) \"\"<"
              << std::endl;
    return "";
  } else {
    // If the key exists get its value and return it
    return mIt->second;
  }
}

void
Config::setString(std::string key, std::string value)
{
  configMap[key] = value;
}

// Get requested bool value from configMap
bool
Config::getBool(std::string key)
{
  // Update iterator to point to requested key
  mIt = configMap.find(key);

  // Check if configMap contains requested key
  if (mIt == configMap.end()) {
    // Requested key doesn't exist in configMap. Display an Error and return
    // false
    std::cout << "Unknown key: " << key << " Returning >(bool) false<"
              << std::endl;
    return false;
  } else {
    // If the key exists get its value and check if its either "1" or "true" and
    // return true
    std::string configValue = mIt->second;
    std::transform(
      configValue.begin(), configValue.end(), configValue.begin(), ::tolower);

    if (configValue == "true" || configValue == "1")
      return true;
    else
      return false;
  }
}

// Get requested integer value from configMap
int
Config::getInt(std::string key)
{
  // Update iterator to point to requested key
  mIt = configMap.find(key);

  // Check if configMap contains requested key
  if (mIt == configMap.end()) {
    // Requested key doesn't exist in configMap. Display an Error and return -1
    std::cout << "Unknown key: " << key << " Returning >(int) -1<" << std::endl;
    return -1;
  } else {
    // If the key exists get its value and convert the string to an integer type
    return atoi(mIt->second.c_str());
  }
}

// Get requested double value from configMap
double
Config::getDouble(std::string key)
{
  // Update iterator to point to requested key
  mIt = configMap.find(key);

  // Check if configMap contains requested key
  if (mIt == configMap.end()) {
    // Requested key doesn't exist in configMap. Display an Error and return 0
    std::cout << "Unknown key: " << key << " Returning >(double) 0<"
              << std::endl;
    return 0;
  } else {
    // If the key exists get its value and convert the string to a double type
    return atof(mIt->second.c_str());
  }
}

// Get requested float value from configMap
float
Config::getFloat(std::string key)
{
  // Update iterator to point to requested key
  mIt = configMap.find(key);

  // Check if configMap contains requested key
  if (mIt == configMap.end()) {
    // Requested key doesn't exist in configMap. Display an Error and return 0
    std::cout << "Unknown key: " << key << " Returning >(float) 0<"
              << std::endl;
    return 0;
  } else {
    // If the key exists get its value and convert the string to a float type
    return (float)atof(mIt->second.c_str());
  }
}

// Reads custom keys and values from VEC.conf and updates the keys in configMap
void
Config::readConfigValues()
{
  // Get number of default key/value pairs
  int numberOfElements =
    sizeof(configurationKeys) / sizeof(configurationKeys[0]);

  // Loop through all key/value pairs
  for (int i = 0; i < numberOfElements; i++) {
    // Get current key
    std::string currentKey = configurationKeys[i][0];

    // Get the value of current key
    std::string currentValue = readValueString(configurationKeys[i][0]);

    // Update or add the current key/value pair
    configMap[currentKey] = currentValue;
  }
}

// Reads string value from requested configuration key
std::string
Config::readValueString(std::string key)
{
  std::string line = "";
  // Clear ctrl-flags of the buffer and set the get pointer to the beginning
  cF.clear();
  cF.seekg(0, cF.beg);

  // Loop through all lines of the buffer
  while (getline(cF, line)) {
    // Find position of keyword
    int kPos = line.find(key);
    // Check if the key was found
    if (kPos != -1) {
      // Extract contents from the beginning to the occurence of the key
      // This substring could declare the line as a comment
      std::string potComment =
        line.substr(0, kPos); // aquire string between keyword and line break
      // Check if substring contains comment indicating char
      if (potComment.find("#") ==
          std::string::npos) // found no comment in string
      {
        // Extract value after the "="
        std::string val = "";
        size_t fpos = line.find('=');
        if (fpos < line.length() - 1)
          val = line.substr(fpos + 1);
        else
          return "";

        // Strip CR and LF from val
        while ((val[val.length() - 1] == '\n') ||
               (val[val.length() - 1] == '\r'))
          val.erase(val.size() - 1);

        // val.erase(std::remove(val.begin(), val.end(), '\n'), val.end());

        return val;
      }
    }
  }
  return "";
}
