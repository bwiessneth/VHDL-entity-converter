/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wieﬂneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: LaTeX.h
*	Author: Benjamin Wieﬂneth
*	Description: Generate LaTeX tables from parser data
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

#ifndef LATEX_H
#define LATEX_H
#include "OFileHandler.h"
#include "VHDLEntity.h"

class LaTeX : OFileHandler
{
private:
  void getMaxTextLengths(); // Get the maximum text lengths for each column in
                            // order to align the table content
  void writeCell(
    colNames column,
    std::string text,
    std::string prefix,
    std::string suffix,
    std::string fillChar = " ",
    bool formatting = false); // Write a text field to the output file

  void getTable();          // Write LaTeX table code into output file
  void getHeaderPorts();    // Generate the table header for the IO ports
  void getHeaderGenerics(); // Generate the table header for the generic signals
  void getPorts();          // Generate table for the IO ports
  void getGenerics();       // Generate table for the generic signals

  std::string Polarity(int i);  // Get polarity string of required port
  std::string Direction(int i); // Get direction string of required port
  std::string Type(int i);      // Get type string of required port

public:
  LaTeX(VHDLEntity sourceEntity);
};

#endif