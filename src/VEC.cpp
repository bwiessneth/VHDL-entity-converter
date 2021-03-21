/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wieﬂneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: VEC.cpp
*	Author: Benjamin Wieﬂneth
*	Description: VEC - VHDL to OpenDocument Generator
*	Create block diagramms from vhdl entities
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

#include "VEC.h"
#include "Config.h"
#include "MSG.h"
#include "OutputGenerator.h"
#include "VHDLParser.h"
#include "tools.h"

#include <iostream>
#include <vector>

int
main(int argc, const char* argv[])
{
  // Create vector of strings to hold input filenames
  std::vector<std::string> inputFiles;

  // If no input file was specified print VEC info dialog
  if (argc <= 1) {
    printInfo();

    // Exit
    return 0;
  }
  // Parse programm options for custom switches and input filenames
  else if (parseArgv(argc, argv, inputFiles) != 0) {
    // Error while parsing programm options, or help option was set
    if (cfg.getBool("VEC.printHelp")) {
      printHelp();
    }

    // Print no more error as this already happened in parseArgv(). Exit
    return 1;
  }
  // Valid programm options and at least one source file
  else {
    MSG(LOG_LEVEL::INFO) << "Verbose mode";

    for (unsigned int i = 0; i < inputFiles.size(); i++) {
      // Check if input argument has a vhdl extension
      if (validFileExtension(inputFiles[i]) == true) {
        std::cout << "Parsing " << inputFiles[i] << std::endl;

        // Create parser object and parse the given source file
        VHDLParser myParser(inputFiles[i]);

        // Set the entity label. It's either provided via command line or
        // defined in VEC.conf
        myParser.setEntityLabel(cfg.getString("default_label"));

        // Check if parsed entity has ports
        if ((myParser.getEntity().getNumberOfInputs() == 0) &&
            (myParser.getEntity().getNumberOfOutputs() == 0)) {
          MSG(LOG_LEVEL::ERROR) << "Parsed VHDL entity has no valid ports.";
        } else {
          // Print short overview of parsed vhdl entity when verbose mode is
          // active
          myParser.printResults();

          std::cout << "Generating output files" << std::endl;

          // Create a OutputGenerator object which takes care of creating the
          // output files
          OutputGenerator mOutputGenerator(inputFiles[i], myParser.getEntity());
        }
      } else {
        // Print message if input file is no valid vhdl file
        MSG(LOG_LEVEL::ERROR) << inputFiles[i]
                              << " has no valid vhd extension. Supported "
                                 "extensions are .vhd and .vhdl";
      }
    }
  }

  std::cout << "Done" << std::endl;
  return 0;
}
