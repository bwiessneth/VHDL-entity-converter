/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wießneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: VHDLParser.h
*	Author: Benjamin Wießneth
*	Description: Parse VHDL source files for the entity declaration
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


#ifndef VHDLParser_H
#define VHDLParser_H

#include <iostream>
#include <string>
#include <vector>
#include "VHDLEntity.h"

class VHDLParser {
	// States for top layer FSM
	enum STATES {	IDLE,
					SEARCH_ENTITY,
					SEARCH_ENTITYNAME,
					SEARCH_PORT,
					SEARCH_IOPORTS,
					SEARCH_GENERICS,
					DONE,
					SEARCH_LEFTPARENTHESIS,
					SEARCH_PORT_GENERIC };
	
	// Flags for second layer FSM
	enum FLAGS {	NONE, READ_IDENTIFIER, SEARCH_IDENTIFIER,
					SEARCH_DIRECTION,	READ_DIRECTION,
					SEARCH_TYPE, READ_TYPE,
					SEARCH_VECTORLENGTH, READ_VECTORLENGTH, FOUND_VECTORLENGTH,
					FOUND_BLANK, FOUND_COMMA,
					FOUND_COLON, FOUND_SEMICOLON,
					FOUND_RIGHTPARENTHESIS, FOUND_LEFTPARENTHESIS, READ_ENTITYNAME,
					FOUND_DASH,FOUND_COMMENT,FOUND_ENDCOMMENT,SEARCH_EQUAL,SEARCH_DEFAULT,FOUND_GENERIC,FOUND_PORT,READ_DEFAULT };
	
public:
	// VHDLParser constructor for vhdl file
	VHDLParser(std::string fileName);

	// VHDLParser destructor
	~VHDLParser();

	// Processes a given source file
	void parseFile(std::string fileName);
	
	// Prints info about the VHDLEntity from parsed source
	void printResults();

	// Returns VHDLEntity from parsed source
	VHDLEntity getEntity(){return result;};

	// Sets the label of VHDLEntity to given parameter
	void setEntityLabel(std::string eL);

private:
	// Loads mSourceBuffer buffer with contents of fileName
	void loadFile(std::string fileName);

	// Clears mParserBuffer
	void clearBuffer();

	// Appends parsed data to the VHDLEntity result
	void appendPort();

	void appendGeneric();

	// Checks if a comment is parsed at the moment
	bool isComment(int i);

	// Checks for the ENTITY keyword
	void searchEntity(int i);
	
	// Parses the entity name
	void searchEntityName(int i);

	// Checks for the PORT or GENERIC keyword
	void searchPortOrGeneric(int i);

	// Holds the length of the source file
	int fLength;

	// parseBuffers
	// Contains the source file
	std::string mSourceBuffer;
	
	// Working buffer
	std::string mParserBuffer;

	// Found VHDLEntity values
	VHDLEntity result;
	std::string entityName;

	STATES currentSTATE;
	FLAGS flag;

	// Keywords for parsing
	std::string ENTITY;
	std::string PORT;

	void readVector(std::string vString, int &vLength, std::string& vStart, std::string& vEnd);
	
	void matchGenerics();

	// Vars to hold keywords
	static const std::string keywordENTITY;
	static const std::string keywordPORT;
	static const std::string keywordGENERIC;
	static const std::string keywordEND;
	unsigned int keywordENTITYcount;
	std::string LOWActiveSuffix;
	std::string HIGHActiveSuffix;
	std::string keywordReset;
	std::string keywordClock;

	// Vars to hold parsed data
	std::vector<std::string> mParsedPortName;
	std::string mParsedPortType;
	std::string mParsedGenericDefaultValue;
	std::string mParsedVectorStart;
	std::string mParsedVectorEnd;

	size_t charCountPORT;
	size_t charCountGENERIC;
	size_t charCountEND;
	bool portSection;
	bool genericSection;
};

#endif