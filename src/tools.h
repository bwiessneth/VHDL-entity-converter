/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wießneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: tools.h
*	Author: Benjamin Wießneth
*	Description: Misc methods used by VEC main()
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


#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <stdio.h>
#include <vector>

extern std::string configFile;		// Path of the config file
extern std::string entityLabel;		// Label for the parsed entity
extern std::string output_path_dokuwiki; // Path for doku wiki files
extern std::string output_path_svg; // Path for converted svg files
extern std::string output_path_png; // Path for converted png files
extern std::string baseDirectory; // Directory from where the executable is launched

// Checks extension from given file name for valid vhdl extension
bool validFileExtension(std::string fileName);

// Extracts extension from given file name
const char *getFileNameExtension(const char *fileName);

// Checks passed path for trailing backslash and removes it
std::string cleanPath(std::string Path);

// Check programm arguments for programm options and source files
int parseArgv(int argc, const char* argv[], std::vector<std::string>& fList);

// Print info dialog
void printInfo();

// Print help dialog
void printHelp();

// Print license dialog
void printLicense();


void replaceAll(std::string& str, const std::string& from, const std::string& to);

std::string replaceStr(std::string source, std::string from, std::string to);
#endif