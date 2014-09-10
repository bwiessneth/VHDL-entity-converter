/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wießneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: VHDLParser.cpp
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


#define _CRT_SECURE_NO_WARNINGS

#include "MSG.h"
#include "tools.h"
#include "VHDLParser.h"
#include "Config.h"

#include <stdlib.h> // atoi
#include <string>
#include <cstring> // strlen

using namespace std;

// entityName
string eName;

bool isVector = false;
int vectorLength = 0;
int vectorStart=0;
int vectorEnd=0;

const std::string VHDLParser::keywordENTITY = "ENTITY";
const std::string VHDLParser::keywordPORT = "PORT";
const std::string VHDLParser::keywordGENERIC = "GENERIC";
const std::string VHDLParser::keywordEND = "END";

EntityPort::portTypes readPortType;
EntityPort::portDirections readPortDir;


VHDLParser::~VHDLParser()
{
	MSG(GROUP::DEBUG,DEBUG::FUNCTIONCALL) << "VHDLParser::~VHDLParser()";
}

VHDLParser::VHDLParser(std::string fileName)
{
	MSG(GROUP::DEBUG,DEBUG::FUNCTIONCALL) << "VHDLParser::VHDLParser(" << fileName << ")";
	clearBuffer();

	currentSTATE = IDLE;
	flag = NONE;
	keywordENTITYcount = 0;
	charCountPORT = 0;
	charCountGENERIC = 0;
	charCountEND = 0;
	portSection = false;
	genericSection = false;

	// Create result object
	result = VHDLEntity();

	// Special ports
	keywordReset = cfg.getString("resetName");
	keywordClock = cfg.getString("clockName");

	// Polarity suffixes
	HIGHActiveSuffix = cfg.getString("HIGHActiveSuffix");
	LOWActiveSuffix = cfg.getString("LOWActiveSuffix");

	if (!fileName.empty())
		parseFile(fileName);
}


void VHDLParser::parseFile(std::string fileName)
{
	MSG(GROUP::DEBUG,DEBUG::FUNCTIONCALL) << "VHDLParser::parseFile(" << fileName << ")";

	if ((result.getNumberOfInputs()) || (result.getNumberOfOutputs())>0)
		MSG(0) << "Parser - VHDLEnity result already contains data!";

	loadFile(fileName);
	// Init common vars
	currentSTATE = SEARCH_ENTITY;
	vectorLength = -1;
	readPortDir = EntityPort::NONE;

	// Loop through all chars in mSourceBuffer
	for (int i=0; i<fLength; i++)
	{
		// Check for comment and set flag accordingly
		if (isComment(i))
			continue;

		switch(currentSTATE){
		// Searching for "ENTITY"
		case SEARCH_ENTITY:
			searchEntity(i);
			break;

		// Searching for "ENTITYNAME"
		case SEARCH_ENTITYNAME:
			searchEntityName(i);
			break;

		// Searching for PORT, GENERIC, END
		case SEARCH_PORT_GENERIC:
			searchPortOrGeneric(i);
			break;

		case SEARCH_LEFTPARENTHESIS:
			if (toupper(mSourceBuffer[i])=='(')
			{
				if (flag == FOUND_GENERIC) 
				{
					currentSTATE = SEARCH_GENERICS;
					flag = READ_IDENTIFIER;
					clearBuffer();
				}
				else if (flag == FOUND_PORT)
				{
					currentSTATE = SEARCH_IOPORTS;
					flag = READ_IDENTIFIER;
					clearBuffer();
				}
			}
			break;

		// Found PORT() section within the source code. Now the ports are processed
		// Now look line-by-line for strings/char* separated by commas which represent the port names. 
		// Then search for a colon which is then followed by the port-type (I/O, Buffer, e.g)
		// The end of the PORT() section is reached when there's an right parenthesis followed by a semicolon
		case SEARCH_IOPORTS:
			switch(int(toupper(mSourceBuffer[i]))){

			// Tabulator
			case 9:
			// blank
			case 32:	
				if (flag == READ_DIRECTION)
				{	
					// blank and active READ_DIRECTION flag => portDirection is read
					if ((mParserBuffer.compare("IN")==0) | (mParserBuffer.compare("in")==0))
					{
						readPortDir = EntityPort::IN;
					}
					else if ((mParserBuffer.compare("OUT")==0) | (mParserBuffer.compare("out")==0))
					{
						readPortDir = EntityPort::OUT;
					}
					else if ((mParserBuffer.compare("BUFFER")==0) | (mParserBuffer.compare("buffer")==0))
					{
						readPortDir = EntityPort::OUT;
					}
					else if ((mParserBuffer.compare("LINKAGE")==0) | (mParserBuffer.compare("linkage")==0))
					{
						readPortDir = EntityPort::OUT;
					}
					else if ((mParserBuffer.compare("INOUT")==0) | (mParserBuffer.compare("inout")==0))
					{
						readPortDir = EntityPort::OUT;
					}
					else
					{
						readPortDir = EntityPort::NONE;
					}
					
					clearBuffer();
					flag = SEARCH_TYPE;
				}
				else if (flag == READ_VECTORLENGTH)
				{
					mParserBuffer += mSourceBuffer[i];
				}
				break;

			// LineFeed
			case 10:
				/// TODO: Add end-of-comment flag
				if (flag == FOUND_COMMENT) flag = SEARCH_IDENTIFIER;
				break;

			// CarriageReturn
			case 13:
				break;

			// comma
			case int(','):
				if (flag == READ_IDENTIFIER)
				{
					mParsedPortName.push_back(mParserBuffer);
					clearBuffer();
				}
				break;

			// colon
			case int(':'):
				if (flag == READ_IDENTIFIER)
				{
					mParsedPortName.push_back(mParserBuffer);
					clearBuffer();
					flag = SEARCH_DIRECTION;
				}
				break;

			// semicolon
			case int(';'):		
				if (flag == READ_TYPE) 
				{
					// semicolon and flag READ_TYPE => the only port declaration is read
					// If the FOUND_RIGHTPARENTHESIS flag isn't set this means there has to be at least one more line of port declarations
					mParsedPortType  = mParserBuffer;

					appendPort();
					clearBuffer(); 
					flag = SEARCH_IDENTIFIER;
				}
				else if (flag == FOUND_VECTORLENGTH)
				{
					flag = SEARCH_IDENTIFIER;
					clearBuffer(); 
				}
				else if (flag == FOUND_RIGHTPARENTHESIS)
				{
					// semicolon and FOUND_RIGHTPARENTHESIS => PORT() section is completely read
					// Previously found a ')' and now a ';' which means the Code section is closed
					//pmesg(5,"PORT(..) done\n");
					flag = NONE;
					clearBuffer();
				
					currentSTATE = SEARCH_PORT_GENERIC;
				}
				break;

			// left parenthesis
			case int('('):
				if (flag == READ_TYPE)
				{
					mParsedPortType = mParserBuffer;
					clearBuffer();
					flag = SEARCH_VECTORLENGTH;
				}
				break;

			// right parenthesis
			case int(')'):
				if (flag == READ_TYPE)
				{
					// right parenthesis and READ_TYPE => portType completely read
					mParsedPortType = mParserBuffer;

					appendPort();
					clearBuffer(); 
					flag = FOUND_RIGHTPARENTHESIS;
				}
				else if (flag == READ_VECTORLENGTH)
				{				
					readVector(mParserBuffer, vectorLength, mParsedVectorStart, mParsedVectorEnd);
					flag = FOUND_VECTORLENGTH;

					appendPort();
					clearBuffer(); 
				}
				else if (flag == FOUND_VECTORLENGTH)
				{
					flag = FOUND_RIGHTPARENTHESIS;
				}
				else if (flag == FOUND_ENDCOMMENT)
				{
					flag = FOUND_RIGHTPARENTHESIS;
				}
				break;

			case '-':				
				if (flag != READ_VECTORLENGTH)
					break;

			default:
				// A valid char was read
				// Set new flags according to the current flag
				if ((flag == FOUND_COMMA) || (flag == NONE)) flag = READ_IDENTIFIER;
				if (flag == SEARCH_DIRECTION) flag = READ_DIRECTION;
				if ((flag == SEARCH_TYPE) || (flag == FOUND_RIGHTPARENTHESIS)) flag = READ_TYPE;
				if (flag == SEARCH_VECTORLENGTH) flag = READ_VECTORLENGTH;
				if (flag == SEARCH_IDENTIFIER) flag = READ_IDENTIFIER;
				if (flag == FOUND_ENDCOMMENT) flag = READ_IDENTIFIER;			

				switch(flag)
				{
					case FOUND_COMMENT:
						// Do nothing with comment
						break;
					case READ_DIRECTION:
					case READ_IDENTIFIER:
					case READ_TYPE:
					case READ_VECTORLENGTH:
					default:
						// Buffer chars
						mParserBuffer += mSourceBuffer[i];
						break;
				}
			}
			break;

		case SEARCH_GENERICS:
			switch(int(toupper(mSourceBuffer[i]))){
				case 32:
					if (flag == READ_VECTORLENGTH) mParserBuffer += mSourceBuffer[i];
				case 13:
				case 9:
					break;			  
	
				// LineFeed
				case 10:
					/// TODO: Add end-of-comment flag
					if (flag == FOUND_COMMENT)
						flag = READ_IDENTIFIER;
					break;

				case ',':
					if (flag == READ_IDENTIFIER)
					{
						mParsedPortName.push_back(mParserBuffer);
						clearBuffer();
					}
					break;
			  
				case ':':
					if (flag == READ_IDENTIFIER)
					{
						flag = SEARCH_TYPE;
						mParsedPortName.push_back(mParserBuffer);
						clearBuffer();
					}
					else if (flag == READ_TYPE)
					{
						flag = SEARCH_EQUAL;
						mParsedPortType = mParserBuffer;
						clearBuffer();
					}
					else if (flag == FOUND_VECTORLENGTH)
						flag = SEARCH_EQUAL;
					break;

				case '(':
					if (flag == READ_TYPE)
					{
						//flag = READ_VECTORLENGTH;
						flag = SEARCH_VECTORLENGTH;
						mParsedPortType = mParserBuffer;
						clearBuffer();		
					}
					break;	
		
				case '=':
					if (flag == SEARCH_EQUAL)
						flag = READ_DEFAULT;
					break;
	
				case ')':
					if (flag == READ_VECTORLENGTH)
					{
						flag = FOUND_VECTORLENGTH;
						
						readVector(mParserBuffer, vectorLength, mParsedVectorStart, mParsedVectorEnd);
						//sscanf(mParserBuffer.c_str(),"%d %*s %d",&vectorStart, &vectorEnd);
						//vectorLength = abs(vectorEnd-vectorStart)+1;
						
						isVector = true;
						flag = FOUND_VECTORLENGTH;
						clearBuffer();
					}
					else if ((flag == FOUND_VECTORLENGTH) ||
							 (flag == READ_TYPE) ||
							 (flag == READ_DEFAULT) ||
							 (flag == FOUND_ENDCOMMENT))
					{
						if ((flag == READ_DEFAULT) || (flag == FOUND_ENDCOMMENT))
						{
							mParsedGenericDefaultValue = mParserBuffer;
							appendGeneric();
							clearBuffer();
						}
						flag = FOUND_RIGHTPARENTHESIS;
						// appendGeneric();
					}
					break;
		
				case ';':
					if (flag == READ_DEFAULT)
					{
						flag = READ_IDENTIFIER;
						mParsedGenericDefaultValue = mParserBuffer;
						clearBuffer();
						appendGeneric();
					}
					else if (flag == FOUND_VECTORLENGTH)
					{
						flag = READ_IDENTIFIER;
						clearBuffer();
						appendGeneric();
					} 
					else if  (flag == READ_TYPE)
					{
						flag = READ_IDENTIFIER;
						mParsedPortType = mParserBuffer;
						clearBuffer();		
						appendGeneric();
					}
					else if (flag == FOUND_RIGHTPARENTHESIS)
					{
						appendGeneric();
						flag = NONE;
						currentSTATE = SEARCH_PORT_GENERIC;
					}
					break;
	
				case '-':				
					if (flag == FOUND_DASH)
					{
						flag = FOUND_COMMENT;
					}
					break;	

				default:
					if (flag == SEARCH_IDENTIFIER)
						flag = READ_IDENTIFIER;
					if (flag == FOUND_ENDCOMMENT)
						flag = READ_IDENTIFIER;
					if (flag == SEARCH_TYPE)
						flag = READ_TYPE;
					if (flag == SEARCH_DEFAULT)
						flag = READ_DEFAULT;
					if (flag == SEARCH_VECTORLENGTH)
						flag = READ_VECTORLENGTH;

					if ((flag == READ_IDENTIFIER) ||
						(flag == READ_TYPE) ||
						(flag == READ_DEFAULT) ||
						(flag == READ_VECTORLENGTH))
						mParserBuffer += mSourceBuffer[i];
					break;
			}

			break;

		default:
			break;
		}

		if (currentSTATE == DONE)
		{			
			break;
		}
	}
	matchGenerics();
}

void VHDLParser::loadFile(string fileName)
{
	MSG(GROUP::DEBUG,DEBUG::FUNCTIONCALL) << "VHDLParser::loadFile(" << fileName << ")";
	// load vhd source into global mSourceBuffer buffer
	std::ifstream ifs((char*) fileName.c_str());
	mSourceBuffer.assign( (std::istreambuf_iterator<char>(ifs) ),
                (std::istreambuf_iterator<char>()    ) );
	fLength = mSourceBuffer.length();	
}

void VHDLParser::clearBuffer()
{
	// MSG(GROUP::DEBUG,DEBUG::FUNCTIONCALL) << "VHDLParser::clearBuffer()";
	mParserBuffer.clear();
}

void VHDLParser::printResults()
{
	result.printEntityInfo();
}

void VHDLParser::appendPort()
{
	MSG(GROUP::DEBUG,DEBUG::FUNCTIONCALL) << "void VHDLParser::appendPort()";

	bool checkClock = false;
	bool checkReset = false;
	bool checkLOWActive = false;

	for (unsigned int i = 0; i < mParsedPortName.size(); i++)
	{
		if (mParsedPortName[i].compare(keywordClock) == 0) checkClock = true;
		if (mParsedPortName[i].compare(keywordReset) == 0) checkReset = true;
		if (mParsedPortName[i].find(HIGHActiveSuffix) != std::string::npos) checkLOWActive = false;
		if (mParsedPortName[i].find(LOWActiveSuffix) != std::string::npos) checkLOWActive = true;

		// Check if parsed port is a vector
		if (vectorLength != -1)
		{
			// Create new port with valid vector length
			EntityPort nP = EntityPort(mParsedPortName[i], mParsedPortType, readPortDir, checkClock, checkReset, vectorLength, checkLOWActive);

			// Set the vector strings to the parsed result
			nP.setVectorStartStr(mParsedVectorStart);
			nP.setVectorEndStr(mParsedVectorEnd);

			// Append the new port to the result entity
			result.appendPort(nP);
		}
		else
		{
			// Create new port without a valid vector length
			EntityPort nP = EntityPort(mParsedPortName[i], mParsedPortType, readPortDir, checkClock, checkReset, -1, checkLOWActive);

			// Append the new port to the result entity
			result.appendPort(nP);
		}

		checkClock = false;
		checkReset = false;
		checkLOWActive = false;
	}

	// Clear global vars
	mParsedGenericDefaultValue = "";
	mParsedVectorStart = "";
	mParsedVectorEnd = "";
	vectorLength = -1;
	isVector = false;
	vectorStart = 0;
	vectorEnd = 0;
	clearBuffer();

	// Clear vars for intermediate parsing step
	mParsedPortName.clear();
	mParsedPortType.clear();
}

void VHDLParser::appendGeneric()
{
	// If multiple generic signals where declared at once, loop through them
	for (unsigned int i = 0; i < mParsedPortName.size(); i++)
	{
		result.appendGeneric(GenericSignal(mParsedPortName[i], mParsedPortType, mParsedGenericDefaultValue, isVector, vectorLength, mParsedVectorStart, mParsedVectorEnd));
	}
	
	// Clear global vars
	mParsedGenericDefaultValue.clear();
	mParsedVectorStart.clear();
	mParsedVectorEnd.clear();
	isVector = false;
	vectorLength = -1;
	vectorStart = 0;
	vectorEnd = 0;

	// Clear vars for intermediate parsing step
	mParsedPortName.clear();
	mParsedPortType.clear();
	mParsedGenericDefaultValue.clear();

	clearBuffer();
}

// Checks mSourceBuffer for comments
bool VHDLParser::isComment(int i)
{	
	// A dash sets flag for possible comment, another Dash sets comment flag
	// LineFeed terminates a comment
	if (mSourceBuffer[i] == 10)
	{
		if (flag == FOUND_COMMENT)
			flag = FOUND_ENDCOMMENT;
		return false;
	}
	else if (flag == FOUND_COMMENT)
		return true;
	else if (flag != READ_VECTORLENGTH)
		if (mSourceBuffer[i] == '-')
		{
			if (flag == FOUND_DASH)
			{
				flag = FOUND_COMMENT;
				return true;
			}
			else
			{
				if (flag == READ_TYPE) appendPort();
					flag = FOUND_DASH;
				return false;
			}
		}
		return false;
}

// Checks mSourceBuffer for the ENTITY keyword
void VHDLParser::searchEntity(int i){
	if (flag == FOUND_COMMENT)
	{
		return;
	}
	else
	{
		if (toupper(mSourceBuffer[i])==keywordENTITY[keywordENTITYcount])
		{
			if (keywordENTITYcount == keywordENTITY.length()-1) 
			{	
				currentSTATE = SEARCH_ENTITYNAME;
			}
			else  keywordENTITYcount++;
		}
		else 
		{
			keywordENTITYcount = 0;
		}
	}
}

// Checks mSourceBuffer for the entity name
void VHDLParser::searchEntityName(int i){
	switch(int(toupper(mSourceBuffer[i])))
	{
		// Stop reading entityname if tab or blank is found		
		case 9:		// tab
		case 32:	// blank
			if (flag == READ_ENTITYNAME)
			{
				entityName = mParserBuffer;
				result.setEntityName(entityName);
				clearBuffer();						
				currentSTATE = SEARCH_PORT_GENERIC;
			}
			break;

		// Fill buffer with entityname
		default:
			// Begin to read in after first tab or blank after the keyword ENTITY
			flag = READ_ENTITYNAME;
			mParserBuffer += mSourceBuffer[i];
	}
	return;
}

// Checks for the PORT or GENERIC keyword
void VHDLParser::searchPortOrGeneric(int i)	
{
	if (portSection == false)
	{
		if (toupper(mSourceBuffer[i]) == keywordPORT[charCountPORT])
		{
			if (charCountPORT == keywordPORT.length() - 1)
			{
				charCountPORT = 0;
				portSection = true;
				currentSTATE = SEARCH_LEFTPARENTHESIS;
				flag = FOUND_PORT;
				clearBuffer();
				return;
			}
			else  charCountPORT++;
		}
		else
		{
			charCountPORT = 0;
		}
	}

	if (genericSection == false)
	{
		if (toupper(mSourceBuffer[i]) == keywordGENERIC[charCountGENERIC])
		{
			if (charCountGENERIC == keywordGENERIC.length() - 1)
			{
				charCountGENERIC = 0;
				genericSection = true;
				currentSTATE = SEARCH_LEFTPARENTHESIS;
				flag = FOUND_GENERIC;
				clearBuffer();
				return;
			}
			else  charCountGENERIC++;
		}
		else
		{
			charCountGENERIC = 0;
		}
	}

	// Check for the keyword END
	// Ends the parsing of ports and generics declaration
	// This will also prevent empty output when parsing testbench files
	if (toupper(mSourceBuffer[i]) == keywordEND[charCountEND])
	{
		if (charCountEND == keywordEND.length() - 1)
		{
			charCountEND = 0;
			currentSTATE = DONE;
			clearBuffer();
			return;
		}
		else
			charCountEND++;
	}

	if (portSection && genericSection)
		currentSTATE = DONE;

	return;
}

// Read vector length, vector start, and vector end from vString
void VHDLParser::readVector(string vString, int &vLength, string& mParsedVectorStart, string& mParsedVectorEnd){
	char* start = new char[1024];
	char* dir = new char[1024];
	char* end = new char[1024];

	// Split vector declaration into three seperate strings
	sscanf(vString.c_str(),"%s %s %s",start, dir, end);

	bool vStartIsDigit = false;
	bool vEndIsDigit = false;

	// Loop through first vector string and check if it consists of digits
	for (size_t i = 0; i < std::strlen(start); i++)
	{
		if (isdigit(start[i]) != 0)
			vStartIsDigit = true;
		else if (isdigit(start[i]) == 0)
		{
			vStartIsDigit = false;
			break;
		}
	}
	if (vStartIsDigit == false)
	{
		mParsedVectorStart = start;
	}

	// Loop through second vector string and check if it consists of digits
	for (size_t i = 0; i < std::strlen(end); i++)
	{
		if (isdigit(end[i]) != 0)
			vEndIsDigit = true;
		else if (isdigit(end[i]) == 0)
		{
			vEndIsDigit = false;
			break;
		}
	}

	mParsedVectorStart = start;
	mParsedVectorEnd = end;

	// If both strings are numbers calculate the vector length
	if ((vStartIsDigit == false) || (vEndIsDigit == false))
	{
		vLength = 0;
	}
	else
	{
		vLength = abs(atoi(start)-atoi(end))+1;
	}
}

void VHDLParser::setEntityLabel(string eL){
	result.setEntityLabel(eL);
}

// Tries to match the vector length declaration of ports with found a generic signal
void VHDLParser::matchGenerics()
{
	// Loop through all regular ports
	for (int i = 0; i < result.getNumberOfInputs()+result.getNumberOfOutputs(); i++)
	{
		// Check if current port is a vector
		if (result.getVectorLength(i) != -1)
		{
			// Set raw input as default vector length in case no suitable match is found
			result.setVectorStr(i,result.getVectorRawStr(i));

			// Get both parsed vector strings (vhd-source: StartStr TO/DOWNTO EndStr)
			std::string parsedVectorStartStr = result.getVectorStartStr(i);
			std::string parsedVectorEndStr = result.getVectorEndStr(i);
			
			// Variable to save max match length
			int matchLength = 0;

			// Loop through all generic signals
			for (int j = 0; j < result.getNumberOfGenerics(); j++)
			{
				// Get current generic signal name
				std::string parsedGenericStr = result.getGenericName(j);
				
				// Only start to match if length of the name is greater or equal the current maximum match length
				// This prevents shorter names from getting matched
				if (parsedGenericStr.length() >= (size_t) matchLength)
				{
					// Get new maximum match lengths
					matchLength = parsedGenericStr.length();

					// Check if the vector string contains the generic name
					int matchResultStart = parsedVectorStartStr.find(parsedGenericStr);
					if (matchResultStart != -1)
					{
						// Check if both strings have the same last character (e.g. Compare width_1 with width_2)
						if (parsedVectorStartStr[matchResultStart+matchLength-1] == parsedGenericStr[matchLength-1])
							// Set the matched generic string as the new vector string
							result.setVectorStr(i,parsedGenericStr);
					}
					
					// Perform same actions with the vector end string
					// Check if the vector string contains the generic name
					matchResultStart = parsedVectorEndStr.find(parsedGenericStr);
					if (matchResultStart != -1)
					{
						// Check if both strings have the same last character (e.g. Compare width_1 with width_2)
						if (parsedVectorEndStr[matchResultStart+matchLength-1] == parsedGenericStr[matchLength-1])
							// Set the matched generic string as the new vector string
							result.setVectorStr(i,parsedGenericStr);
					}
				}
			}

			// Check if no match was found, only check ports which vector lengths are declared with a variable name
			if ((result.getVectorStr(i) == result.getVectorRawStr(i)) && (result.getVectorLength(i) == 0))
				MSG(GROUP::WARNING) << "Found no matching generic signal for vector length. Using raw input.";
		}
	}
}