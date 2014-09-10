/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wießneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: SVGWriter.h
*	Author: Benjamin Wießneth
*	Description: Create SVG elements
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

class SVGWriter
{
public:
	// Creates an SVGWriter object with passed ostream reference as output
	SVGWriter(std::ostream &os);
	
	~SVGWriter(void);	
		
	// Writes the SVG header to the output file. If width and height are given the produced svg file will have these dimensions
	void SVGHeader(double width, double height);

	// Writes the SVG footer section
	void SVGFooter();
	
	// Writes the required SVG styles to the output file
	void SVGStyles();
	
	// Writes a group tag to the output file
	void group(bool close=false);

	// Write a line tag with given data	
	void line(double x1, double y1, double x2, double y2, std::string CSSclass);

	// Write a line tag with given data	
	void rect(double x, double y, double width, double height, std::string CSSclass);

	// Write circle tag with given data
	void circle(double x, double y, double diameter, std::string CSSclass);

	// Places a string as a text object at given location 
	void text(double x, double y, std::string text, std::string CSSclass);

	
private:
	std::ostream &os;
	int gluePointID;
	void header();
};

#endif