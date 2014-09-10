/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wießneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: MSG.h
*	Author: Benjamin Wießneth
*	Description: Message method to create console output
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


#ifndef MSG_H
#define MSG_H
#include <iostream>
#include <string>
#include <iomanip>

// Define control sequence for a newline
#define NL "\r\n"	/// TODO: Check correct output!

#define INDENT std::setw(10) << " "	// INDENT for console output
#define TAB "    "	// Tabulator for console output

// Class for all message groups
class GROUP
{
public:
	enum MessageGroups { DEFAULT = -1, ERROR = 0, WARNING = 1, INFO = 2, DEBUG = 3, ALL = 255 };
};

// Class for all debug levels
class DEBUG
{
public:
	enum DebugTypes { NONE = 0, FUNCTIONCALL = 1, RESULT = 2 };
};



class MSG
{
public: 
	MSG(int msgGroup = GROUP::DEFAULT, int debugLevel = DEBUG::NONE);
	template<typename T> MSG& operator<<(T value);
	~MSG();

	// Flag for suppressing labels
	static bool noLabel;  

	// Flag for printing labels once
	static bool printLabelOnce; 

	// Set global message level
	static void setMsgLevel(int level = -1);

	// Set global debug filter
	static void setDebugLevel(int level = -1);

	// Return global message level
	static int getMsgLevel();
	
	// Return global debug filter
	static int getDebugLevel();
    
	// Suppress message group label
	static void suppressLabel(bool quiet = true);

private:
	static bool validOutput;		// Flag if current call will pass threshold and produce output
	static int _MsgGroupTreshold;	// Global message level, this is set once
	static int MsgGroup;			// Intern message level, this is set by calling constructor
	static int DebugLevelTreshold;	// Intern message level, this is set by calling constructor
	static int DebugLevel;			// Intern message level, this is set by calling constructor
};

// Operator << overload to enable consecutive calls e.g. MSG() << "some" << "more";
template<typename T>
MSG& MSG::operator<<(T value)
{
	if (MsgGroup != GROUP::DEBUG)
	{
		if (MsgGroup <= _MsgGroupTreshold)
		{
			std::cout << value;			
		}
	}
	else if (MsgGroup == GROUP::DEBUG)
	{
		if (validOutput)
		{
			std::cout << value;
		}
	}
	return *this;        
}

#endif