/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wieﬂneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: tools.cpp
*	Author: Benjamin Wieﬂneth
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


#include "VEC.h"
#include "MSG.h"
#include "tools.h"
#include "Config.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>  //strcmp, strchr
#include <stdio.h>
#include <sys/types.h>

bool enablePNGExport = 0;	// Flag for PNG export
bool enableSVGExport = 0;	// Flag for SVG export
bool enableTableExport = 0;	// Flag for DokuWiki table export
bool enableMDExport = 0;	// Flag for Markdown export
bool enableLaTeXExport = 0;	// Flag for LaTeX export

std::string configFile = "";	  // Path of the config file
std::string entityLabel = "";	  // Label for the parsed entity
std::string output_path_svg = ""; // Path for converted svg files
std::string output_path_png = ""; // Path for converted png files
std::string output_path_dokuwiki = ""; // Path for DokuWiki files

std::string baseDirectory;	// Directory from where the executable is launched


// Checks extension from given file name for valid vhdl extension
bool validFileExtension(std::string fileName)
{
	std::string extension = getFileNameExtension(fileName.c_str());
	if ((strcmp(extension.c_str(),"vhd") == 0) ||
		(strcmp(extension.c_str(),"vhdl") == 0))
		return true;
	else
		return false;
}	


// Extracts extension from given file name
const char *getFileNameExtension(const char *fileName)
{
	// Find last occurence of "." within fileName
	const char *dot = strrchr(fileName, '.');
	// If pointer is null or the same as fileName no "." was found
    if(!dot || dot == fileName)
    	return "";
    else
    // Else return pointer to string after the "." which should be the file extension
    return dot + 1;
}


// Removes extension from given path
std::string truncateFileExtension(std::string filePath)
{	
	// Find last occurence of "." within fileName
	const char *dot = strrchr(filePath.c_str(), '.');
	// Get length of the string from beginning to "."
	int length_extension = strlen(dot);
	// Return substring without file extension
	return filePath.substr(0,filePath.length()-length_extension);;
}


// Removes file name from given path
std::string truncateFileName(std::string filePath)
{
	// Find last occurence of "\" within filePath
	const char *dot = strrchr(filePath.c_str(), '\\');

	int length_path = filePath.length();
	if (dot)
	{
		int length_extension = strlen(dot);
		return filePath.substr(length_path-length_extension+1).append("\\");
	}
	return "";
}


// Removes trailing slash from given path (e.g. input = C:\VEC\ -> output = C:\VEC)
std::string cleanPath(std::string Path)
{
	// If path isn't empty
	if (!Path.empty())
	{
		// Check if last char is a '\' or a '/'
		if ((Path[Path.length() - 1] == '\\') || (Path[Path.length() - 1] == '/'))
			// Return path wihtout the last char
			return Path.substr(0, Path.length() - 1);
	}
	// If no path or a path without the trailing slash was passed return it
	return Path;
}


// Check programm arguments for programm options and source files
int parseArgv(int argc, const char* argv[], std::vector<std::string>& fList)
{
	bool readLabel = false;
	bool readConfig = false;
	bool readFODGOutputPath = false;
	bool readPNGOutputPath = false;
	bool readSVGOutputPath = false;
	bool readDokuWikiOutputPath = false;
	bool readMDOutputPath = false;
	bool readLaTeXOutputPath = false;	
	bool readDBGLevel = false;
	bool readOptionValue = false;
	bool printHelp = false;
	cfg.setString("VEC.printHelp", "false");

	// Loop through all arguments
	for (int currentArgCounter = 1; currentArgCounter < argc; currentArgCounter++)
	{
		// Check if the current argv should be a option value
		if (readOptionValue && argv[currentArgCounter][0] == '-')
		{
			// Detected a new parameter instead => Error
			MSG(GROUP::ERROR) << "Error in program arguments. Maybe missing a parameter? (Check " << argv[currentArgCounter-1] << ")";
			return 1;
		}

		// Set flags or programm options depending on the found strings
		if (strcmp(argv[currentArgCounter], "-d") == 0)
		{
			if (cfg.getBool("DokuWiki.enableExport") == false)
				cfg.setString("DokuWiki.enableExport", "1");
			continue;
		}
		else if (strcmp(argv[currentArgCounter], "-do") == 0)
		{
			readDokuWikiOutputPath = true;
			readOptionValue = true;
			continue;
		}
		else if (strcmp(argv[currentArgCounter], "-f") == 0)
		{
			if (cfg.getBool("FODG.enableExport") == false)
				cfg.setString("FODG.enableExport", "1");
			continue;
		}
		else if (strcmp(argv[currentArgCounter], "-fo") == 0)
		{
			readFODGOutputPath = true;
			readOptionValue = true;
			continue;
		}
		else if (strcmp(argv[currentArgCounter], "-m") == 0)
		{
			if (cfg.getBool("Markdown.enableExport") == false)
				cfg.setString("Markdown.enableExport", "1");
			continue;
		}
		else if (strcmp(argv[currentArgCounter], "-mo") == 0)
		{
			readMDOutputPath = true;
			readOptionValue = true;
			continue;
		}
		else if (strcmp(argv[currentArgCounter], "-p") == 0)
		{
			if (cfg.getBool("PNG.enableExport") == false)
				cfg.setString("PNG.enableExport", "1");
			continue;
		}			
		else if (strcmp(argv[currentArgCounter], "-po") == 0)
		{
			readPNGOutputPath = true;
			readOptionValue = true;
			continue;
		}
		else if (strcmp(argv[currentArgCounter], "-s") == 0)
		{
			if (cfg.getBool("SVG.enableExport") == false)
				cfg.setString("SVG.enableExport", "1");
			continue;
		}
		else if (strcmp(argv[currentArgCounter], "-so") == 0)
		{
			readSVGOutputPath = true;
			readOptionValue = true;
			continue;			
		}
		else if (strcmp(argv[currentArgCounter], "-t") == 0)
		{
			if (cfg.getBool("LaTeX.enableExport") == false)
				cfg.setString("LaTeX.enableExport", "1");
			continue;
		}
		else if (strcmp(argv[currentArgCounter], "-to") == 0)
		{
			readLaTeXOutputPath = true;
			readOptionValue = true;
			continue;
		}
		else if (strcmp(argv[currentArgCounter], "-l") == 0)
		{
			readLabel = true;
			readOptionValue = true;
			continue;
		}
		else if (strcmp(argv[currentArgCounter], "-c") == 0)
		{
			readConfig = true;
			readOptionValue = true;
			continue;			
		}
		else if (strcmp(argv[currentArgCounter], "-v") == 0)
		{
			MSG::setMsgLevel(GROUP::INFO);
			continue;
		}
		else if (strcmp(argv[currentArgCounter], "-dbg") == 0)
		{
			readDBGLevel = true;
			readOptionValue = true;
			continue;
		}
		else if ((strcmp(argv[currentArgCounter], "-help") == 0) || (strcmp(argv[currentArgCounter], "-h") == 0) || (strcmp(argv[currentArgCounter], "-?") == 0))
		{
			printHelp = true;
			cfg.setString("VEC.printHelp", "true");
			return 1;
		}
		else if (readDokuWikiOutputPath)
		{
			readDokuWikiOutputPath = false;
			readOptionValue = false;
			cfg.setString("DokuWiki.outputPath", argv[currentArgCounter]);
		}
		else if (readFODGOutputPath)
		{
			readFODGOutputPath = false;
			readOptionValue = false;
			cfg.setString("FODG.outputPath", argv[currentArgCounter]);
		}
		else if (readMDOutputPath)
		{
			readMDOutputPath = false;
			readOptionValue = false;
			cfg.setString("Markdown.outputPath", argv[currentArgCounter]);
		}
		else if (readPNGOutputPath)
		{
			readPNGOutputPath = false;
			readOptionValue = false;
			cfg.setString("PNG.outputPath", argv[currentArgCounter]);
		}
		else if (readSVGOutputPath)
		{
			readSVGOutputPath = false;
			readOptionValue = false;
			cfg.setString("SVG.outputPath", argv[currentArgCounter]);
		}
		else if (readLaTeXOutputPath)
		{
			readLaTeXOutputPath = false;
			readOptionValue = false;
			cfg.setString("LaTeX.outputPath", argv[currentArgCounter]);
		}
		else if (readConfig)
		{
			readConfig = false;
			readOptionValue = false;
			cfg.setString("PATH.config", argv[currentArgCounter]);
		}
		else if (readLabel)
		{
			readLabel = false;
			readOptionValue = false;
			cfg.setString("default_label", argv[currentArgCounter]);
		}
		else if (readDBGLevel)
		{
			readDBGLevel = false;
			readOptionValue = false;
			MSG::setMsgLevel(GROUP::DEBUG);
			if (isdigit(argv[currentArgCounter][0]))
			{
				MSG::setDebugLevel(atoi(argv[currentArgCounter]));
			}
			else
			{
				MSG(GROUP::WARNING) << "No valid debug level specified. Debug level = 0 assumed. -dbg <NUMBER>";
				MSG::setDebugLevel(DEBUG::NONE);
			}
		}
		else
		{
			// If no programm option was found the current argument is treated as a source filepath
			fList.push_back(std::string(argv[currentArgCounter]));
		}
	}

	// Check if a option value should be read, but no more argv exist => Error
	if (readOptionValue)
	{
		MSG(GROUP::ERROR) << "Error in program arguments. Maybe missing a parameter? (Check " << argv[argc-1] << ")";
		return 1;
	}

	// Display programm options as debug output
	MSG(GROUP::DEBUG, DEBUG::RESULT) << "Program options: " << NL <<
		INDENT << "-d " << (cfg.getBool("DokuWiki.enableExport") == true ? "true" : "false") << " -do \"" << cfg.getString("DokuWiki.outputPath") << "\"" << NL <<
		INDENT << "-f " << (cfg.getBool("FODG.enableExport") == true ? "true" : "false") << " -fo \"" << cfg.getString("FODG.outputPath") << "\"" << NL <<
		INDENT << "-m " << (cfg.getBool("Markdown.enableExport") == true ? "true" : "false") << " -mo \"" << cfg.getString("Markdown.outputPath") << "\"" << NL <<
		INDENT << "-p " << (cfg.getBool("PNG.enableExport") == true ? "true" : "false") << " -po \"" << cfg.getString("PNG.outputPath") << "\"" << NL <<
		INDENT << "-s " << (cfg.getBool("SVG.enableExport") == true ? "true" : "false") << " -so \"" << cfg.getString("SVG.outputPath") << "\"" << NL <<
		INDENT << "-t " << (cfg.getBool("LaTeX.enableExport") == true ? "true" : "false") << " -to \"" << cfg.getString("LaTeX.outputPath") << "\"" << NL <<
		INDENT << "-l \"" << entityLabel << "\"" << NL <<
		//INDENT << "-c \"" << configFile << "\"" << NL <<
		INDENT << "-v " << (MSG::getMsgLevel() != -1 ? "true" : "false") << " (MsgLevel = " << MSG::getMsgLevel() << ")" << NL <<
		INDENT << "-dbg " << (MSG::getDebugLevel() != -1 ? "true" : "false") << " (DebugLevel = " << MSG::getDebugLevel() << ")";

	for (unsigned int i = 0; i < fList.size(); i++)
	{
		MSG::suppressLabel(true);
		MSG(GROUP::DEBUG, DEBUG::RESULT) << INDENT << "SOURCE " << fList[i];
		MSG::suppressLabel(false);
	}

	// If no source filepath was found pop an error
	if ((fList.size() == 0) && !printHelp)
	{
		MSG(GROUP::ERROR) << "Error in program arguments. Missing source file";
		return 1;
	}

	return 0;
}


// Print info dialog
void printInfo()
{
	MSG(GROUP::DEFAULT) << "This is VEC v" << VERSION;
	MSG(GROUP::DEFAULT) << "To generate data please specify an input file.";
#ifdef _WIN32		
	MSG(GROUP::DEFAULT) << "E.g. \"VEC rs_ff.vhd\"";
#else
	MSG(GROUP::DEFAULT) << "E.g. \"./VEC rs_ff.vhd\"";
#endif
	MSG(GROUP::DEFAULT) << "For further information call VEC with \"-help\".";

}


// Print the help dialog
void printHelp()
{
	MSG(GROUP::DEFAULT) << "VEC SOURCE [-l label] [-d] [-do DWPath] [-f] [-fo FODGPath] [-m] [-mo MDPath] [-p] [-po PNGPath] [-s] [-so SVGPath] [-t] [-to LaTeXPath] [-v] [-dbg level]";
	MSG(GROUP::DEFAULT) << NL
		<< std::setw(10) << std::left << "SOURCE" << "Specify a VHD or VHDL as source file" << NL
		//<< std::setw(10) << std::left << "-c" << "Specify a config file to use" << NL
		<< std::setw(10) << std::left << "-l" << "Specify label for the VHDL entity" << NL
		<< std::setw(10) << std::left << "-d" << "Enable DokuWiki export" << NL
		<< std::setw(10) << std::left << "-do" << "Specify the DokuWiki export output path" << NL
		<< std::setw(10) << std::left << "-f" << "Enable FODG export" << NL
		<< std::setw(10) << std::left << "-fo" << "Specify the FODG export output path" << NL
		<< std::setw(10) << std::left << "-m" << "Enable Markdown export" << NL
		<< std::setw(10) << std::left << "-mo" << "Specify the Markdown export output path" << NL
		<< std::setw(10) << std::left << "-p" << "Enable PNG export" << NL
		<< std::setw(10) << std::left << "-po" << "Specify the PNG export output path" << NL
		<< std::setw(10) << std::left << "-s" << "Enable SVG export" << NL
		<< std::setw(10) << std::left << "-so" << "Specify the SVG export output path" << NL
		<< std::setw(10) << std::left << "-t" << "Enable LaTeX export" << NL
		<< std::setw(10) << std::left << "-to" << "Specify the LaTeX export output path" << NL
		<< std::setw(10) << std::left << "-v" << "Set verbose mode" << NL
		<< std::setw(10) << std::left << "-dbg" << "Enter Debug mode at specified level" << NL
		<< std::setw(10) << std::left << "-help" << "Print this message" << NL;

	printLicense();
}


// Print the license
void printLicense()
{
	MSG(GROUP::DEFAULT) << NL
		<< "Copyright(C) 2014 University of Applied Sciences Augsburg" << NL
		<< "                  Benjamin Wiessneth, Johann Faerber" << NL
		<< "Email: benjamin.wiessneth@hs-augsburg.de" << NL
		<< "Email: johann.faerber@hs-augsburg.de" << NL
		<< NL
		<< "VEC is free software: you can redistribute it and / or modify" << NL
		<< "it under the terms of the GNU General Public License as published" << NL
		<< "by the Free Software Foundation, either version 3 of the License," << NL
		<< "or any later version." << NL;
		/*<< NL
		<< "This program is distributed in the hope that it will be useful," << NL
		<< "but WITHOUT ANY WARRANTY; without even the implied warranty of" << NL
		<< "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the" << NL
		<< "GNU General Public License for more details." << NL
		<< NL
		<< "You should have received a copy of the GNU General Public License" << NL
		<< "along with this program.If not, see <http://www.gnu.org/licenses/>." << NL;*/
}


void replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

std::string replaceStr(std::string source, std::string from, std::string to)
{
	if (from.empty())
		return "";
	size_t start_pos = 0;
	while ((start_pos = source.find(from, start_pos)) != std::string::npos)
	{
		source.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
	return source;
}