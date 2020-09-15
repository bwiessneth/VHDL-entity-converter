/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wießneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: EntityPort.cpp
*	Author: Benjamin Wießneth
*	Description: Represents the smallest possible unit of a VHDLEntity,
*	a single port.
*	Each port has a name, type, direction, and polarity.
*	Depending on it's constructor call it may defined as a clock or reset
port. *	A vector length may also be passed.
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

#include "EntityPort.h"
#include "MSG.h"
#include <ctype.h>
#include <iomanip>
#include <locale>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

const std::string EntityPort::portTypesLabels[] = { "NA", "LOGIC", "VECTOR" };
const std::string EntityPort::portDirectionsLabels[] = { "NONE",
                                                         "IN",
                                                         "OUT",
                                                         "BUFFER",
                                                         "INOUT" };

// Create "blank" port
EntityPort::EntityPort()
  : portLabel("")
  , portTypeStr("")
  , portDirection(EntityPort::NONE)
  , isClock(false)
  , isReset(false)
  , isVector(false)
  , vectorLength(-1)
  , vectorStart(-1)
  , vectorEnd(-1)
  , vectorStr("")
  , vStart("")
  , vEnd("")
  , LOWActive(false)
{}

// Create port with given parameters
EntityPort::EntityPort(std::string pLabel,
                       std::string pTypeStr,
                       portDirections portDirection,
                       bool isClock,
                       bool isReset,
                       int vectorLength,
                       bool LOWActive)
  : portLabel(pLabel)
  , portTypeStr(pTypeStr)
  , portDirection(portDirection)
  , isClock(isClock)
  , isReset(isReset)
  , vectorLength(vectorLength)
  , LOWActive(LOWActive)
{
  portTypeStr = pTypeStr;
}

// Print info about member values
void
EntityPort::printPortInfo()
{
  // printf("%-20s %-15s %-10s %-10i %-10i
  // %-10i\n",portLabel,portDirectionsLabels[portDirection],portTypesLabels[portType],isClock,isReset,vectorLength);
}

// Sets clock property
void
EntityPort::setClock(bool isClock)
{
  isClock = isClock;
}

// Sets reset property
void
EntityPort::setReset(bool isReset)
{
  isReset = isReset;
}

// Set the port direction
void
EntityPort::setPortDirection(portDirections portDirection)
{
  portDirection = portDirection;
}

// Set port label
void
EntityPort::setPortLabel(std::string portLabel)
{
  portLabel = portLabel;
}

// Returns the port direction
EntityPort::portDirections
EntityPort::getPortDirection()
{
  return portDirection;
}

// Returns the port label
std::string
EntityPort::getPortLabel()
{
  return portLabel;
}

// Returns reset property
bool
EntityPort::getReset()
{
  return isReset;
}

// Returns clock property
bool
EntityPort::getClock()
{
  return isClock;
}

// Returns vector property
bool
EntityPort::getVector()
{
  if (vectorLength < 0)
    return false;
  else
    return true;
}

// Returns vector length
int
EntityPort::getVectorLength()
{
  return vectorLength;
}

int
EntityPort::getVectorStart()
{
  return vectorStart;
}

int
EntityPort::getVectorEnd()
{
  return vectorEnd;
}

void
EntityPort::setVectorStart(int start)
{
  vectorStart = start;
}

void
EntityPort::setVectorEnd(int end)
{
  vectorEnd = end;
}

bool
EntityPort::getLOWActive()
{
  return LOWActive;
}

std::string
EntityPort::getPortType()
{
  return portTypeStr;
}

std::string
EntityPort::getVectorStr()
{
  // Check EntityPort's vector length
  // If greater than 0 it's a valid number
  if (vectorLength > 0) {
    // If vector length is greater than "0" it's an valid number
    // Convert the vector length to a string
    return std::to_string(vectorLength);
  }
  // If equal to "0" it contains a generic variable
  else if (vectorLength == 0) {
    return vectorStr;
  }

  // Return empty string when vector length = -1
  return "";
}

std::string
EntityPort::getVectorRawStr()
{
  // Check EntityPort's vector length
  // If greater than 0 it's a valid number
  if (vectorLength > 0) {
    // If vector length is greater than "0" it's an valid number
    // Convert the vector length to a string
    return static_cast<std::ostringstream*>(
             &(std::ostringstream() << vectorLength))
      ->str();
  }
  // If equal to "0" it contains a generic variable
  else if (vectorLength == 0) {
    // Check which part of the vector declaration is a number
    // If a number is found return the other part of the declaration
    //
    // e.g.  n-1 DOWNTO 0	< vStart isn't a number so it get's returned
    // 		 0 TO n-1		< vStart is a number so vEnd get's
    // returned

    // Check if first part contains data
    if (vStart.length() > 0) {
      // Check if vStart is a number
      if (isdigit(vStart[0]))
        return (vEnd);
      else
        return (vStart);
    }

    // Check if last part contains data
    if (vEnd.length() > 0) {
      // Check if vEnd is a number
      if (isdigit(vEnd[0]))
        return (vStart);
      else
        return (vEnd);
    }
  }

  // Return empty string when vector length = -1
  return "";
}

void
EntityPort::setVectorEndStr(std::string vE)
{
  vEnd = vE;
}

void
EntityPort::setVectorStartStr(std::string vS)
{
  vStart = vS;
}

void
EntityPort::setVectorStr(std::string vStr)
{
  vectorStr = vStr;
}

std::string
EntityPort::getVectorEndStr()
{
  return vEnd;
}

std::string
EntityPort::getVectorStartStr()
{
  return vStart;
}