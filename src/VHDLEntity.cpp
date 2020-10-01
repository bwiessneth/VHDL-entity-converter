/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wießneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: VHDLEntity.cpp
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

#include "VHDLEntity.h"
#include "MSG.h"
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

VHDLEntity::~VHDLEntity() {}

VHDLEntity::VHDLEntity()
  : entityName("myEntity")
  , entityLabel("")
  , numberOfPorts(0)
  , numberOfGenerics(0)
  , numberOfInputs(0)
  , numberOfOutputs(0)
  , clockPort(-1)
  , resetPort(-1)
{}

void
VHDLEntity::printEntityInfo()
{
  MSG(GROUP::INFO) << "Entity structure:";
  MSG::suppressLabel(true);
  MSG(GROUP::INFO) << INDENT << "Inputs: " << numberOfInputs << NL << INDENT
                   << "Outputs: " << numberOfOutputs << NL << INDENT
                   << "Generics: " << numberOfGenerics << NL << INDENT
                   << "CLOCK: " << ((getClockPort() != -1) ? "true" : "false")
                   << NL << INDENT
                   << "RESET: " << ((getResetPort() != -1) ? "true" : "false");
  MSG::suppressLabel(false);

  return;
}

void
VHDLEntity::setEntityName(string eName)
{
  entityName = eName;
}

EntityPort::portDirections
VHDLEntity::getPortdirection(int portNumber)
{
  return myPorts[portNumber].getPortDirection();
}

void
VHDLEntity::appendPort(EntityPort newPort)
{
  myPorts.push_back(EntityPort(newPort));
  if (myPorts[numberOfPorts].getPortDirection() == EntityPort::IN)
    numberOfInputs++;
  if (myPorts[numberOfPorts].getPortDirection() == EntityPort::OUT)
    numberOfOutputs++;
  if (myPorts[numberOfPorts].getClock())
    clockPort = numberOfPorts;
  if (myPorts[numberOfPorts].getReset())
    resetPort = numberOfPorts;
  numberOfPorts++;
}

void
VHDLEntity::appendGeneric(GenericSignal newGeneric)
{
  myGenerics.push_back(GenericSignal(newGeneric));
  numberOfGenerics++;
}

int
VHDLEntity::getNumberOfInputs()
{
  return numberOfInputs;
}

int
VHDLEntity::getNumberOfOutputs()
{
  return numberOfOutputs;
}

int
VHDLEntity::getNumberOfGenerics()
{
  return numberOfGenerics;
}

string
VHDLEntity::getPortName(int portNumber)
{
  return myPorts[portNumber].getPortLabel();
}

string
VHDLEntity::getEntityName()
{
  return entityName;
}

int
VHDLEntity::getClockPort()
{
  return clockPort;
}

int
VHDLEntity::getResetPort()
{
  return resetPort;
}

bool
VHDLEntity::isVector(int i)
{
  return myPorts[i].getVector();
}

int
VHDLEntity::getVectorLength(int i)
{
  return myPorts[i].getVectorLength();
}

int
VHDLEntity::getVectorStart(int i)
{
  return myPorts[i].getVectorStart();
}

int
VHDLEntity::getVectorEnd(int i)
{
  return myPorts[i].getVectorEnd();
}

bool
VHDLEntity::getLOWActive(int i)
{
  return myPorts[i].getLOWActive();
}

string
VHDLEntity::getPortTypeStr(int i)
{
  return myPorts[i].getPortType();
}

string
VHDLEntity::getGenericName(int genericNumber)
{
  return myGenerics[genericNumber].getGenericName();
}

string
VHDLEntity::getGenericType(int genericNumber)
{
  return myGenerics[genericNumber].getGenericTypeStr();
}

bool
VHDLEntity::getGenericIsVector(int genericNumber)
{
  return myGenerics[genericNumber].getIsVector();
}

int
VHDLEntity::getGenericVectorLength(int genericNumber)
{
  return myGenerics[genericNumber].getVectorLength();
}

int
VHDLEntity::getGenericVectorStart(int genericNumber)
{
  return myGenerics[genericNumber].getVectorStart();
}

int
VHDLEntity::getGenericVectorEnd(int genericNumber)
{
  return myGenerics[genericNumber].getVectorEnd();
}

string
VHDLEntity::getGenericDefaultValue(int genericNumber)
{
  return myGenerics[genericNumber].getDefaultValue();
}

void
VHDLEntity::setVectorEndStr(int i, string vE)
{
  myPorts[i].setVectorEndStr(vE);
}

void
VHDLEntity::setVectorStartStr(int i, string vS)
{
  myPorts[i].setVectorStartStr(vS);
}

void
VHDLEntity::setVectorStr(int i, string vStr)
{
  myPorts[i].setVectorStr(vStr);
}

string
VHDLEntity::getVectorStr(int i)
{
  return myPorts[i].getVectorStr();
}

string
VHDLEntity::getVectorRawStr(int i)
{
  return myPorts[i].getVectorRawStr();
}

string
VHDLEntity::getVectorEndStr(int i)
{
  return myPorts[i].getVectorEndStr();
}

string
VHDLEntity::getVectorStartStr(int i)
{
  return myPorts[i].getVectorStartStr();
}

void
VHDLEntity::setEntityLabel(string eL)
{
  entityLabel = eL;
}

string
VHDLEntity::getEntityLabel()
{
  return entityLabel;
}

string
VHDLEntity::getGenericStr(int i)
{
  return myGenerics[i].getGenericStr();
}