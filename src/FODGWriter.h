/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wießneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: FODGWriter.h
*	Author: Benjamin Wießneth
*	Description: Create FODG file contents
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


#ifndef SVGWRITER_H
#define SVGWRITER_H

#include <string>
#include <iostream>
#include <sstream>
#include <stack>
#include <cassert>
#include <fstream>

class FODGWriter
{
public:
	// Creates an FODGWriter object with passed ostream reference as output
	FODGWriter(std::ostream &os);
	
	~FODGWriter(void);	
	
	// Writes a string
	void writeString(std::string str);
	
	// Places a string as a text object at given location 
	void text(int x, int y, char *text);
	
	// Writes the FODG header to the output file. If width and height are given the produced fodg file will have these dimensions
	void FODGHeader(double width, double height);
	
	// Writes the required FODG styles to the output file
	void FODGStyles();
	
	// Writes the FODG section for page dimensions
	void FODGPage(double width, double height);
	
	// Writes the FODG footer section
	void FODGclose();
	
	// Writes a group tag to the output file
	void group(bool close=false);
	
	// Places glue points at given coordinates
	void gluePoint(double x, double y);
	
	// Write custom shape tag with given data
	void customShape(double x, double y, double width, double height, std::string label, std::string shapeID = "gr1", std::string textID = "P1");
	
	// Write circle tag with given data
	void circle(double x, double y, double diameter, std::string shapeID = "gr1");
	
	// Write a line tag with given data	
	void line(double x1, double y1, double x2, double y2, std::string shapeID = "gr3");
	
private:
	std::ostream &os;
	int gluePointID;
	void header();
};

#endif