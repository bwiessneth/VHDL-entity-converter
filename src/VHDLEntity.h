/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wießneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: VHDLEntity.h
*	Author: Benjamin Wießneth
*	Description: Represents a complete VHDLEntity
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


#ifndef VHDLEntity_H
#define VHDLEntity_H

#include "EntityPort.h"
#include "GenericSignal.h"
#include <string>
#include <vector>

#define MAXPORTS 1024

class VHDLEntity
{
public:
	// Creates an VHDLEntity object with no ports
	VHDLEntity();

	~VHDLEntity();

	// Appends an EntityPort to a VHDLEntity object
	void appendPort(EntityPort);

	// Appends an GenericSignal to a VHDLEntity object
	void appendGeneric(GenericSignal);

	// Sets the number of output ports for a VHDLEntity object
	void setNumberOfOutputs(int numberOfOutputs);

	// Sets the number of input ports for a VHDLEntity object
	void setNumberOfInputs(int numberOfInputs);

	// Sets the entity name for a VHDLEntity object
	void setEntityName(std::string entityName);

	// Prints some info about self
	void printEntityInfo();
	
	// Returns the number of input ports
	int getNumberOfInputs();

	// Returns the number of output ports
	int getNumberOfOutputs();

	// Returns the number of generic signals
	int getNumberOfGenerics();

	// Returns the port name from given port number
	std::string getPortName(int portNumber);

	// Returns the entity name
	std::string getEntityName();

	// Returns the reset property from given port number
	int getResetPort();

	// Returns the clock property from given port number
	int getClockPort();

	// Returns the port direction from given port number
	EntityPort::portDirections getPortdirection(int portNumber);

	// Returns the isVector property from given port number
	bool isVector(int i);

	// Returns the vector length from given port number
	int getVectorLength(int i);

	// Returns the vector start value from given port number
	int getVectorStart(int i);

	// Returns the vector end value from given port number
	int getVectorEnd(int i);

	// Returns the polarity from given port number
	bool getLOWActive(int i);

	// Returns the port type std::string from given port number
	std::string getPortTypeStr(int i);

	std::string getGenericName(int genericNumber);
	
	std::string getGenericType(int genericNumber);
	
	bool getGenericIsVector(int genericNumber);
	
	int getGenericVectorLength(int genericNumber);
	
	int getGenericVectorStart(int genericNumber);
	
	int getGenericVectorEnd(int genericNumber);
	
	std::string getGenericDefaultValue(int genericNumber);

	void setVectorEndStr(int i, std::string vE);
	
	void setVectorStartStr(int i, std::string vS);
	
	void setVectorStr(int i, std::string vStr);
	
	std::string getVectorStr(int i);

	std::string getVectorStartStr(int i);
	
	std::string getVectorEndStr(int i);

	void setEntityLabel(std::string eL);
	
	std::string getEntityLabel();

	std::string getGenericStr(int genericNumber);

	std::string getVectorRawStr(int i);

private:
	std::string entityName;
	std::string entityLabel;
	int numberOfPorts;	
	int numberOfGenerics;
	int numberOfInputs;
	int numberOfOutputs;
	int clockPort;
	int resetPort;
	std::vector<EntityPort>  myPorts;
	std::vector<GenericSignal>  myGenerics;
};


#endif