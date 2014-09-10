/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wieﬂneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: DWMarkup.cpp
*	Author: Benjamin Wieﬂneth
*	Description: Generate Doku Wiki table markup code from parser data
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

#include "DWMarkup.h"
#include "MSG.h"
#include "Config.h"

DWMarkup::DWMarkup(VHDLEntity sourceEntity)
:OFileHandler(sourceEntity, "DokuWiki", "txt", "markup")
{
	MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "DWMarkup::DWMarkup(VHDLEntity sourceEntity)";
	MSG(GROUP::DEBUG, DEBUG::RESULT) << "mConfigName = " << mConfigName;
	MSG(GROUP::DEBUG, DEBUG::RESULT) << "mExtension = " << mExtension;
	MSG(GROUP::DEBUG, DEBUG::RESULT) << "mSuffix = " << mSuffix;

	getMaxTextLengths();

	getHeader();
	getRows();

	if (cfg.getBool("Table.exportGenerics"))
	{
		getGenerics();
	}
}


// Get polarity string of required port
std::string DWMarkup::Polarity(int i)
{
	MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "DWMarkup::Polarity(int i)";

	if (mSourceEntity.getLOWActive(i) == true)
	{
		return cfg.getString("Table.caption_LOWactive");
	}
	else
	{
		return cfg.getString("Table.caption_HIGHactive");
	}
}

// Get direction string of required port
std::string DWMarkup::Direction(int i)
{
	MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "DWMarkup::Direction(int i)";

	return EntityPort::portDirectionsLabels[mSourceEntity.getPortdirection(i)];
}

// Get type string of required port
std::string DWMarkup::Type(int i)
{
	MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "DWMarkup::Type(int i)";

	return mSourceEntity.getPortTypeStr(i);
}

// Generate seperate table for the generic signals
void DWMarkup::getGenerics()
{
	MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "DWMarkup::getGenerics()";

	if (mSourceEntity.getNumberOfGenerics() > 0)
	{
		mOutputFile << std::endl;
		mOutputFile << std::endl;

		mOutputFile << "^";
		std::string outputStr = "";

		if (cfg.getBool("Table.centeredGenericName"))
			outputStr += " ";

		if (cfg.getBool("Table.boldHeadings"))
			outputStr += "**";

		outputStr += cfg.getString("Table.GenericName");

		if (cfg.getBool("Table.boldHeadings"))
			outputStr += "**";

		if (cfg.getBool("Table.centeredGenericName"))
			outputStr += " ";

		writeCell(colNames::GenericName, outputStr, " ", " ", " ");
		mOutputFile << "^";
		outputStr.clear();

		if (cfg.getBool("Table.centeredGenericType"))
			outputStr += " ";

		if (cfg.getBool("Table.boldHeadings"))
			outputStr += "**";

		outputStr += cfg.getString("Table.GenericType");

		if (cfg.getBool("Table.boldHeadings"))
			outputStr += "**";

		if (cfg.getBool("Table.centeredGenericType"))
			outputStr += " ";

		writeCell(colNames::GenericType, outputStr, " ", " ", " ");
		mOutputFile << "^";
		outputStr.clear();

		if (cfg.getBool("Table.centeredGenericDefaultValue"))
			outputStr += " ";

		if (cfg.getBool("Table.boldHeadings"))
			outputStr += "**";

		outputStr += cfg.getString("Table.GenericDefaultValue");

		if (cfg.getBool("Table.boldHeadings"))
			outputStr += "**";

		if (cfg.getBool("Table.centeredGenericDefaultValue"))
			outputStr += " ";

		writeCell(colNames::DefaultValue, outputStr, " ", " ", " ");
		mOutputFile << "^";
		outputStr.clear();
		mOutputFile << std::endl;

		for (int i = 0; i < mSourceEntity.getNumberOfGenerics(); i++)
		{
			mOutputFile << "|";
			
			if (cfg.getBool("Table.centeredGenericName"))
				outputStr += " ";

			outputStr += mSourceEntity.getGenericName(i);
			
			if (cfg.getBool("Table.centeredGenericName"))
				outputStr += " ";
			
			writeCell(colNames::GenericName, outputStr, " ", " ", " ");
			outputStr.clear();
			mOutputFile << "|";


			if (cfg.getBool("Table.centeredGenericType"))
				outputStr += " ";

			outputStr += mSourceEntity.getGenericType(i);
			if (cfg.getBool("Table.showArrayLength") && mSourceEntity.getGenericIsVector(i))
			{
				if (cfg.getBool("Table.arrayNotation"))
					outputStr += "[" + std::to_string(mSourceEntity.getGenericVectorLength(i)) + "]";
				else
					outputStr += "[" + std::to_string(mSourceEntity.getGenericVectorStart(i)) + ":" + std::to_string(mSourceEntity.getGenericVectorEnd(i)) + "]";
			}
						
			if (cfg.getBool("Table.centeredGenericType"))
				outputStr += " ";
			
			writeCell(colNames::GenericType, outputStr, " ", " ", " ");
			outputStr.clear();
			mOutputFile << "|";

			if (cfg.getBool("Table.centeredGenericDefaultValue"))
				outputStr += " ";

			outputStr += mSourceEntity.getGenericDefaultValue(i);

			if (cfg.getBool("Table.centeredGenericDefaultValue"))
				outputStr += " ";

			writeCell(colNames::DefaultValue, outputStr, " ", " ", " ");
			outputStr.clear();
			mOutputFile << "|";
			mOutputFile << std::endl;
		}
	}
}

// Generate the table header
void DWMarkup::getHeader()
{
	MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "DWMarkup::getHeader()";

	mOutputFile << "^";
	std::string tmpStr = "";	

	if (cfg.getBool("Table.centeredName"))
		tmpStr += " ";
	
	if (cfg.getBool("Table.boldHeadings"))
		tmpStr += "**";

	tmpStr += cfg.getString("Table.Name_heading");

	if (cfg.getBool("Table.boldHeadings"))
		tmpStr += "**";

	if (cfg.getBool("Table.centeredName"))
		tmpStr += " ";

	writeCell(colNames::Name, tmpStr, " ", " ", " ");

	mOutputFile << "^";

	if ((cfg.getBool("Table.exportType")) && !cfg.getBool("Table.combineNameAndType"))
	{
		tmpStr.clear();

		if (cfg.getBool("Table.centeredType"))
			tmpStr += " ";

		if (cfg.getBool("Table.boldHeadings"))
			tmpStr += "**";

		tmpStr += cfg.getString("Table.Type_heading");

		if (cfg.getBool("Table.boldHeadings"))
			tmpStr += "**";

		if (cfg.getBool("Table.centeredType"))
			tmpStr += " ";

		writeCell(colNames::Type, tmpStr, " ", " ", " ");

		mOutputFile << "^";
	}

	if (cfg.getBool("Table.exportDirection"))
	{
		tmpStr.clear();
		if (cfg.getBool("Table.centeredDirection"))
			tmpStr += " ";

		if (cfg.getBool("Table.boldHeadings"))
			tmpStr += "**";

		tmpStr += cfg.getString("Table.Direction_heading");

		if (cfg.getBool("Table.boldHeadings"))
			tmpStr += "**";

		if (cfg.getBool("Table.centeredDirection"))
			tmpStr += " ";

		writeCell(colNames::Direction, tmpStr, " ", " ", " ");

		mOutputFile << "^";
	}

	if (cfg.getBool("Table.exportPolarity"))
	{
		tmpStr.clear();

		if (cfg.getBool("Table.centeredPolarity"))
			tmpStr += " ";

		if (cfg.getBool("Table.boldHeadings"))
			tmpStr += "**";

		tmpStr += cfg.getString("Table.Polarity_heading");

		if (cfg.getBool("Table.boldHeadings"))
			tmpStr += "**";

		if (cfg.getBool("Table.centeredPolarity"))
			tmpStr += " ";

		writeCell(colNames::Polarity, tmpStr, " ", " ", " ");

		mOutputFile << "^";
	}

	if (cfg.getBool("Table.exportDescription"))
	{
		tmpStr.clear();

		if (cfg.getBool("Table.centeredDescription"))
			tmpStr += " ";
		
		if (cfg.getBool("Table.boldHeadings"))
			tmpStr += "**";
		
		tmpStr += cfg.getString("Table.Description_heading");
		
		if (cfg.getBool("Table.boldHeadings"))
			tmpStr += "**";
		
		if (cfg.getBool("Table.centeredDescription"))
			tmpStr += " ";

		writeCell(colNames::Description, tmpStr, " ", " ", " ");

		mOutputFile << "^";
	}

	if (cfg.getBool("Table.exportBlank1"))
	{
		tmpStr.clear();

		if (cfg.getBool("Table.centeredBlank"))
			tmpStr += " ";

		if (cfg.getBool("Table.boldHeadings"))
			tmpStr += "**";

		tmpStr += cfg.getString("Table.Blank1_heading");
		
		if (cfg.getBool("Table.boldHeadings"))
			tmpStr += "**";
		
		if (cfg.getBool("Table.centeredBlank"))
			tmpStr += " ";
		
		writeCell(colNames::Blank, tmpStr, " ", " ", " ");
		
		mOutputFile << "^";
	}

	if (cfg.getBool("Table.exportBlank2"))
	{
		tmpStr.clear();

		if (cfg.getBool("Table.centeredBlank"))
			tmpStr += " ";

		if (cfg.getBool("Table.boldHeadings"))
			tmpStr += "**";

		tmpStr += cfg.getString("Table.Blank2_heading");

		if (cfg.getBool("Table.boldHeadings"))
			tmpStr += "**";

		if (cfg.getBool("Table.centeredBlank"))
			tmpStr += " ";

		writeCell(colNames::Blank, tmpStr, " ", " ", " ");

		mOutputFile << "^";
	}
	mOutputFile << "\n";
}

// Generate the table content
void DWMarkup::getRows()
{
	MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "DWMarkup::getRows()";

	// Get the number of ports
	int noPorts = mSourceEntity.getNumberOfInputs() + mSourceEntity.getNumberOfOutputs();

	// Loop through all ports
	for (int i = 0; i < noPorts; i++)
	{
		std::string outputString = "";
		// Create DokuWiki rows
		mOutputFile << "|";

		if (cfg.getBool("Table.centeredName"))
			outputString = " ";

		outputString += mSourceEntity.getPortName(i);

		if (cfg.getBool("Table.showArrayLength") && mSourceEntity.isVector(i) && cfg.getBool("Table.combineNameAndType"))
		{
			if (cfg.getBool("Table.arrayNotation"))
				outputString += "[" + mSourceEntity.getVectorStr(i) + "]";
			else
				outputString += "[" + mSourceEntity.getVectorStartStr(i) + ":" + mSourceEntity.getVectorEndStr(i) + "]";
		}

		if (cfg.getBool("Table.centeredName"))
			outputString += " ";

		writeCell(colNames::Name, outputString, " ", " ", " ");
		outputString.clear();
		mOutputFile << "|";

		if ((cfg.getBool("Table.exportType")) && !cfg.getBool("Table.combineNameAndType"))
		{
			if (cfg.getBool("Table.centeredType"))
				outputString = " ";
			
			outputString += Type(i);

			if (cfg.getBool("Table.showArrayLength") &&  mSourceEntity.isVector(i) && !cfg.getBool("Table.combineNameAndType"))
			{
				if (cfg.getBool("Table.arrayNotation"))
					outputString += "[" + mSourceEntity.getVectorStr(i) + "]";
				else
					outputString += "[" + mSourceEntity.getVectorStartStr(i) + ":" + mSourceEntity.getVectorEndStr(i) + "]";
			}

			if (cfg.getBool("Table.centeredType"))
				outputString += " ";

			writeCell(colNames::Type, outputString, " ", " ", " ");
			outputString.clear();
			mOutputFile << "|";
		}

		if (cfg.getBool("Table.exportDirection"))
		{
			if (cfg.getBool("Table.centeredDirection"))
				writeCell(colNames::Direction, Direction(i), "  ", "  ", " ");
			else
				writeCell(colNames::Direction, Direction(i), " ", " ", " ");
			
			mOutputFile << "|";
		}
		if (cfg.getBool("Table.exportPolarity"))
		{
			if (cfg.getBool("Table.centeredPolarity"))
				writeCell(colNames::Polarity, Polarity(i), "  ", "  ", " ");
			else
				writeCell(colNames::Polarity, Polarity(i), " ", " ", " ");
			
			mOutputFile << "|";
		}
		if (cfg.getBool("Table.exportDescription"))
		{
			if (cfg.getBool("Table.centeredDescription"))
				writeCell(colNames::Description, " ", "  ", "  ", " ");
			else
				writeCell(colNames::Description, " ", "", "", " ");
			mOutputFile << "|";
		}
		if (cfg.getBool("Table.exportBlank1"))
		{
			mOutputFile << " ";
			mOutputFile << "|";
		}
		if (cfg.getBool("Table.exportBlank2"))
		{
			mOutputFile << " ";
			mOutputFile << "|";
		}
		mOutputFile << "\n";
	}
}

void DWMarkup::getMaxTextLengths()
{
	int formattingCorrection = 0;

	// Get the number of ports
	int noPorts = mSourceEntity.getNumberOfInputs() + mSourceEntity.getNumberOfOutputs();

	colWidthName = 0;
	colWidthType = 0;
	colWidthDirection = 0;
	colWidthPolarity = 0;
	colWidthDescription = 0;
	colWidthBlank = 0;
	colWidthGenericName = 0;
	colWidthGenericType = 0;
	colWidthDefaultValue = 0;

	// Loop through all ports
	for (int i = 0; i < noPorts; i++)
	{
		colWidthName = max(colWidthName, mSourceEntity.getPortName(i).length());
		colWidthType = max(colWidthType, mSourceEntity.getPortTypeStr(i).length());

		if (mSourceEntity.isVector(i))
		{
			if (cfg.getBool("Table.showArrayLength"))
			{
				if (cfg.getBool("Table.combineNameAndType"))
				{
					if (cfg.getBool("Table.arrayNotation"))
						colWidthName = max(colWidthName, mSourceEntity.getPortName(i).length() + mSourceEntity.getVectorStr(i).length() + 2);
					else
						colWidthName = max(colWidthName, mSourceEntity.getPortName(i).length() + mSourceEntity.getVectorStartStr(i).length() + mSourceEntity.getVectorEndStr(i).length() + 3);
				}
				else if (cfg.getBool("Table.exportType"))
				{
					if (cfg.getBool("Table.arrayNotation"))
						colWidthType = max(colWidthType, mSourceEntity.getPortTypeStr(i).length() + mSourceEntity.getVectorStr(i).length() + 2);
					else
						colWidthType = max(colWidthType, mSourceEntity.getPortTypeStr(i).length() + mSourceEntity.getVectorStartStr(i).length() + mSourceEntity.getVectorEndStr(i).length() + 3);
				}
			}
		}
	}

	for (int i = 0; i < mSourceEntity.getNumberOfGenerics(); i++)
	{
		colWidthGenericName = max(colWidthGenericName, mSourceEntity.getGenericName(i).length());
		colWidthDefaultValue = max(colWidthDefaultValue, mSourceEntity.getGenericDefaultValue(i).length());
		colWidthGenericType = max(colWidthGenericType, mSourceEntity.getGenericType(i).length());
	}

	if (cfg.getBool("Table.boldHeadings"))
		formattingCorrection = 4;
	
	colWidthName = max(colWidthName + (cfg.getBool("Table.centeredName") ? 2 : 0), cfg.getString("Table.Name_heading").length() + formattingCorrection + (cfg.getBool("Table.centeredName") ? 2 : 0));
	colWidthType = max(colWidthType +  (cfg.getBool("Table.centeredType") ? 2 : 0), cfg.getString("Table.Type_heading").length() + formattingCorrection + (cfg.getBool("Table.centeredType") ? 2 : 0));
	colWidthDirection = max(colWidthDirection +  (cfg.getBool("Table.centeredDirection") ? 2 : 0), cfg.getString("Table.Direction_heading").length() + formattingCorrection + (cfg.getBool("Table.centeredDirection") ? 2 : 0));
	colWidthPolarity = max(colWidthPolarity +  (cfg.getBool("Table.centeredPolarity") ? 2 : 0), cfg.getString("Table.Polarity_heading").length() + formattingCorrection + (cfg.getBool("Table.centeredPolarity") ? 2 : 0));
	colWidthDescription = max(colWidthDescription +  (cfg.getBool("Table.centeredDescription") ? 2 : 0), cfg.getString("Table.Description_heading").length() + formattingCorrection + (cfg.getBool("Table.centeredDescription") ? 2 : 0));
	colWidthBlank = max(colWidthBlank +  (cfg.getBool("Table.centeredBlank") ? 2 : 0), cfg.getString("Table.Blank1_heading").length() + formattingCorrection + (cfg.getBool("Table.centeredBlank") ? 2 : 0));
	colWidthBlank = max(colWidthBlank +  (cfg.getBool("Table.centeredBlank") ? 2 : 0), cfg.getString("Table.Blank2_heading").length() + formattingCorrection + (cfg.getBool("Table.centeredBlank") ? 2 : 0));
	colWidthGenericName = max(colWidthGenericName + (cfg.getBool("Table.centeredGenericName") ? 2 : 0), cfg.getString("Table.Name_heading").length() + formattingCorrection + (cfg.getBool("Table.centeredGenericName") ? 2 : 0));
	colWidthGenericType = max(colWidthGenericType + (cfg.getBool("Table.centeredGenericType") ? 2 : 0), cfg.getString("Table.Type_heading").length() + formattingCorrection + (cfg.getBool("Table.centeredGenericType") ? 2 : 0));
	colWidthDefaultValue = max(colWidthDefaultValue + (cfg.getBool("Table.centeredGenericDefaultValue") ? 2 : 0), std::string("Default value").length() + formattingCorrection + (cfg.getBool("Table.centeredGenericDefaultValue") ? 2 : 0));

	colWidthName += 2;
	colWidthType += 2;
	colWidthDirection += 2;
	colWidthPolarity += 2;
	colWidthDescription += 2;
	colWidthBlank += 2;
	colWidthGenericName += 2;
	colWidthDefaultValue += 2;
	colWidthGenericType += 2;

	MSG(GROUP::DEBUG, DEBUG::RESULT) << "colWidthName = " << colWidthName;
	MSG(GROUP::DEBUG, DEBUG::RESULT) << "colWidthType = " << colWidthType;
	MSG(GROUP::DEBUG, DEBUG::RESULT) << "colWidthDirection = " << colWidthDirection;
	MSG(GROUP::DEBUG, DEBUG::RESULT) << "colWidthPolarity = " << colWidthPolarity;
	MSG(GROUP::DEBUG, DEBUG::RESULT) << "colWidthDescription = " << colWidthDescription;
	MSG(GROUP::DEBUG, DEBUG::RESULT) << "colWidthBlank = " << colWidthBlank;
	MSG(GROUP::DEBUG, DEBUG::RESULT) << "colWidthGenericName = " << colWidthGenericName;
	MSG(GROUP::DEBUG, DEBUG::RESULT) << "colWidthDefaultValue = " << colWidthDefaultValue;
	MSG(GROUP::DEBUG, DEBUG::RESULT) << "colWidthGenericType = " << colWidthGenericType;
}

// Write a text field to the output file
// Capable of inserting filling chars to create nice looking alignments
void DWMarkup::writeCell(colNames column, std::string text, std::string prefix, std::string suffix, std::string fillChar, bool formatting)
{
	MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "TableGeneratorUtils::writeText(std::string text, std::string fillChar)";

	size_t textLength = 0;
	size_t maxTextLength = 0;
	switch (column)
	{
	case colNames::Name:
		maxTextLength = colWidthName;
		break;
	case colNames::Type:
		maxTextLength = colWidthType;
		break;
	case colNames::Direction:
		maxTextLength = colWidthDirection;
		break;
	case colNames::Polarity:
		maxTextLength = colWidthPolarity;
		break;
	case colNames::Description:
		maxTextLength = colWidthDescription;
		break;
	case colNames::Blank:
		maxTextLength = colWidthBlank;
		break;
	case colNames::GenericName:
		maxTextLength = colWidthGenericName;
		break;
	case colNames::GenericType:
		maxTextLength = colWidthGenericType;
		break;
	case colNames::DefaultValue:
		maxTextLength = colWidthDefaultValue;
		break;
	}

	mOutputFile << prefix;
	textLength += prefix.length();

	mOutputFile << text;
	textLength += text.length();

	if (!formatting)
	{
		mOutputFile << suffix;
		textLength += suffix.length();

		if (textLength < maxTextLength)
		{
			MSG(GROUP::DEBUG, DEBUG::RESULT) << "if (textLength + suffix.length() < maxTextLength)";

			for (size_t i = 0; i < maxTextLength - textLength; i++)
			{
				MSG(GROUP::DEBUG, DEBUG::RESULT) << "fillChar";
				mOutputFile << fillChar;
			}
		}
	}
	else
	{
		if (textLength < maxTextLength)
		{
			MSG(GROUP::DEBUG, DEBUG::RESULT) << "if (textLength + suffix.length() < maxTextLength)";

			for (size_t i = 0; i < maxTextLength - textLength - suffix.length(); i++)
			{
				MSG(GROUP::DEBUG, DEBUG::RESULT) << "fillChar";
				mOutputFile << fillChar;
			}
		}

		mOutputFile << suffix;
		textLength += suffix.length();
	}
}