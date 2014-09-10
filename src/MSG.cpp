/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wießneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: MSG.cpp
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


#include "MSG.h"
#include "VEC.h"
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>

int MSG::MsgGroup = GROUP::DEFAULT;
int MSG::DebugLevel = DEBUG::NONE;
int MSG::_MsgGroupTreshold = GROUP::WARNING;	// Set default to show Info, Warning, and Error messages
int MSG::DebugLevelTreshold = DEBUG::NONE;		// Display no debug messages per default

bool MSG::validOutput = false;
bool MSG::noLabel = false;
bool MSG::printLabelOnce = false;

// Deconstructor
MSG::~MSG()
{
	// Check if valid output was created
	if (validOutput)
	{
		// Clear output stream
		std::cout << std::endl << std::flush;
		
		// Reset flag
		validOutput = false;
	}
}

// Constructor
MSG::MSG(int group, int level)
{
	MSG::MsgGroup = group;		// Remember constructor call parameter for operator<< function
	MSG::DebugLevel = level;	// Remember constructor call parameter for operator<< function
	
	// Check if current message passes threshold
	if (MsgGroup <= _MsgGroupTreshold)
	{
		// Check if no label is desired
		if (MSG::noLabel)
		{
			validOutput = true;
		}
		else
		{	
			// Print message group label 
			switch (MsgGroup)
			{
				case GROUP::DEFAULT:
					validOutput = true;
					break;
				case GROUP::ERROR:
					std::cout << std::setw(10) << std::left << "[Error] ";
					validOutput = true;
					break;
				case GROUP::WARNING:
					std::cout << std::setw(10) << std::left << "[Warning] ";
					validOutput = true;
					break;
				case GROUP::INFO:
					std::cout << std::setw(10) << std::left << "[Info] ";
					validOutput = true;
					break;
				case GROUP::DEBUG:
					// Check if current message passes debug filter
					if (((DebugLevelTreshold & (1 << (level - 1))) >= 1) || level == DEBUG::NONE)
					{
						std::cout << std::setw(10) << std::left << "[Debug] ";
						validOutput = true;
					}					
					break;
			}
		}
	}		
}

// Suppress message group label
void MSG::suppressLabel(bool quiet)
{
	if (quiet)
		noLabel = true;
	else
		noLabel = false;
}

// Set global message level
void MSG::setMsgLevel(int level)
{
	// Output has to be created by hand
	std::cout << std::setw(10) << std::left<< "[Info] " << "Set MsgGroupTreshold to " << level << std::endl;
	
	// Set global message threshold
	_MsgGroupTreshold = level;

	// Check for invalid levels
	if (_MsgGroupTreshold < -1)
		_MsgGroupTreshold = -1;
}

// Set global debug filter
void MSG::setDebugLevel(int level)
{
	// Output has to be created by hand
	std::cout << std::setw(10) << std::left << "[Info] " << "Set DebugLevelTreshold to " << level << std::endl;

	// Set global debug filter
	DebugLevelTreshold = level;

	// Check for invalid filter
	if (DebugLevelTreshold < -1)
		DebugLevelTreshold = -1;
}

// Return global message level
int MSG::getMsgLevel()
{
	return _MsgGroupTreshold;
}

// Return global debug filter
int MSG::getDebugLevel()
{
	return DebugLevelTreshold;
}