/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wieﬂneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: OutputGenerator.cpp
*	Author: Benjamin Wieﬂneth
*	Description: The OutputGenerator class contains all functions to create
*	output from a source file.
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

#include "OutputGenerator.h"
#include "Config.h"
#include "DWMarkup.h"
#include "FODG.h"
#include "LaTeX.h"
#include "MSG.h"
#include "Markdown.h"
#include "SVG.h"

OutputGenerator::OutputGenerator(std::string sourcePath,
                                 VHDLEntity sourceEntity)
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL)
    << "OutputGenerator::OutputGenerator(std::string sourcePath, VHDLEntity "
       "sourceEntity)";

  mSourcePath = sourcePath;
  mSourceEntity = sourceEntity;
  mSourceName = mSourceEntity.getEntityName();

  // Check if source entity has a valid name and path
  if (mSourceName.empty() || mSourcePath.empty()) {
    MSG(GROUP::ERROR) << "Empty source name or source path.";
  } else {
    // Start to generate the OpenDocument file. This is needed for some
    // subsequent steps
    if ((cfg.getBool("FODG.enableExport")) || (cfg.getBool("PNG.enableExport")))
      generateFODG();

    // Create several other output formats if requested
    if (cfg.getBool("SVG.enableExport"))
      generateSVG();
    if (cfg.getBool("PNG.enableExport"))
      generatePNG();
    if (cfg.getBool("DokuWiki.enableExport"))
      generateDokuWikiMarkup();
    if (cfg.getBool("Markdown.enableExport"))
      generateMarkdown();
    if (cfg.getBool("LaTeX.enableExport"))
      generateLaTeX();
  }
}

// Generate a OpenDocument file
void
OutputGenerator::generateFODG()
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "OutputGenerator::generateFODG()";

  // Pass the source entity to the FODGTranslator
  FODG mFODG(mSourceEntity);
  return;
}

// Generate a SVG file
void
OutputGenerator::generateSVG()
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "OutputGenerator::generateFODG()";

  SVG mSVG(mSourceEntity);
  return;
}

// Generate a PNG file by utilizing the LibreOffice conversion tool
void
OutputGenerator::generatePNG()
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "OutputGenerator::generatePNG()";
  MSG(GROUP::INFO) << "Generating PNG file";

  // Initialize common vars
  std::string arg = "\"\"";
  std::string SOFFICE_path = cfg.getString(
    "PATH.soffice"); // Get path to the StarOffice / Libre Office installation
  std::string outputPath = cfg.getString(
    "PNG.outputPath"); // If the user didn't specify a output path via command
                       // line arg read the default from config file

  // Get path to put output files in
  std::string fPath = getOutputPath(mSourcePath);

#ifdef _WIN32
  if (!fPath.empty())
    fPath = cleanPath(fPath) + "\\" +
            cleanPath(cfg.getString("FODG.outputPath")) + "\\" + mSourceName;
  else
    fPath = cleanPath(cfg.getString("FODG.outputPath")) + "\\" + mSourceName;
#else
  if (!fPath.empty())
    fPath = cleanPath(fPath) + "/" +
            cleanPath(cfg.getString("FODG.outputPath")) + "/" + mSourceName;
  else
    fPath = cleanPath(cfg.getString("FODG.outputPath")) + "/" + mSourceName;
#endif

  MSG(GROUP::DEBUG, DEBUG::RESULT) << "PNG.outputPath = " << outputPath;

  // Check if a SO-Installation was specified
  if (SOFFICE_path.empty()) {
    MSG(GROUP::WARNING)
      << "PATH.soffice not specified. Please edit your VEC.conf";
  } else {
#ifdef _WIN32
    // Build the arg string to pass
    arg += SOFFICE_path +
           "\" --headless --convert-to png:\"draw_png_Export\" \"" + fPath +
           ".fodg\" --outdir \"";
#else
    // Build the arg string to pass
    arg = "\'";
    arg += SOFFICE_path +
           "\' --headless  --convert-to png:\"draw_png_Export\" \'" + fPath +
           ".fodg\' --outdir \'";
#endif

    // If no output path was set via cmd line argument or config file use
    // directory of the executable
    if (outputPath.empty())
      arg += cleanPath(cfg.getString("FODG.outputPath"));
    else
      arg += cleanPath(outputPath);

#ifdef _WIN32
    // End arg string with quotes
    arg += "\"\"";
#else
    // End arg string with hard quotes
    arg += "\'";
#endif

    MSG(GROUP::INFO) << TAB << "SOFFICE call = " << arg;

    // Execute arg string
    system(arg.c_str());
  }
  return;
}

// Generate a DokuWiki table source file
void
OutputGenerator::generateDokuWikiMarkup()
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL)
    << "OutputGenerator::generateDokuWikiMarkup()";
  MSG(GROUP::INFO) << "Generating DokuWiki markup";

  DWMarkup mDWMarkup(mSourceEntity);
  return;
}

// Generate a Markdown source file
void
OutputGenerator::generateMarkdown()
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL)
    << "OutputGenerator::generateMarkdown()";
  MSG(GROUP::INFO) << "Generating markdown";

  Markdown mMarkdown(mSourceEntity);
  return;
}

// Generate a LaTeX source file
void
OutputGenerator::generateLaTeX()
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "OutputGenerator::generateLaTeX()";
  MSG(GROUP::INFO) << "Generating LaTeX";

  LaTeX mLaTeX(mSourceEntity);
  return;
}

// Exclude filename (e.g. microdata.vhd) from filepath to get output directory
// (e.g. input = C:\VEC\microdata.vhd -> output = C:\VEC\)
std::string
OutputGenerator::getOutputPath(std::string filePath)
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL)
    << "OutputGenerator::getOutputPath(std::string filePath)";

  int lastSlash = filePath.find_last_of('\\');
  if (lastSlash == -1)
    return "";
  else
    return filePath.substr(0, lastSlash + 1);
}