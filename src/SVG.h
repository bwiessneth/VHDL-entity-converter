/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wieﬂneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: SVG.h
*	Author: Benjamin Wieﬂneth
*	Description: Generate SVG files from parser data
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

#ifndef SVG_H
#define SVG_H
#include "OFileHandler.h"
#include "VHDLEntity.h"

class SVG : OFileHandler
{
private:
  void initValues();
  void translate();
  double calcTextWidth(int charCount, double fontSize);

  double entityHeight;
  double entityWidth;
  double groupHeight;
  double groupWidth;
  double originX;
  double originY;
  double portWidth;
  double genericWidth;
  double genericHeight;
  double fontSize;
  double generics_fontSize;
  double entity_fontSize;
  double fixedWidth;
  std::string clockName;
  std::string resetName;
  int clockPort;
  int resetPort;
  //	std::string default_label;
  double horizontal_Margin;
  double vertical_Margin;
  double vectorTextLenghtIn;
  double vectorTextLenghtOut;

public:
  SVG(VHDLEntity sourceEntity);
};

#endif