/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wießneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: GenericSignal.h
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


#ifndef GENERICSIGNAL_H
#define GENERICSIGNAL_H
#include <string>

using namespace std;

class GenericSignal {
	public:
		// Creates empty signal
		GenericSignal();
		// Creates signal from given parameters
		GenericSignal(string genericName, string genericTypeStr, string defaultValue, bool isVector = false, int vectorLength = -1, std::string vectorStart = "", std::string vectorEnd = "");
		// Returns stored generic name
		string getGenericName();
		// Returns stored generic type
		string getGenericTypeStr();
		// Returns if generic signal is a vector type
		bool getIsVector();
		// Returns vector length
		int getVectorLength();
		// Returns vector start value
		int getVectorStart();
		// Returns vector end value
		int getVectorEnd();
		// Returns signal default value
		string getDefaultValue();
		// Returns a string with all information about signal
		string getGenericStr();

	private:
		string genericName;		// name
		string genericTypeStr;	// type as a string
		string defaultValue;		// default value as a string
		bool isVector;			// vector flag
		int vectorLength;		// vector length
		int vectorStart;		// vector start value
		int vectorEnd;			// vector end value
		std::string vStart;
		std::string vEnd;
};

#endif