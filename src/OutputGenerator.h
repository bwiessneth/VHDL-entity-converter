/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wieﬂneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: OutputGenerator.h
*	Author: Benjamin Wieﬂneth
*	Description: The OutputGenerator class contains all functions to create
*	output from a source file.
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


#ifndef OUTPUTGENERATOR_H
#define OUTPUTGENERATOR_H
#include "VHDLEntity.h"

class OutputGenerator
{
public:
	OutputGenerator(std::string sourcePath, VHDLEntity sourceEntity);
	
private:
	void generateFODG();	// Generate FODG file
	void generateSVG();		// Generate SVG file
	void generatePNG();		// Generate PNG file
	void generateDokuWikiMarkup();	// Generate DokuWiki table
	void generateMarkdown();	// Generate Makrdown table
	void generateLaTeX();	// Generate LaTeX table

	std::string getOutputPath(std::string filePath);	// Helper function to get path from source file

	VHDLEntity mSourceEntity;
	std::string mSourceName;
	std::string mSourcePath;
};

#endif