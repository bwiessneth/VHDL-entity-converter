/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wießneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: GenericSignal.cpp
*	Author: Benjamin Wießneth
*	Description: Represents a generic signal object
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


#include "GenericSignal.h"
#include <string>
#include <iomanip>
#include <locale>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

GenericSignal::GenericSignal(){}

GenericSignal::GenericSignal(string gL, string gTS, string dV, bool iV, int vL, std::string vS, std::string vE)
:genericName(gL), genericTypeStr(gTS), defaultValue(dV), isVector(iV), vectorLength(vL), vStart(vS), vEnd(vE)
{
	if (vectorLength >= 0)
		isVector = true;
	else
		isVector = false;
}

string GenericSignal::getGenericName(){
	return genericName;
}

string GenericSignal::getGenericTypeStr(){
	return genericTypeStr;
}

bool GenericSignal::getIsVector(){
	return isVector;
}

// Returns vector length
int GenericSignal::getVectorLength(){
	return vectorLength;
}

int GenericSignal::getVectorStart(){
	return vectorStart;
}

int GenericSignal::getVectorEnd(){
	return vectorEnd;
}

string GenericSignal::getDefaultValue(){
	return defaultValue;
}

string GenericSignal::getGenericStr()
{
	// Add the generics name to return string
	string tmp = genericName;
	
	// Check if the signal is a vector
	if (isVector)
	{
		if (vectorLength != 0)
		{
			// Append vector notation to the name
			tmp.append("[");
			tmp.append(std::to_string(vectorLength));
			tmp.append("]");
		}
		else
		{
			// Append vector notation to the name
			tmp.append("[");
			tmp.append(vStart);
			tmp.append(":");
			tmp.append(vEnd);
			tmp.append("]");
		}
	}

	// Check if the generic signal has a default value
	if (defaultValue.length() != 0)		
	{
		// Append default value to return string
		tmp.append(" = ");
		tmp.append(defaultValue);
	}
	
	return tmp;
}