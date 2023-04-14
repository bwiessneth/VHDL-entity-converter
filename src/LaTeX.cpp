/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wieﬂneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: LaTeX.cpp
*	Author: Benjamin Wieﬂneth
*	Description: Generate LaTeX tables from parser data
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

#include "LaTeX.h"
#include "Config.h"
#include "MSG.h"

LaTeX::LaTeX(VHDLEntity sourceEntity)
  : OFileHandler(sourceEntity, "LaTeX", "tex")
{
  MSG(LOG_LEVEL::DEBUG, DEBUG::FUNCTIONCALL)
    << "LaTeX::LaTeX(VHDLEntity sourceEntity)";
  MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT) << "mConfigName = " << mConfigName;
  MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT) << "mExtension = " << mExtension;
  MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT) << "mSuffix = " << mSuffix;

  getMaxTextLengths();

  getTable();
}

// Get polarity string of required port
std::string
LaTeX::Polarity(int i)
{
  MSG(LOG_LEVEL::DEBUG, DEBUG::FUNCTIONCALL) << "LaTeX::Polarity(int i)";

  if (mSourceEntity.getLOWActive(i) == true) {
    return cfg.getString("Table.caption_LOWactive");
  } else {
    return cfg.getString("Table.caption_HIGHactive");
  }
}

// Get direction string of required port
std::string
LaTeX::Direction(int i)
{
  MSG(LOG_LEVEL::DEBUG, DEBUG::FUNCTIONCALL) << "LaTeX::Direction(int i)";

  return EntityPort::portDirectionsLabels[mSourceEntity.getPortdirection(i)];
}

// Get type string of required port
std::string
LaTeX::Type(int i)
{
  MSG(LOG_LEVEL::DEBUG, DEBUG::FUNCTIONCALL) << "LaTeX::Type(int i)";

  return mSourceEntity.getPortTypeStr(i);
}

// Write LaTeX table code into output file
void
LaTeX::getTable()
{
  if (cfg.getBool("LaTeX.addTable")) {
    mOutputFile << "\\begin{table}[htbp]" << std::endl;

    if (cfg.getBool("LaTeX.centering"))
      mOutputFile << "\\centering" << std::endl;

    if (cfg.getBool("LaTeX.caption"))
      mOutputFile << "\\caption{"
                  << replaceStr(mSourceEntity.getEntityName(), "_", "\\_")
                  << "}" << std::endl;

    if (cfg.getBool("LaTeX.label"))
      mOutputFile << "\\label{ref:" << mSourceEntity.getEntityName() << "}"
                  << std::endl;
  }

  getHeaderPorts();
  getPorts();

  if (cfg.getBool("Table.exportGenerics") && mSourceEntity.getNumberOfGenerics() > 0) {
    if (cfg.getBool("LaTeX.addTable"))
      mOutputFile << "\\vspace*{ 1 em }" << std::endl << std::endl;

    getHeaderGenerics();
    getGenerics();
  }

  if (cfg.getBool("LaTeX.addTable")) {
    mOutputFile << "\\end{table}" << std::endl;
  }
}

// Generate table for the generic signals
void
LaTeX::getGenerics()
{
  MSG(LOG_LEVEL::DEBUG, DEBUG::FUNCTIONCALL) << "LaTeX::getGenerics()";

  // Build table content
  for (int i = 0; i < mSourceEntity.getNumberOfGenerics(); i++) {
    writeCell(
      colNames::GenericName, mSourceEntity.getGenericName(i), " ", " ", " ");
    mOutputFile << "&";

    std::string tmpStr = mSourceEntity.getGenericType(i);

    if (cfg.getBool("Table.showArrayLength") &&
        mSourceEntity.getGenericIsVector(i)) {
      if (cfg.getBool("Table.arrayNotation"))
        tmpStr +=
          "[" + std::to_string(mSourceEntity.getGenericVectorLength(i)) + "]";
      else
        tmpStr += "[" + std::to_string(mSourceEntity.getGenericVectorStart(i)) +
                  ":" + std::to_string(mSourceEntity.getGenericVectorEnd(i)) +
                  "]";
    }
    writeCell(colNames::GenericType, tmpStr, " ", " ", " ");

    mOutputFile << "&";
    writeCell(colNames::DefaultValue,
              mSourceEntity.getGenericDefaultValue(i),
              " ",
              " ",
              " ");
    mOutputFile << "\\\\";

    mOutputFile << std::endl;
  }

  mOutputFile << "\\hline" << std::endl << "\\end{tabular}" << std::endl;
}

// Generate the table header for the generic signals
void
LaTeX::getHeaderGenerics()
{
  MSG(LOG_LEVEL::DEBUG, DEBUG::FUNCTIONCALL) << "LaTeX::getHeaderGenerics()";

  mOutputFile << "\\begin{tabular}{ ";

  if (cfg.getBool("Table.centeredGenericName"))
    mOutputFile << "c ";
  else
    mOutputFile << "l ";

  if (cfg.getBool("Table.centeredGenericType"))
    mOutputFile << "c ";
  else
    mOutputFile << "l ";

  if (cfg.getBool("Table.centeredGenericDefaultValue"))
    mOutputFile << "c ";
  else
    mOutputFile << "l ";

  mOutputFile << "}" << std::endl;
  mOutputFile << "\\hline" << std::endl;

  if (cfg.getBool("Table.boldHeadings"))
    writeCell(colNames::Name,
              cfg.getString("Table.GenericName"),
              "\\textbf{",
              "} ",
              " ");
  else
    writeCell(colNames::Name, cfg.getString("Table.GenericName"), "", "", " ");

  mOutputFile << "&";

  if (cfg.getBool("Table.boldHeadings"))
    writeCell(colNames::Type,
              cfg.getString("Table.GenericType"),
              " \\textbf{",
              "} ",
              " ");
  else
    writeCell(
      colNames::Type, cfg.getString("Table.GenericType"), " ", " ", " ");

  mOutputFile << "&";

  if (cfg.getBool("Table.boldHeadings"))
    writeCell(colNames::DefaultValue,
              cfg.getString("Table.GenericDefaultValue"),
              " \\textbf{",
              "} ",
              " ");
  else
    writeCell(colNames::DefaultValue,
              cfg.getString("Table.GenericDefaultValue"),
              " ",
              " ",
              " ");

  mOutputFile << "\\\\" << std::endl;
  mOutputFile << "\\hline" << std::endl;
}

// Generate the table header for the IO ports
void
LaTeX::getHeaderPorts()
{
  MSG(LOG_LEVEL::DEBUG, DEBUG::FUNCTIONCALL) << "LaTeX::getHeaderPorts()";

  mOutputFile << "\\begin{tabular}{ ";

  if (cfg.getBool("Table.centeredName"))
    mOutputFile << "c ";
  else
    mOutputFile << "l ";

  if (cfg.getBool("Table.exportType")) {
    if (cfg.getBool("Table.centeredType"))
      mOutputFile << "c ";
    else
      mOutputFile << "l ";
  }

  if (cfg.getBool("Table.exportDirection")) {
    if (cfg.getBool("Table.centeredDirection"))
      mOutputFile << "c ";
    else
      mOutputFile << "l ";
  }

  if (cfg.getBool("Table.exportPolarity")) {
    if (cfg.getBool("Table.centeredPolarity"))
      mOutputFile << "c ";
    else
      mOutputFile << "l ";
  }

  if (cfg.getBool("Table.exportDescription")) {
    if (cfg.getBool("Table.centeredDescription"))
      mOutputFile << "c ";
    else
      mOutputFile << "l ";
  }

  if (cfg.getBool("Table.exportBlank1")) {
    if (cfg.getBool("Table.centeredBlank"))
      mOutputFile << "c ";
    else
      mOutputFile << "l ";
  }

  if (cfg.getBool("Table.exportBlank2")) {
    if (cfg.getBool("Table.centeredBlank"))
      mOutputFile << "c ";
    else
      mOutputFile << "l ";
  }

  mOutputFile << "}" << std::endl;
  mOutputFile << "\\hline" << std::endl;

  if (cfg.getBool("Table.boldHeadings"))
    writeCell(colNames::Name,
              cfg.getString("Table.Name_heading"),
              "\\textbf{",
              "} ",
              " ");
  else
    writeCell(colNames::Name, cfg.getString("Table.Name_heading"), "", "", " ");

  if ((cfg.getBool("Table.exportType")) &&
      !cfg.getBool("Table.combineNameAndType")) {
    mOutputFile << "&";

    if (cfg.getBool("Table.boldHeadings"))
      writeCell(colNames::Type,
                cfg.getString("Table.Type_heading"),
                " \\textbf{",
                "} ",
                " ");
    else
      writeCell(
        colNames::Type, cfg.getString("Table.Type_heading"), " ", " ", " ");
  }

  if (cfg.getBool("Table.exportDirection")) {
    mOutputFile << "&";

    if (cfg.getBool("Table.boldHeadings"))
      writeCell(colNames::Direction,
                cfg.getString("Table.Direction_heading"),
                " \\textbf{",
                "} ",
                " ");
    else
      writeCell(colNames::Direction,
                cfg.getString("Table.Direction_heading"),
                " ",
                " ",
                " ");
  }

  if (cfg.getBool("Table.exportPolarity")) {
    mOutputFile << "&";

    if (cfg.getBool("Table.boldHeadings"))
      writeCell(colNames::Polarity,
                cfg.getString("Table.Polarity_heading"),
                " \\textbf{",
                "} ",
                " ");
    else
      writeCell(colNames::Polarity,
                cfg.getString("Table.Polarity_heading"),
                " ",
                " ",
                " ");
  }

  if (cfg.getBool("Table.exportDescription")) {
    mOutputFile << "&";

    if (cfg.getBool("Table.boldHeadings"))
      writeCell(colNames::Description,
                cfg.getString("Table.Description_heading"),
                " \\textbf{",
                "} ",
                " ");
    else
      writeCell(colNames::Description,
                cfg.getString("Table.Description_heading"),
                " ",
                " ",
                " ");
  }

  if (cfg.getBool("Table.exportBlank1")) {
    mOutputFile << "&";

    if (cfg.getBool("Table.boldHeadings"))
      writeCell(colNames::Blank,
                cfg.getString("Table.Blank1_heading"),
                " \\textbf{",
                "} ",
                " ");
    else
      writeCell(
        colNames::Blank, cfg.getString("Table.Blank1_heading"), " ", " ", " ");
  }

  if (cfg.getBool("Table.exportBlank2")) {
    mOutputFile << "&";

    if (cfg.getBool("Table.boldHeadings"))
      writeCell(colNames::Blank,
                cfg.getString("Table.Blank2_heading"),
                " \\textbf{",
                "} ",
                " ");
    else
      writeCell(
        colNames::Blank, cfg.getString("Table.Blank2_heading"), " ", " ", " ");
  }
  mOutputFile << "\\\\" << std::endl;
  mOutputFile << "\\hline" << std::endl;
}

// Generate table for the IO ports
void
LaTeX::getPorts()
{
  MSG(LOG_LEVEL::DEBUG, DEBUG::FUNCTIONCALL) << "LaTeX::getRows()";

  // Get the number of ports
  int noPorts =
    mSourceEntity.getNumberOfInputs() + mSourceEntity.getNumberOfOutputs();

  // Loop through all ports
  for (int i = 0; i < noPorts; i++) {
    // Create rows
    std::string tmpStr = mSourceEntity.getPortName(i);

    if (cfg.getBool("Table.showArrayLength") && mSourceEntity.isVector(i) &&
        cfg.getBool("Table.combineNameAndType")) {
      if (cfg.getBool("Table.arrayNotation"))
        tmpStr += "[" + mSourceEntity.getVectorStr(i) + "]";
      else
        tmpStr += "[" + mSourceEntity.getVectorStartStr(i) + ":" +
                  mSourceEntity.getVectorEndStr(i) + "]";
    }
    writeCell(colNames::Name, tmpStr, "", "", " ");

    tmpStr.clear();
    if ((cfg.getBool("Table.exportType")) &&
        !cfg.getBool("Table.combineNameAndType")) {
      mOutputFile << "&";

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
    }

    if (cfg.getBool("Table.exportDirection")) {
      mOutputFile << "&";
      writeCell(colNames::Direction, Direction(i), " ", " ", " ");
    }

    if (cfg.getBool("Table.exportPolarity")) {
      mOutputFile << "&";
      writeCell(colNames::Polarity, Polarity(i), " ", " ", " ");
    }

    if (cfg.getBool("Table.exportDescription")) {
      mOutputFile << "&";
      writeCell(colNames::Description, "", "", "", " ");
    }

    if (cfg.getBool("Table.exportBlank1")) {
      mOutputFile << "&";
      writeCell(colNames::Blank, "", "", "", " ");
    }

    if (cfg.getBool("Table.exportBlank2")) {
      mOutputFile << "&";
      writeCell(colNames::Blank, "", "", "", " ");
    }

    mOutputFile << "\\\\" << std::endl;
  }

  mOutputFile << "\\hline" << std::endl << "\\end{tabular}" << std::endl;
}

// Get the maximum text lengths for each column in order to align the table
// content
void
LaTeX::getMaxTextLengths()
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
    colWidthName =
      max(colWidthName,
          replaceStr(mSourceEntity.getPortName(i), "_", "\\_").length());
    colWidthType =
      max(colWidthType,
          replaceStr(mSourceEntity.getPortTypeStr(i), "_", "\\_").length());

    if (mSourceEntity.isVector(i)) {
      if (cfg.getBool("Table.showArrayLength")) {
        if (cfg.getBool("Table.combineNameAndType")) {
          if (cfg.getBool("Table.arrayNotation"))
            colWidthName = max(
              colWidthName,
              replaceStr(mSourceEntity.getPortName(i), "_", "\\_").length() +
                mSourceEntity.getVectorStr(i).length() + 2);
          else
            colWidthName = max(
              colWidthName,
              replaceStr(mSourceEntity.getPortName(i), "_", "\\_").length() +
                mSourceEntity.getVectorStartStr(i).length() +
                mSourceEntity.getVectorEndStr(i).length() + 3);
        } else if (cfg.getBool("Table.exportType")) {
          if (cfg.getBool("Table.arrayNotation"))
            colWidthType = max(
              colWidthType,
              replaceStr(mSourceEntity.getPortTypeStr(i), "_", "\\_").length() +
                mSourceEntity.getVectorStr(i).length() + 2);
          else
            colWidthType = max(
              colWidthType,
              replaceStr(mSourceEntity.getPortTypeStr(i), "_", "\\_").length() +
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
    formattingCorrection = std::string("\textbf{}").length();

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

  MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT) << "colWidthName = " << colWidthName;
  MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT) << "colWidthType = " << colWidthType;
  MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT)
    << "colWidthDirection = " << colWidthDirection;
  MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT)
    << "colWidthPolarity = " << colWidthPolarity;
  MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT)
    << "colWidthDescription = " << colWidthDescription;
  MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT) << "colWidthBlank = " << colWidthBlank;
  MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT)
    << "colWidthGenericName = " << colWidthGenericName;
  MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT)
    << "colWidthDefaultValue = " << colWidthDefaultValue;
  MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT)
    << "colWidthGenericType = " << colWidthGenericType;
}

// Write a text field to the output file
// Capable of inserting filling chars to create nice looking alignments
void
LaTeX::writeCell(colNames column,
                 std::string text,
                 std::string prefix,
                 std::string suffix,
                 std::string fillChar,
                 bool formatting)
{
  MSG(LOG_LEVEL::DEBUG, DEBUG::FUNCTIONCALL)
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

  replaceAll(text, "_", "\\_");
  mOutputFile << text;
  textLength += text.length();

  if (!formatting) {
    mOutputFile << suffix;
    textLength += suffix.length();

    if (textLength < maxTextLength) {
      MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT)
        << "if (textLength + suffix.length() < maxTextLength)";

      for (size_t i = 0; i < maxTextLength - textLength; i++) {
        MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT) << "fillChar";
        mOutputFile << fillChar;
      }
    }
  } else {
    if (textLength < maxTextLength) {
      MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT)
        << "if (textLength + suffix.length() < maxTextLength)";

      for (size_t i = 0; i < maxTextLength - textLength - suffix.length();
           i++) {
        MSG(LOG_LEVEL::DEBUG, DEBUG::RESULT) << "fillChar";
        mOutputFile << fillChar;
      }
    }

    mOutputFile << suffix;
    textLength += suffix.length();
  }
}
