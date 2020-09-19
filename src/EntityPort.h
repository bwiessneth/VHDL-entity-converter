/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wießneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: EntityPort.h
*	Author: Benjamin Wießneth
*	Description: Represents the smallest possible unit of a VHDLEntity,
*	a single port.
*	Each port has a name, type, direction, and polarity.
*	Depending on it's constructor call it may defined as a clock or reset
* port. A vector length may also be passed.
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

#ifndef ENTITYPORT_H
#define ENTITYPORT_H

#include <string>

class EntityPort
{
public:
  enum portDirections
  {
    NONE,
    IN,
    OUT,
    INOUT,
    BUFFER,
    LINKAGE
  }; // enum for possible directions of a single port
  static const std::string
    portDirectionsLabels[]; // captions for the direction enum

  // TODO: portTypes will be discontinued
  enum portTypes
  {
    NA,
    LOGIC,
    VECTOR
  }; // enum for possible types of a single port
  static const std::string portTypesLabels[]; // captions for the type enum

  // Creates an EntityPort object with no contents
  EntityPort();

  // Creates an EntityPort object with given parameters
  EntityPort(std::string portLabel,
             std::string portTypeStr,
             portDirections portDirection,
             bool isClock,
             bool isReset,
             int vectorLength,
             bool LOWActive);

  // Sets the port label of the current object
  void setPortLabel(std::string portLabel);

  // Sets the port direction of the current object
  void setPortDirection(portDirections portDirection);

  // Defines the port object as a clock port
  void setClock(bool isClock);

  // Defines the port object as a reset port
  void setReset(bool isReset);

  // Prints some information
  // The information is formatted like this:
  // portLabel portDirection* portDirection portType* portType isClock isReset
  void printPortInfo();

  // Returns the port direction
  portDirections getPortDirection();

  // Returns the clock property
  bool getClock();

  // Returns the reset property
  bool getReset();

  // Returns character pointer to the port name
  std::string getPortLabel();

  // Returns the vector property
  bool getVector();

  // Returns the vector length
  int getVectorLength();

  // Returns the vector start value
  int getVectorStart();

  // Returns the vector end value
  int getVectorEnd();

  // Set the vector start value
  void setVectorStart(int start);

  // Set the vector end value
  void setVectorEnd(int end);

  // Get the port polarity
  bool getLOWActive();

  // Get the port type string
  std::string getPortType();

  std::string getVectorStr();

  void setVectorStr(std::string vStr);

  void setVectorStartStr(std::string vS);

  void setVectorEndStr(std::string vE);

  std::string getVectorStartStr();

  std::string getVectorEndStr();

  std::string getVectorRawStr();

private:
  std::string portLabel;        // Name
  std::string portTypeStr;      // Type as a string
  portTypes portType;           // Type as enum
  portDirections portDirection; // Direction as enum
  bool isClock;                 // Clock flag
  bool isReset;                 // Reset flag
  bool isVector;                // Vector flag
  int vectorLength;             // vector length
  int vectorStart;              // vector start value
  int vectorEnd;                // vector end value
  std::string vectorStr;
  std::string vStart;
  std::string vEnd;
  bool LOWActive; // polarity flag
};

#endif