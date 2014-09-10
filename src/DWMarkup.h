/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wieﬂneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: DWMarkup.h
*	Author: Benjamin Wieﬂneth
*	Description: Generate Doku Wiki table markup code from parser data
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

#ifndef DWMARKUP_H
#define DWMARKUP_H
#include "OFileHandler.h"
#include "VHDLEntity.h"

class DWMarkup : OFileHandler
{
private:
	void getMaxTextLengths();
	void writeCell(colNames column, std::string text, std::string prefix, std::string suffix, std::string fillChar=" ", bool formatting=false);

	void getHeader();	// Generate the table header
	void getRows();	// Generate the table content
	void getGenerics();	// Generate seperate table for the generic signals

	std::string Polarity(int i);	// Get polarity string of required port
	std::string Direction(int i);	// Get direction string of required port
	std::string Type(int i);	// Get type string of required port

public:
	DWMarkup(VHDLEntity sourceEntity);
};

#endif