/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wieﬂneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: FODG.cpp
*	Author: Benjamin Wieﬂneth
*	Description: Generate FODG files from parser data
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

#include "FODG.h"
#include "Config.h"
#include "FODGWriter.h"
#include "MSG.h"

FODG::FODG(VHDLEntity sourceEntity)
  : OFileHandler(sourceEntity, "FODG", "fodg", "")
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL)
    << "FODG::FODG(VHDLEntity sourceEntity)";
  MSG(GROUP::DEBUG, DEBUG::RESULT) << "mConfigName = " << mConfigName;
  MSG(GROUP::DEBUG, DEBUG::RESULT) << "mExtension = " << mExtension;
  MSG(GROUP::DEBUG, DEBUG::RESULT) << "mSuffix = " << mSuffix;

  initValues();
  translate();
}

void
FODG::initValues()
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL)
    << "FODG::initValues(VHDLEntity mSourceEntity)";
  // Initialize global variables
  groupHeight = 0;
  entityHeight = 0;
  genericWidth = 0;
  genericHeight = 0;
  originX = 0;
  originY = 0;
  clockPort = -1;
  resetPort = -1;
  vectorTextLenghtIn = 0;
  vectorTextLenghtOut = 0;

  // Load custom settings from config file
  fontSize = cfg.getDouble("port.fontSize");
  entity_fontSize = cfg.getDouble("entity.fontSize");
  generics_fontSize = cfg.getDouble("genericSignal.fontSize");
  clockName = cfg.getString("clockName");
  resetName = cfg.getString("resetName");
  fixedWidth = cfg.getDouble("fixedWidth");
  horizontal_Margin = cfg.getDouble("horizontal_Margin");
  vertical_Margin = cfg.getDouble("vertical_Margin");
  //	labelBox_visible = cfg.getBool("labelBox_visible");

  int noInputs = mSourceEntity.getNumberOfInputs();
  int noOutputs = mSourceEntity.getNumberOfOutputs();

  // Calculate entityHeight depending on total number of ports and the presence
  // of special ports No clock port and no reset port
  if ((mSourceEntity.getClockPort() == -1) &&
      (mSourceEntity.getResetPort() == -1)) {
    entityHeight = max(noInputs, noOutputs);
  }
  // One clock port and no reset port
  else if ((mSourceEntity.getClockPort() != -1) &&
           (mSourceEntity.getResetPort() == -1)) {
    if (noInputs - 1 >= noOutputs)
      entityHeight = noInputs;
    else
      entityHeight = noOutputs;
  }
  // No clock port and one reset port
  else if ((mSourceEntity.getClockPort() == -1) &&
           (mSourceEntity.getResetPort() != -1)) {
    if (noInputs - 1 >= noOutputs)
      entityHeight = noInputs;
    else
      entityHeight = noOutputs + 1;

    // Reset connector facing downward => +1
    groupHeight = 1.0;
  }
  // One clock port and one reset port
  else if ((mSourceEntity.getClockPort() != -1) &&
           (mSourceEntity.getResetPort() != -1)) {
    if ((noInputs - 2) >= noOutputs)
      entityHeight = noInputs;
    else
      entityHeight = noOutputs + 1;

    // Reset connector facing downward => +1
    groupHeight = 1.0;
  }

  // Add 1cm for entity caption (always present)
  entityHeight += 1;

  // includes label
  groupHeight = groupHeight + entityHeight + 1;

  // Calculate the portWidth or use a fixed width
  if (fixedWidth <= 0) {
    // Calculate the portWidth depending on the maximum length of the port names
    // if no config value is given Get longest portname
    int charCount = 0;
    for (int i = 0; i < mSourceEntity.getNumberOfInputs(); i++)
      charCount = max(charCount, (int)mSourceEntity.getPortName(i).length());
    for (int i = 0; i < mSourceEntity.getNumberOfOutputs(); i++)
      charCount = max(
        charCount,
        (int)mSourceEntity.getPortName(mSourceEntity.getNumberOfInputs() + i)
          .length());

    portWidth = calcTextWidth(charCount, fontSize);
    entityWidth =
      calcTextWidth(mSourceEntity.getEntityName().length(), entity_fontSize);

    // Set entityWidth and portWidth to required size
    if (2 * portWidth > entityWidth)
      // 2*portWidth is greater than the entity name
      entityWidth = 2 * portWidth;
    else
      // Width of entity Name is greater than 2*portWidth
      portWidth = 0.5 * entityWidth;
  } else {
    // Use the given port width property from config file
    entityWidth = fixedWidth;
    portWidth = entityWidth / 2;
  }

  // Calculate height and width of the generic box
  if (mSourceEntity.getNumberOfGenerics() > 0) {
    // Get longest generic name
    int charCount = 0;
    for (int i = 0; i < mSourceEntity.getNumberOfGenerics(); i++)
      charCount = max(charCount, (int)mSourceEntity.getGenericStr(i).length());

    // genericWidth = floor(charCount*(generics_fontSize*0.03527)*0.7);
    genericWidth = calcTextWidth(charCount, generics_fontSize);

    genericHeight = mSourceEntity.getNumberOfGenerics() * 0.5;
    // if (genericHeight <= 1.0)
    // genericHeight = ;
  }

  // Get the longest vector length string	on the left side
  for (int i = 0; i < mSourceEntity.getNumberOfInputs(); i++) {
    if (mSourceEntity.isVector(i))
      vectorTextLenghtIn =
        max(vectorTextLenghtIn,
            calcTextWidth(mSourceEntity.getVectorStr(i).length(), fontSize));
  }
  // If no vector length string is longer than the connector line itself it is
  // set to 1.0
  if (vectorTextLenghtIn < 1.0)
    vectorTextLenghtIn = 1;

  // Get the longest vector length string	on the right side
  for (int i = 0; i < mSourceEntity.getNumberOfOutputs(); i++) {
    if (mSourceEntity.isVector(mSourceEntity.getNumberOfInputs() + i))
      vectorTextLenghtOut =
        max(vectorTextLenghtOut,
            calcTextWidth(
              mSourceEntity.getVectorStr(mSourceEntity.getNumberOfInputs() + i)
                .length(),
              fontSize));
  }

  // If no vector length string is longer than the connector line itself, it is
  // set to 1.0
  if (vectorTextLenghtOut < 1.0)
    vectorTextLenghtOut = 1.0;

  // Calculate origins of the drawing. These coordinates are used for glue
  // points Half the group height
  originY = (groupHeight) / 2;

  // Two connectors on each side + rectangle width
  groupWidth = (2 + entityWidth);
  originX = groupWidth / 2;
}

void
FODG::translate()
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "FODG::translate()";

  // Relative coordinates for gluepoints
  // svg:y = (portY / (entityHeight/2) * resolution * direction) = (1.5cm / 4cm
  // = 0.375cm) * 5 * -1 = -1.875cm
  double relX = 0;
  double relY = 0;

  double currentX = 0;
  double currentY = 0;
  double strokeWidth = cfg.getDouble("entity.strokeWidth");

  stringstream ss;
  FODGWriter sWriter(mOutputFile);

  // Insert FODG header and custom styles
  if (genericWidth > 0) {
    // Entity has generics
    if (vectorTextLenghtOut > genericWidth) {
      // One of the vector length strings is greater than the width of the
      // generic box
      if (mSourceEntity.getResetPort() != -1)
        // Entity has a reset port
        sWriter.FODGHeader(entityWidth + 2 * horizontal_Margin +
                             vectorTextLenghtIn + vectorTextLenghtOut,
                           entityHeight + 1 + genericHeight +
                             2 * vertical_Margin);
      else
        // Entity has no reset port
        sWriter.FODGHeader(entityWidth + 2 * horizontal_Margin +
                             vectorTextLenghtIn + vectorTextLenghtOut,
                           entityHeight + genericHeight + 2 * vertical_Margin);
    } else {
      // Vector length strings are all shorter than than the width of the
      // generic box
      if (mSourceEntity.getResetPort() != -1) {
        // Entity has a reset port
        sWriter.FODGHeader(entityWidth + 2 * horizontal_Margin +
                             vectorTextLenghtIn + genericWidth,
                           entityHeight + 1 + genericHeight +
                             2 * vertical_Margin);
      } else {
        // Entity has no reset port
        sWriter.FODGHeader(entityWidth + 2 * horizontal_Margin +
                             vectorTextLenghtIn + genericWidth,
                           entityHeight + genericHeight + 2 * vertical_Margin);
      }
    }
  } else {
    // Entity has no generics
    if (mSourceEntity.getResetPort() != -1)
      // Entity has a reset port
      sWriter.FODGHeader(entityWidth + 2 * horizontal_Margin +
                           vectorTextLenghtIn + vectorTextLenghtOut,
                         entityHeight + 2 + 2 * vertical_Margin);
    else
      // Entity has no reset port
      sWriter.FODGHeader(entityWidth + 2 * horizontal_Margin +
                           vectorTextLenghtIn + vectorTextLenghtOut,
                         entityHeight + 1 + 2 * vertical_Margin);
  }

  // Set coordinate to the upper left corner of the label box
  currentX = horizontal_Margin + vectorTextLenghtIn;
  currentY = vertical_Margin;

  if (mSourceEntity.getNumberOfGenerics() > 0) {
    // If entity has generics

    // Open group for generics
    sWriter.group();

    // Add generic box
    sWriter.customShape(currentX + entityWidth,
                        currentY,
                        genericWidth,
                        genericHeight,
                        "",
                        "generics",
                        "generics");

    // Loop through all generics
    for (int i = 0; i < mSourceEntity.getNumberOfGenerics(); i++) {
      // Add shape for each generic
      sWriter.customShape(currentX + entityWidth,
                          currentY + i * 0.5,
                          genericWidth,
                          0.5,
                          mSourceEntity.getGenericStr(i),
                          "genericSignal",
                          "generics");
    }

    // Set y coordinate to top of the label box
    currentY = vertical_Margin + genericHeight - 1;

    // Close group for generics
    sWriter.group(1);
  }

  // Open group for entity
  sWriter.group();

  // Draw label box if visibility is set in config file or the label string
  // isn't empty
  if (mSourceEntity.getEntityLabel().length() != std::string::npos)
    sWriter.customShape(currentX,
                        currentY,
                        entityWidth,
                        1,
                        mSourceEntity.getEntityLabel(),
                        "label",
                        "tlabel");

  // Draw enitity box
  sWriter.customShape(currentX,
                      currentY + 1,
                      entityWidth,
                      entityHeight,
                      mSourceEntity.getEntityName(),
                      "entity",
                      "tentity");

  // Glue point for the generic box connector
  // relY = ((groupHeight)-originY-1.25)/groupHeight*10;
  // relX = (entityWidth*0.5/(entityWidth+2)*10);
  // TODO: ?
  sWriter.gluePoint(relX, -relY);

  // Draw input and output ports. Skip clock and reset ports
  int noPorts =
    mSourceEntity.getNumberOfInputs() + mSourceEntity.getNumberOfOutputs();
  int noInputs = 0;
  int noOutputs = 0;

  // Loop through all ports
  for (int i = 0; i < noPorts; i++) {
    // Ports which are placed on the left side
    if (mSourceEntity.getPortdirection(i) == EntityPort::IN) {
      // Find the clock/reset port and exclude them
      if (mSourceEntity.getClockPort() == i)
        clockPort = i;
      else if (mSourceEntity.getResetPort() == i)
        resetPort = i;
      else {
        // Increment current port counter
        noInputs++;

        // Offset from upper left corner: Y = (vertical_margin + labelHeight +
        // entityLabelHeight + ) Offset for connector line = 1.5
        sWriter.customShape(currentX,
                            currentY + noInputs + 1,
                            portWidth,
                            1,
                            mSourceEntity.getPortName(i),
                            "port",
                            "tlsport");
        sWriter.line(currentX - 1,
                     currentY + noInputs + 1.5,
                     currentX,
                     currentY + noInputs + 1.5,
                     "connector");
        if (mSourceEntity.getLOWActive(i) == true)
          sWriter.circle(
            currentX - 0.125, currentY + noInputs + 1.5, 0.25, "connector");

        // If current port ist a vector place slash onto the connector and
        // display vector length
        if (mSourceEntity.getVectorLength(i) >= 0) {
          sWriter.line(currentX - 0.75,
                       currentY + noInputs + 1.75,
                       currentX - 0.25,
                       currentY + noInputs + 1.25,
                       "connector");
          sWriter.customShape(currentX - 1,
                              currentY + noInputs + 0.6,
                              1,
                              1,
                              mSourceEntity.getVectorStr(i),
                              "vectorLength",
                              "vector_centered");
        }

        // Place glue point on calculated relative coordinates
        // labelHeight + entityLabelHeight + connectorOffset
        relY = ((noInputs + 1.5) - originY) / groupHeight * 10;
        relX = -5;

        sWriter.gluePoint(relX, relY);
      }
    }
    // Ports which are placed on the right side
    else if (mSourceEntity.getPortdirection(i) == EntityPort::OUT) {
      // Increment current port counter
      noOutputs++;

      // Offset from upper left corner: Y = (labelHeight + entityLabelHeight)
      // Offset for connector line = 1.5
      sWriter.customShape(currentX + portWidth,
                          currentY + noOutputs + 1,
                          portWidth,
                          1,
                          mSourceEntity.getPortName(i),
                          "port",
                          "trsport");
      sWriter.line(currentX + entityWidth,
                   currentY + noOutputs + 1.5,
                   currentX + entityWidth + 1,
                   currentY + noOutputs + 1.5,
                   "connector");

      // If current port ist a vector place slash onto the connector and display
      // vector length
      if (mSourceEntity.getVectorLength(i) >= 0) {
        sWriter.line(currentX + entityWidth + 0.25,
                     currentY + noOutputs + 1.75,
                     currentX + entityWidth + 0.75,
                     currentY + noOutputs + 1.25,
                     "connector");
        sWriter.customShape(currentX + entityWidth,
                            currentY + noOutputs + 0.6,
                            1,
                            1,
                            mSourceEntity.getVectorStr(i),
                            "vectorLength",
                            "vector_centered");
      }

      // Place glue point on calculated relative coordinates
      // labelHeight + entityLabelHeight + connectorOffset
      relY = ((noOutputs + 1.5) - originY) / groupHeight * 10;
      relX = 5;

      // pmesg(6,"GLUEPOINT absX = %f, absY = %f\n",0.0, noInputs+1.5);
      // pmesg(6,"          relX = %f, relY = %f\n",relX, relY);

      sWriter.gluePoint(relX, relY);
    }
  }

  // Entity has a clock and a reset port
  if ((mSourceEntity.getClockPort() != -1) &&
      (mSourceEntity.getResetPort() != -1)) {
    // reset port
    sWriter.customShape(currentX,
                        currentY + entityHeight,
                        2 * portWidth,
                        1,
                        mSourceEntity.getPortName(resetPort),
                        "port",
                        "tcport");
    sWriter.line(currentX + portWidth,
                 currentY + entityHeight + 1,
                 currentX + portWidth,
                 currentY + entityHeight + 2,
                 "connector");
    if (mSourceEntity.getLOWActive(resetPort) == true)
      sWriter.circle(currentX + portWidth,
                     currentY + entityHeight + 1 + 0.125 + strokeWidth,
                     0.25,
                     "connector");
    sWriter.gluePoint(0, 5);

    // clock port
    sWriter.customShape(currentX,
                        currentY + entityHeight - 1,
                        portWidth,
                        1,
                        mSourceEntity.getPortName(clockPort),
                        "port",
                        "tlsport");
    sWriter.line(currentX - 1,
                 currentY + entityHeight - 0.5,
                 currentX,
                 currentY + entityHeight - 0.5,
                 "connector");
    sWriter.line(currentX,
                 currentY + entityHeight - 0.35,
                 currentX + 0.2,
                 currentY + entityHeight - 0.5,
                 "connector");
    sWriter.line(currentX,
                 currentY + entityHeight - 0.65,
                 currentX + 0.2,
                 currentY + entityHeight - 0.5,
                 "connector");
    // if (mSourceEntity.getLOWActive(clockPort) == true)
    //	sWriter.circle(currentX+portWidth,currentY+entityHeight+1+0.125,0.25,"connector");

    // Place glue point on calculated relative coordinates
    relY = ((entityHeight - 0.5) - originY) / groupHeight * 10;
    relX = -5;
    sWriter.gluePoint(relX, relY);
  }
  // Entity has only a clock port
  else if ((mSourceEntity.getClockPort() != -1) &&
           (mSourceEntity.getResetPort() == -1)) {
    sWriter.customShape(currentX,
                        currentY + entityHeight,
                        portWidth,
                        1,
                        mSourceEntity.getPortName(clockPort),
                        "port",
                        "tlsport");
    // sWriter.customShape(1,entityHeight,portWidth,1,mSourceEntity.getPortName(clockPort),"port","tlsport");

    // clock connector
    sWriter.line(currentX - 1,
                 currentY + entityHeight + 0.5,
                 currentX,
                 currentY + entityHeight + 0.5,
                 "connector");
    sWriter.line(currentX,
                 currentY + entityHeight + 0.35,
                 currentX + 0.2,
                 currentY + entityHeight + 0.5,
                 "connector");
    sWriter.line(currentX,
                 currentY + entityHeight + 0.65,
                 currentX + 0.2,
                 currentY + entityHeight + 0.5,
                 "connector");

    // Place glue point on calculated relative coordinates
    relY = ((entityHeight + 0.5) - originY) / groupHeight * 10;
    relX = -5;
    sWriter.gluePoint(relX, relY);
  }
  // Entity has only a reset port
  else if ((mSourceEntity.getClockPort() == -1) &&
           (mSourceEntity.getResetPort() != -1)) {
    // reset port
    sWriter.customShape(currentX,
                        currentY + entityHeight,
                        2 * portWidth,
                        1,
                        mSourceEntity.getPortName(resetPort),
                        "port",
                        "tcport");
    sWriter.line(currentX + portWidth,
                 currentY + entityHeight + 1,
                 currentX + portWidth,
                 currentY + entityHeight + 2,
                 "connector");
    if (mSourceEntity.getLOWActive(resetPort) == true)
      sWriter.circle(currentX + portWidth,
                     currentY + entityHeight + 1 + 0.125,
                     0.25,
                     "connector");
    sWriter.gluePoint(0, 5);
  }

  // Close group
  sWriter.group(true);

  // Insert closing tags
  sWriter.FODGclose();
}

double
FODG::calcTextWidth(int charCount, double fontSize)
{
  double yOffset = 0.52;
  double lengthCoeff = 0.0232;
  double lengthOffset = 0.0069;
  double adjFactor = 0.95;

  if (charCount == 0)
    return 0;

  double width =
    ((lengthCoeff * charCount + lengthOffset) * fontSize + yOffset) * adjFactor;
  width = floor(width) + 0.5;

  if (width < 1.0)
    return 1.0;
  else
    return width;
}