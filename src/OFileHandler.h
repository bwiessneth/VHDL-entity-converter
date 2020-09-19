/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wieﬂneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: OFileHandler.h
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

#ifndef OFILEHANDLER_H
#define OFILEHANDLER_H

#include "EntityPort.h"
#include "VHDLEntity.h"
#include "tools.h"
#include <fstream>
#include <ostream>
#include <string>

class OFileHandler
{
protected:
  ofstream mOutputFile;

  VHDLEntity mSourceEntity;
  std::string mOutputDirectory;
  std::string mOutputFileName;
  std::string mOutputFilePath;
  int createOutputDirectory();
  std::string mConfigName;
  std::string mExtension;
  std::string mSuffix;

  size_t colWidthName;
  size_t colWidthType;
  size_t colWidthDirection;
  size_t colWidthPolarity;
  size_t colWidthDescription;
  size_t colWidthBlank;
  size_t colWidthGenericName;
  size_t colWidthGenericType;
  size_t colWidthDefaultValue;

  enum colNames
  {
    Name,
    Type,
    Direction,
    Polarity,
    Description,
    Blank,
    GenericName,
    GenericType,
    DefaultValue
  };

public:
  OFileHandler(VHDLEntity sourceEntity,
               std::string configName = "",
               std::string extension = "txt",
               std::string suffix = "");
  ~OFileHandler();
};

#endif
