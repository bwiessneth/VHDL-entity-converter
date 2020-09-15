/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wieﬂneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: Markdown.cpp
*	Author: Benjamin Wieﬂneth
*	Description: Generate markdown code from parser data
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

#include "Markdown.h"
#include "Config.h"
#include "MSG.h"

Markdown::Markdown(VHDLEntity sourceEntity)
  : OFileHandler(sourceEntity, "Markdown", "md", "markdown")
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL)
    << "Markdown::Markdown(VHDLEntity sourceEntity)";
  MSG(GROUP::DEBUG, DEBUG::RESULT) << "mConfigName = " << mConfigName;
  MSG(GROUP::DEBUG, DEBUG::RESULT) << "mExtension = " << mExtension;
  MSG(GROUP::DEBUG, DEBUG::RESULT) << "mSuffix = " << mSuffix;

  getMaxTextLengths();

  getHeader();
  getRows();

  if (cfg.getBool("Table.exportGenerics")) {
    getGenerics();
  }
}

// Get polarity string of required port
std::string
Markdown::Polarity(int i)
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "Markdown::Polarity(int i)";

  if (mSourceEntity.getLOWActive(i) == true) {
    return cfg.getString("Table.caption_LOWactive");
  } else {
    return cfg.getString("Table.caption_HIGHactive");
  }
}

// Get direction string of required port
std::string
Markdown::Direction(int i)
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "Markdown::Direction(int i)";

  return EntityPort::portDirectionsLabels[mSourceEntity.getPortdirection(i)];
}

// Get type string of required port
std::string
Markdown::Type(int i)
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "Markdown::Type(int i)";

  return mSourceEntity.getPortTypeStr(i);
}

// Generate seperate table for the generic signals
void
Markdown::getGenerics()
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "Markdown::getGenerics()";

  if (mSourceEntity.getNumberOfGenerics() > 0) {
    // Build table headings
    mOutputFile << std::endl << std::endl;

    mOutputFile << "|";

    if (cfg.getBool("Table.boldHeadings"))
      writeCell(colNames::GenericName,
                cfg.getString("Table.GenericName"),
                " **",
                "** ",
                " ");
    else
      writeCell(colNames::GenericName,
                cfg.getString("Table.GenericName"),
                " ",
                " ",
                " ");

    mOutputFile << "|";

    if (cfg.getBool("Table.boldHeadings"))
      writeCell(colNames::GenericType,
                cfg.getString("Table.GenericType"),
                " **",
                "** ",
                " ");
    else
      writeCell(colNames::GenericType,
                cfg.getString("Table.GenericType"),
                " ",
                " ",
                " ");

    mOutputFile << "|";

    if (cfg.getBool("Table.boldHeadings"))
      writeCell(colNames::DefaultValue,
                cfg.getString("Table.GenericDefaultValue"),
                " **",
                "** ",
                " ");
    else
      writeCell(colNames::DefaultValue,
                cfg.getString("Table.GenericDefaultValue"),
                " ",
                " ",
                " ");

    mOutputFile << "|";
    mOutputFile << std::endl;

    // Build table hline
    mOutputFile << "|";

    if (cfg.getBool("Table.centeredGenericName"))
      writeCell(colNames::GenericName, "", ":", ":", "-", true);
    else
      writeCell(colNames::GenericName, "", "", "", "-");

    mOutputFile << "|";

    if (cfg.getBool("Table.centeredGenericType"))
      writeCell(colNames::GenericType, "", ":", ":", "-", true);
    else
      writeCell(colNames::GenericType, "", "", "", "-");

    mOutputFile << "|";

    if (cfg.getBool("Table.centeredGenericDefaultValue"))
      writeCell(colNames::DefaultValue, "", ":", ":", "-", true);
    else
      writeCell(colNames::DefaultValue, "", "", "", "-");

    mOutputFile << "|";

    mOutputFile << std::endl;

    // Build table content
    for (int i = 0; i < mSourceEntity.getNumberOfGenerics(); i++) {
      mOutputFile << "|";
      writeCell(
        colNames::GenericName, mSourceEntity.getGenericName(i), " ", " ", " ");
      mOutputFile << "|";

      std::string tmpStr = mSourceEntity.getGenericType(i);

      if (cfg.getBool("Table.showArrayLength") &&
          mSourceEntity.getGenericIsVector(i)) {
        if (cfg.getBool("Table.arrayNotation"))
          tmpStr +=
            "[" + std::to_string(mSourceEntity.getGenericVectorLength(i)) + "]";
        else
          tmpStr +=
            "[" + std::to_string(mSourceEntity.getGenericVectorStart(i)) + ":" +
            std::to_string(mSourceEntity.getGenericVectorEnd(i)) + "]";
      }
      writeCell(colNames::GenericType, tmpStr, " ", " ", " ");

      mOutputFile << "|";
      writeCell(colNames::DefaultValue,
                mSourceEntity.getGenericDefaultValue(i),
                " ",
                " ",
                " ");
      mOutputFile << "|";

      mOutputFile << std::endl;
    }
  }
}

// Generate the table header
void
Markdown::getHeader()
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "Markdown::getHeader()";

  mOutputFile << "|";

  if (cfg.getBool("Table.boldHeadings"))
    writeCell(
      colNames::Name, cfg.getString("Table.Name_heading"), " **", "** ", " ");
  else
    writeCell(
      colNames::Name, cfg.getString("Table.Name_heading"), " ", " ", " ");

  mOutputFile << "|";

  if ((cfg.getBool("Table.exportType")) &&
      !cfg.getBool("Table.combineNameAndType")) {
    if (cfg.getBool("Table.boldHeadings"))
      writeCell(
        colNames::Type, cfg.getString("Table.Type_heading"), " **", "** ", " ");
    else
      writeCell(
        colNames::Type, cfg.getString("Table.Type_heading"), " ", " ", " ");

    mOutputFile << "|";
  }

  if (cfg.getBool("Table.exportDirection")) {
    if (cfg.getBool("Table.boldHeadings"))
      writeCell(colNames::Direction,
                cfg.getString("Table.Direction_heading"),
                " **",
                "** ",
                " ");
    else
      writeCell(colNames::Direction,
                cfg.getString("Table.Direction_heading"),
                " ",
                " ",
                " ");

    mOutputFile << "|";
  }

  if (cfg.getBool("Table.exportPolarity")) {
    if (cfg.getBool("Table.boldHeadings"))
      writeCell(colNames::Polarity,
                cfg.getString("Table.Polarity_heading"),
                " **",
                "** ",
                " ");
    else
      writeCell(colNames::Polarity,
                cfg.getString("Table.Polarity_heading"),
                " ",
                " ",
                " ");

    mOutputFile << "|";
  }

  if (cfg.getBool("Table.exportDescription")) {
    if (cfg.getBool("Table.boldHeadings"))
      writeCell(colNames::Description,
                cfg.getString("Table.Description_heading"),
                " **",
                "** ",
                " ");
    else
      writeCell(colNames::Description,
                cfg.getString("Table.Description_heading"),
                " ",
                " ",
                " ");

    mOutputFile << "|";
  }

  if (cfg.getBool("Table.exportBlank1")) {
    if (cfg.getBool("Table.boldHeadings"))
      writeCell(colNames::Blank,
                cfg.getString("Table.Blank1_heading"),
                " **",
                "** ",
                " ");
    else
      writeCell(
        colNames::Blank, cfg.getString("Table.Blank1_heading"), " ", " ", " ");

    mOutputFile << "|";
  }

  if (cfg.getBool("Table.exportBlank2")) {
    if (cfg.getBool("Table.boldHeadings"))
      writeCell(colNames::Blank,
                cfg.getString("Table.Blank2_heading"),
                " **",
                "** ",
                " ");
    else
      writeCell(
        colNames::Blank, cfg.getString("Table.Blank2_heading"), " ", " ", " ");

    mOutputFile << "|";
  }
  mOutputFile << "\n";

  // Hline
  mOutputFile << "|";

  if (cfg.getBool("Table.centeredName")) {
    writeCell(colNames::Name, "", ":", ":", "-", true);
  } else
    writeCell(colNames::Name, "", "", "", "-");

  mOutputFile << "|";

  if ((cfg.getBool("Table.exportType")) &&
      !cfg.getBool("Table.combineNameAndType")) {
    if (cfg.getBool("Table.centeredType")) {
      writeCell(colNames::Type, "", ":", ":", "-", true);
    } else
      writeCell(colNames::Type, "", "", "", "-");

    mOutputFile << "|";
  }

  if (cfg.getBool("Table.exportDirection")) {
    if (cfg.getBool("Table.centeredDirection")) {
      writeCell(colNames::Direction, "", ":", ":", "-", true);
    } else
      writeCell(colNames::Direction, "", "", "", "-");

    mOutputFile << "|";
  }

  if (cfg.getBool("Table.exportPolarity")) {
    if (cfg.getBool("Table.centeredPolarity")) {
      writeCell(colNames::Polarity, "", ":", ":", "-", true);
    } else
      writeCell(colNames::Polarity, "", "", "", "-");

    mOutputFile << "|";
  }

  if (cfg.getBool("Table.exportDescription")) {
    if (cfg.getBool("Table.centeredDescription")) {
      writeCell(colNames::Description, "", ":", ":", "-", true);
    } else
      writeCell(colNames::Description, "", "", "", "-");

    mOutputFile << "|";
  }

  if (cfg.getBool("Table.exportBlank1")) {
    if (cfg.getBool("Table.centeredBlank")) {
      writeCell(colNames::Blank, "", ":", ":", "-", true);
    } else
      writeCell(colNames::Blank, "", "", "", "-");

    mOutputFile << "|";
  }

  if (cfg.getBool("Table.exportBlank2")) {
    if (cfg.getBool("Table.centeredBlank")) {
      writeCell(colNames::Blank, "", ":", ":", "-", true);
    } else
      writeCell(colNames::Blank, "", "", "", "-");

    mOutputFile << "|";
  }
  mOutputFile << "\n";
}

// Generate the table content
void
Markdown::getRows()
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "Markdown::getRows()";

  // Get the number of ports
  int noPorts =
    mSourceEntity.getNumberOfInputs() + mSourceEntity.getNumberOfOutputs();

  // Loop through all ports
  for (int i = 0; i < noPorts; i++) {
    // Create rows
    mOutputFile << "|";

    std::string tmpStr = mSourceEntity.getPortName(i);

    if (cfg.getBool("Table.showArrayLength") && mSourceEntity.isVector(i) &&
        cfg.getBool("Table.combineNameAndType")) {
      if (cfg.getBool("Table.arrayNotation"))
        tmpStr += "[" + mSourceEntity.getVectorStr(i) + "]";
      else
        tmpStr += "[" + mSourceEntity.getVectorStartStr(i) + ":" +
                  mSourceEntity.getVectorEndStr(i) + "]";
    }
    writeCell(colNames::Name, tmpStr, " ", " ", " ");
    mOutputFile << "|";

    tmpStr.clear();
    if ((cfg.getBool("Table.exportType")) &&
        !cfg.getBool("Table.combineNameAndType")) {
      tmpStr += Type(i);
      if (cfg.getBool("Table.showArrayLength") && mSourceEntity.isVector(i) &&
          !cfg.getBool("Table.combineNameAndType")) {
        if (cfg.getBool("Table.arrayNotation"))
          tmpStr += "[" + mSourceEntity.getVectorStr(i) + "]";
        else
          tmpStr += "[" + mSourceEntity.getVectorStartStr(i) + ":" +
                    mSourceEntity.getVectorEndStr(i) + "]";
      }
      writeCell(colNames::Type, tmpStr, " ", " ", " ");
      mOutputFile << "|";
    }

    if (cfg.getBool("Table.exportDirection")) {
      writeCell(colNames::Direction, Direction(i), " ", " ", " ");
      mOutputFile << "|";
    }

    if (cfg.getBool("Table.exportPolarity")) {
      writeCell(colNames::Polarity, Polarity(i), " ", " ", " ");
      mOutputFile << "|";
    }

    if (cfg.getBool("Table.exportDescription")) {
      writeCell(colNames::Description, "", "", "", " ");
      mOutputFile << "|";
    }

    if (cfg.getBool("Table.exportBlank1")) {
      writeCell(colNames::Blank, "", "", "", " ");
      mOutputFile << "|";
    }

    if (cfg.getBool("Table.exportBlank2")) {
      writeCell(colNames::Blank, "", "", "", " ");
      mOutputFile << "|";
    }

    mOutputFile << "\n";
  }
}

void
Markdown::getMaxTextLengths()
{
  int formattingCorrection = 0;

  // Get the number of ports
  int noPorts =
    mSourceEntity.getNumberOfInputs() + mSourceEntity.getNumberOfOutputs();

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
  for (int i = 0; i < noPorts; i++) {
    colWidthName = max(colWidthName, mSourceEntity.getPortName(i).length());
    colWidthType = max(colWidthType, mSourceEntity.getPortTypeStr(i).length());

    if (mSourceEntity.isVector(i)) {
      if (cfg.getBool("Table.showArrayLength")) {
        if (cfg.getBool("Table.combineNameAndType")) {
          if (cfg.getBool("Table.arrayNotation"))
            colWidthName = max(colWidthName,
                               mSourceEntity.getPortName(i).length() +
                                 mSourceEntity.getVectorStr(i).length() + 2);
          else
            colWidthName = max(colWidthName,
                               mSourceEntity.getPortName(i).length() +
                                 mSourceEntity.getVectorStartStr(i).length() +
                                 mSourceEntity.getVectorEndStr(i).length() + 3);
        } else if (cfg.getBool("Table.exportType")) {
          if (cfg.getBool("Table.arrayNotation"))
            colWidthType = max(colWidthType,
                               mSourceEntity.getPortTypeStr(i).length() +
                                 mSourceEntity.getVectorStr(i).length() + 2);
          else
            colWidthType = max(colWidthType,
                               mSourceEntity.getPortTypeStr(i).length() +
                                 mSourceEntity.getVectorStartStr(i).length() +
                                 mSourceEntity.getVectorEndStr(i).length() + 3);
        }
      }
    }
  }

  for (int i = 0; i < mSourceEntity.getNumberOfGenerics(); i++) {
    colWidthGenericName =
      max(colWidthGenericName, mSourceEntity.getGenericName(i).length());
    colWidthDefaultValue = max(
      colWidthDefaultValue, mSourceEntity.getGenericDefaultValue(i).length());
    colWidthGenericType =
      max(colWidthGenericType, mSourceEntity.getGenericType(i).length());
  }

  if (cfg.getBool("Table.boldHeadings"))
    formattingCorrection = 4;

  colWidthName =
    max(colWidthName,
        cfg.getString("Table.Name_heading").length() + formattingCorrection);
  colWidthType =
    max(colWidthType,
        cfg.getString("Table.Type_heading").length() + formattingCorrection);
  colWidthDirection = max(colWidthDirection,
                          cfg.getString("Table.Direction_heading").length() +
                            formattingCorrection);
  colWidthPolarity = max(colWidthPolarity,
                         cfg.getString("Table.Polarity_heading").length() +
                           formattingCorrection);
  colWidthDescription = max(
    colWidthDescription,
    cfg.getString("Table.Description_heading").length() + formattingCorrection);
  colWidthBlank =
    max(colWidthBlank,
        cfg.getString("Table.Blank1_heading").length() + formattingCorrection);
  colWidthBlank =
    max(colWidthBlank,
        cfg.getString("Table.Blank2_heading").length() + formattingCorrection);
  colWidthGenericName =
    max(colWidthGenericName,
        cfg.getString("Table.Name_heading").length() + formattingCorrection);
  colWidthGenericType =
    max(colWidthGenericType,
        cfg.getString("Table.Type_heading").length() + formattingCorrection);
  colWidthDefaultValue =
    max(colWidthDefaultValue,
        std::string("Default value").length() + formattingCorrection);

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
  MSG(GROUP::DEBUG, DEBUG::RESULT)
    << "colWidthDirection = " << colWidthDirection;
  MSG(GROUP::DEBUG, DEBUG::RESULT) << "colWidthPolarity = " << colWidthPolarity;
  MSG(GROUP::DEBUG, DEBUG::RESULT)
    << "colWidthDescription = " << colWidthDescription;
  MSG(GROUP::DEBUG, DEBUG::RESULT) << "colWidthBlank = " << colWidthBlank;
  MSG(GROUP::DEBUG, DEBUG::RESULT)
    << "colWidthGenericName = " << colWidthGenericName;
  MSG(GROUP::DEBUG, DEBUG::RESULT)
    << "colWidthDefaultValue = " << colWidthDefaultValue;
  MSG(GROUP::DEBUG, DEBUG::RESULT)
    << "colWidthGenericType = " << colWidthGenericType;
}

// Write a text field to the output file
// Capable of inserting filling chars to create nice looking alignments
void
Markdown::writeCell(colNames column,
                    std::string text,
                    std::string prefix,
                    std::string suffix,
                    std::string fillChar,
                    bool formatting)
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL)
    << "TableGeneratorUtils::writeText(std::string text, std::string fillChar)";

  size_t textLength = 0;
  size_t maxTextLength = 0;
  switch (column) {
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

  if (!formatting) {
    mOutputFile << suffix;
    textLength += suffix.length();

    if (textLength < maxTextLength) {
      MSG(GROUP::DEBUG, DEBUG::RESULT)
        << "if (textLength + suffix.length() < maxTextLength)";

      for (size_t i = 0; i < maxTextLength - textLength; i++) {
        MSG(GROUP::DEBUG, DEBUG::RESULT) << "fillChar";
        mOutputFile << fillChar;
      }
    }
  } else {
    if (textLength < maxTextLength) {
      MSG(GROUP::DEBUG, DEBUG::RESULT)
        << "if (textLength + suffix.length() < maxTextLength)";

      for (size_t i = 0; i < maxTextLength - textLength - suffix.length();
           i++) {
        MSG(GROUP::DEBUG, DEBUG::RESULT) << "fillChar";
        mOutputFile << fillChar;
      }
    }

    mOutputFile << suffix;
    textLength += suffix.length();
  }
}