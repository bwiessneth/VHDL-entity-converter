/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wießneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: SVGWriter.cpp
*	Author: Benjamin Wießneth
*	Description: Create SVG elements
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

#include "SVGWriter.h"
#include "Config.h"
#include "MSG.h"
#include "tools.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <stdlib.h>

SVGWriter::SVGWriter(std::ostream& os)
  : os(os)
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "SVGWriter::SVGWriter(ostream& os)";
}

SVGWriter::~SVGWriter()
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "SVGWriter::~SVGWriter()";
}

void
SVGWriter::group(bool close)
{
  if (close)
    os << "</g>" << std::endl;
  else
    os << "<g>" << std::endl;

  return;
}

void
SVGWriter::line(double x1,
                double y1,
                double x2,
                double y2,
                std::string CSSclass)
{
  // <line class="connector" x1="0cm" y1="0cm" x2="20cm" y2="5cm" />

  os << "<line class=\"" << CSSclass << "\" x1=\"" << x1 << "cm\" y1=\"" << y1
     << "cm\" x2=\"" << x2 << "cm\" y2=\"" << y2 << "cm\" />" << std::endl;

  return;
}

void
SVGWriter::rect(double x,
                double y,
                double width,
                double height,
                std::string CSSclass)
{
  // <rect class="entityBox" x="1cm" y="2cm" width="5cm" height="1cm" />

  os << "<rect class=\"" << CSSclass << "\" x=\"" << x << "cm\" y=\"" << y
     << "cm\" width=\"" << width << "cm\" height=\"" << height << "cm\" />"
     << std::endl;

  return;
}

void
SVGWriter::circle(double x, double y, double radius, std::string CSSclass)
{
  // <circle class="connector" cx="50" cy="50" r="40" />

  os << "<circle class=\"" << CSSclass << "\" cx=\"" << x << "cm\" cy=\"" << y
     << "cm\" r=\"" << radius << "cm\" />" << std::endl;

  return;
}

void
SVGWriter::text(double x, double y, std::string text, std::string CSSclass)
{
  // <text class="label center" x="0" y="15">cntdnmodm_rtl</text>

  os << "<text class=\"" << CSSclass << "\" x=\"" << x << "cm\" y=\"" << y
     << "cm\" >" << text << "</text>" << std::endl;

  return;
}

void
SVGWriter::SVGHeader(double width, double height)
{
  os << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
        "xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\""
     << width << "cm\" height=\"" << height << "cm\">" << std::endl;

  SVGStyles();

  return;
}

void
SVGWriter::SVGFooter()
{
  os << "</svg>" << std::endl;

  return;
}

void
SVGWriter::SVGStyles()
{
  os << "<style type=\"text/css\" >" << std::endl << "\t<![CDATA[" << std::endl;

  // Text styles
  os << "\t/* Text styles */" << std::endl;

  // Label
  os << "\t.label {" << std::endl
     << "\t\tfont-family: " << cfg.getString("label.fontFamily") << ";"
     << std::endl
     << "\t\tfont-size: " << cfg.getString("label.fontSize") << ";" << std::endl
     << "\t\tfont-weight: " << cfg.getString("label.fontWeight") << ";"
     << std::endl
     << "\t}" << std::endl
     << std::endl;

  // Entity
  os << "\t.entity {" << std::endl
     << "\t\tfont-family: " << cfg.getString("entity.fontFamily") << ";"
     << std::endl
     << "\t\tfont-size: " << cfg.getString("entity.fontSize") << ";"
     << std::endl
     << "\t\tfont-weight: " << cfg.getString("entity.fontWeight") << ";"
     << std::endl
     << "\t}" << std::endl
     << std::endl;

  // Port name
  os << "\t.port {" << std::endl
     << "\t\tfont-family: " << cfg.getString("port.fontFamily") << ";"
     << std::endl
     << "\t\tfont-size: " << cfg.getString("port.fontSize") << ";" << std::endl
     << "\t\tfont-weight: " << cfg.getString("port.fontWeight") << ";"
     << std::endl
     << "\t}" << std::endl
     << std::endl;

  // Vectors
  os << "\t.vector {" << std::endl
     << "\t\tfont-family: " << cfg.getString("vector.fontFamily") << ";"
     << std::endl
     << "\t\tfont-size: " << cfg.getString("vector.fontSize") << ";"
     << std::endl
     << "\t\tfont-weight: " << cfg.getString("vector.fontWeight") << ";"
     << std::endl
     << "\t}" << std::endl
     << std::endl;

  // Generics
  os << "\t.generics {" << std::endl
     << "\t\tfont-family: " << cfg.getString("genericSignal.fontFamily") << ";"
     << std::endl
     << "\t\tfont-size: " << cfg.getString("genericSignal.fontSize") << ";"
     << std::endl
     << "\t\tfont-weight: " << cfg.getString("genericSignal.fontWeight") << ";"
     << std::endl
     << "\t}" << std::endl
     << std::endl;

  // Text alignment
  os << "\t/* Text alignment */" << std::endl;

  // Output port (right side)
  os << "\t.right {" << std::endl
     << "\t\tdominant-baseline: central;" << std::endl
     << "\t\ttext-anchor: end;" << std::endl
     << "\t}" << std::endl
     << std::endl;

  // Input port (left side)
  os << "\t.left {" << std::endl
     << "\t\tdominant-baseline: central;" << std::endl
     << "\t\ttext-anchor: start;" << std::endl
     << "\t}" << std::endl
     << std::endl;

  // Center port
  os << "\t.center {" << std::endl
     << "\t\tdominant-baseline: central;" << std::endl
     << "\t\ttext-anchor: middle;" << std::endl
     << "\t}" << std::endl
     << std::endl;

  // Shape styles
  os << "\t/* Shape styles */" << std::endl;

  // Entity box
  os << "\t.entityBox {" << std::endl
     << "\t\tstroke-width: " << cfg.getString("entity.strokeWidth") << ";"
     << std::endl
     //	<< "\t\tstroke: " << cfg.getString("entity.stroke") << ";" << std::endl
     << "\t\tstroke: " << cfg.getString("entity.strokeColor") << ";"
     << std::endl
     //	<< "\t\tfill: " << cfg.getString("entity.fill") << ";" << std::endl
     << "\t\tfill: " << cfg.getString("entity.fillColor") << ";" << std::endl
     << "\t}" << std::endl
     << std::endl;

  // Port box
  os << "\t.portBox {" << std::endl
     << "\t\tstroke-width: " << cfg.getString("port.strokeWidth") << ";"
     << std::endl
     //	<< "\t\tstroke: " << cfg.getString("port.stroke") << ";" << std::endl
     << "\t\tstroke: " << cfg.getString("port.strokeColor") << ";"
     << std::endl
     //	<< "\t\tfill: " << cfg.getString("port.fill") << ";" << std::endl
     << "\t\tfill: " << cfg.getString("port.fillColor") << ";" << std::endl
     << "\t}" << std::endl
     << std::endl;

  // Label box
  os << "\t.labelBox {" << std::endl
     << "\t\tstroke-width: " << cfg.getString("label.strokeWidth") << ";"
     << std::endl
     //	<< "\t\tstroke: " << cfg.getString("label.stroke") << ";" << std::endl
     << "\t\tstroke: " << cfg.getString("label.strokeColor") << ";"
     << std::endl
     //	<< "\t\tfill: " << cfg.getString("label.fill") << ";" << std::endl
     << "\t\tfill: " << cfg.getString("label.fillColor") << ";" << std::endl
     << "\t}" << std::endl
     << std::endl;

  // VectorLength box
  os << "\t.vectorLength {" << std::endl
     << "\t\tstroke-width: " << cfg.getString("label.strokeWidth") << ";"
     << std::endl
     //	<< "\t\tstroke: " << cfg.getString("label.stroke") << ";" << std::endl
     //	<< "\t\tstroke: " << cfg.getString("label.strokeColor") << ";" <<
     // std::endl
     //	<< "\t\tfill: " << cfg.getString("label.fill") << ";" << std::endl
     << "\t\tfill: " << cfg.getString("label.fillColor") << ";" << std::endl
     << "\t}" << std::endl
     << std::endl;

  // Generic box
  os << "\t.genericsBox {" << std::endl
     << "\t\tstroke-width: " << cfg.getString("generics.strokeWidth") << ";"
     << std::endl
     //	<< "\t\tstroke: " << cfg.getString("generics.stroke") << ";" <<
     // std::endl
     << "\t\tstroke: " << cfg.getString("generics.strokeColor") << ";"
     << std::endl
     //	<< "\t\tfill: " << cfg.getString("generics.fill") << ";" << std::endl
     << "\t\tfill: " << cfg.getString("generics.fillColor") << ";" << std::endl
     << "\t}" << std::endl
     << std::endl;

  // Connector
  os << "\t.connector {" << std::endl
     << "\t\tstroke-width: " << cfg.getString("connector.strokeWidth") << ";"
     << std::endl
     //	<< "\t\tstroke: " << cfg.getString("connector.stroke") << ";" <<
     // std::endl
     << "\t\tstroke: " << cfg.getString("connector.strokeColor") << ";"
     << std::endl
     //	<< "\t\tfill: " << cfg.getString("connector.fill") << ";" << std::endl
     << "\t\tfill: " << cfg.getString("connector.fillColor") << ";" << std::endl
     << "\t}" << std::endl
     << std::endl;

  os << "\t]]>" << std::endl << "</style>" << std::endl;

  return;
}