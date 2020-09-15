/***************************************************************************\
*   Copyright(C) 2014 University of Applied Sciences Augsburg
*                     Benjamin Wießneth, Johann Faerber
*   Email: benjamin.wiessneth@hs-augsburg.de
*   Email: johann.faerber@hs-augsburg.de
*
*****************************************************************************
*
*	File: FODGWriter.cpp
*	Author: Benjamin Wießneth
*	Description: Create FODG file contents
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

#include "FODGWriter.h"
#include "Config.h"
#include "MSG.h"
#include "tools.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <stdlib.h>

FODGWriter::FODGWriter(std::ostream& os)
  : os(os)
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL)
    << "FODGWriter::FODGWriter(ostream& os)";

  gluePointID = 4;
}

FODGWriter::~FODGWriter()
{
  MSG(GROUP::DEBUG, DEBUG::FUNCTIONCALL) << "FODGWriter::~FODGWriter()";
  // pmesg(10,"~FODGWriter()\n");
}

void
FODGWriter::line(double x1,
                 double y1,
                 double x2,
                 double y2,
                 std::string shapeID)
{
  os << "\t<draw:line draw:style-name = \"" << shapeID
     << "\" draw:layer = \"layout\" svg:x1 = \"" << x1 << "cm\" svg:y1 = \""
     << y1 << "cm\" svg:x2 = \"" << x2 << "cm\" svg:y2 = \"" << y2 << "cm\">"
     << std::endl
     << "\t\t<text:p/>" << std::endl
     << "\t</draw:line>" << std::endl;

  return;
}

void
FODGWriter::text(int x, int y, char* text)
{
  os << "<draw:frame draw:style-name=\"gr6\" draw:text-style-name=\"P6\" "
        "draw:layer=\"layout\" svg:x=\""
     << x << "cm\" svg:y=\"" << y << "cm\">"
     << "<draw:text-box>"
     << "<text:p text:style-name=\"P6\">" << text << "</text:p>"
     << "</draw:text-box>"
     << "</draw:frame>";
  return;
}

void
FODGWriter::group(bool close)
{
  // pmesg(10,"&FODGWriter::group(bool close)\n");
  if (close)
    os << "</draw:g>" << std::endl;
  else
    os << "<draw:g>" << std::endl;

  return;
}

void
FODGWriter::gluePoint(double x, double y)
{
  // pmesg(10,"&FODGWriter::gluePoint(double x, double y)\n");

  os << "\t<draw:glue-point draw:id = \"" << gluePointID << "\" svg:x = \"" << x
     << "cm\" svg:y = \"" << y << "cm\" />" << std::endl;
  gluePointID++;

  return;
}

void
FODGWriter::customShape(double x,
                        double y,
                        double width,
                        double height,
                        std::string label,
                        std::string shapeID,
                        std::string textID)
{
  os << "\t<draw:custom-shape draw:style-name = \"" << shapeID
     << "\" draw:text-style-name = \"" << textID
     << "\" draw:layer = \"layout\" svg:width = \"" << width
     << "cm\"  svg:height = \"" << height << "cm\" svg:x = \"" << x
     << "cm\" svg:y = \"" << y << "cm\">" << std::endl;

  os << "\t\t<text:p text:style-name = \"" << textID
     << "\"><text:span text:style-name = \"" << textID << "\">" << label
     << "</text:span></text:p>" << std::endl;

  os << "\t\t<draw:enhanced-geometry svg:viewBox = \"0 0 500 500\" "
        "draw:mirror-horizontal = \"false\" draw:mirror-vertical = \"false\" "
        "draw:type = \"rectangle\" draw:enhanced-path = \"M 0 0 L 500 0 500 "
        "500 0 500 0 0 Z N\"/>"
     << std::endl;

  os << "\t</draw:custom-shape>" << std::endl;

  return;
}

void
FODGWriter::FODGHeader(double width, double height)
{
  os
    << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl
    << "<office:document "
       "xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\" "
       "xmlns:style=\"urn:oasis:names:tc:opendocument:xmlns:style:1.0\" "
       "xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\" "
       "xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\" "
       "xmlns:draw=\"urn:oasis:names:tc:opendocument:xmlns:drawing:1.0\" "
       "xmlns:fo=\"urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1."
       "0\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
       "xmlns:dc=\"http://purl.org/dc/elements/1.1/\" "
       "xmlns:meta=\"urn:oasis:names:tc:opendocument:xmlns:meta:1.0\" "
       "xmlns:number=\"urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0\" "
       "xmlns:presentation=\"urn:oasis:names:tc:opendocument:xmlns:"
       "presentation:1.0\" "
       "xmlns:svg=\"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0\" "
       "xmlns:chart=\"urn:oasis:names:tc:opendocument:xmlns:chart:1.0\" "
       "xmlns:dr3d=\"urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0\" "
       "xmlns:math=\"http://www.w3.org/1998/Math/MathML\" "
       "xmlns:form=\"urn:oasis:names:tc:opendocument:xmlns:form:1.0\" "
       "xmlns:script=\"urn:oasis:names:tc:opendocument:xmlns:script:1.0\" "
       "xmlns:config=\"urn:oasis:names:tc:opendocument:xmlns:config:1.0\" "
       "xmlns:ooo=\"http://openoffice.org/2004/office\" "
       "xmlns:ooow=\"http://openoffice.org/2004/writer\" "
       "xmlns:oooc=\"http://openoffice.org/2004/calc\" "
       "xmlns:dom=\"http://www.w3.org/2001/xml-events\" "
       "xmlns:xforms=\"http://www.w3.org/2002/xforms\" "
       "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "
       "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
       "xmlns:smil=\"urn:oasis:names:tc:opendocument:xmlns:smil-compatible:1."
       "0\" xmlns:anim=\"urn:oasis:names:tc:opendocument:xmlns:animation:1.0\" "
       "xmlns:rpt=\"http://openoffice.org/2005/report\" "
       "xmlns:of=\"urn:oasis:names:tc:opendocument:xmlns:of:1.2\" "
       "xmlns:xhtml=\"http://www.w3.org/1999/xhtml\" "
       "xmlns:grddl=\"http://www.w3.org/2003/g/data-view#\" "
       "xmlns:officeooo=\"http://openoffice.org/2009/office\" "
       "xmlns:tableooo=\"http://openoffice.org/2009/table\" "
       "xmlns:drawooo=\"http://openoffice.org/2010/draw\" "
       "xmlns:calcext=\"urn:org:documentfoundation:names:experimental:calc:"
       "xmlns:calcext:1.0\" "
       "xmlns:field=\"urn:openoffice:names:experimental:ooo-ms-interop:xmlns:"
       "field:1.0\" "
       "xmlns:formx=\"urn:openoffice:names:experimental:ooxml-odf-interop:"
       "xmlns:form:1.0\" xmlns:css3t=\"http://www.w3.org/TR/css3-text/\" "
       "office:version=\"1.2\" "
       "office:mimetype=\"application/vnd.oasis.opendocument.graphics\">"
    << std::endl
    << "<office:styles>" << std::endl
    << "<draw:marker draw:name=\"Arrow\" svg:viewBox=\"0 0 20 30\" svg:d=\"m10 "
       "0-10 30h20z\"/>"
    << std::endl
    << "<style:style style:name=\"standard\" style:family=\"graphic\">"
    << std::endl
    << "<style:graphic-properties draw:stroke=\"solid\" "
       "svg:stroke-width=\"0cm\" svg:stroke-color=\"#808080\" "
       "draw:marker-start-width=\"0.2cm\" draw:marker-start-center=\"false\" "
       "draw:marker-end-width=\"0.2cm\" draw:marker-end-center=\"false\" "
       "draw:fill=\"solid\" draw:fill-color=\"#cfe7f5\" "
       "draw:textarea-horizontal-align=\"justify\" fo:padding-top=\"0.125cm\" "
       "fo:padding-bottom=\"0.125cm\" fo:padding-left=\"0.25cm\" "
       "fo:padding-right=\"0.25cm\" draw:shadow=\"hidden\" "
       "draw:shadow-offset-x=\"0.2cm\" draw:shadow-offset-y=\"0.2cm\" "
       "draw:shadow-color=\"#808080\">"
    << std::endl
    << "</style:graphic-properties>" << std::endl
    << "<style:paragraph-properties fo:margin-left=\"0cm\" "
       "fo:margin-right=\"0cm\" fo:margin-top=\"0cm\" fo:margin-bottom=\"0cm\" "
       "fo:line-height=\"100%\" fo:text-indent=\"0cm\"/>"
    << std::endl
    << "<style:text-properties style:use-window-font-color=\"true\" "
       "style:text-outline=\"false\" style:text-line-through-style=\"none\" "
       "fo:font-family=\"Arial\" style:font-family-generic=\"roman\" "
       "style:font-pitch=\"variable\" fo:font-size=\"18pt\" "
       "fo:font-style=\"normal\" fo:text-shadow=\"none\" "
       "style:text-underline-style=\"none\" fo:font-weight=\"normal\" "
       "style:letter-kerning=\"true\" "
       "style:font-family-asian=\"&apos;Microsoft YaHei&apos;\" "
       "style:font-family-generic-asian=\"system\" "
       "style:font-pitch-asian=\"variable\" style:font-size-asian=\"18pt\" "
       "style:font-style-asian=\"normal\" style:font-weight-asian=\"normal\" "
       "style:font-family-complex=\"Mangal\" "
       "style:font-family-generic-complex=\"system\" "
       "style:font-pitch-complex=\"variable\" style:font-size-complex=\"18pt\" "
       "style:font-style-complex=\"normal\" "
       "style:font-weight-complex=\"normal\" style:text-emphasize=\"none\" "
       "style:font-relief=\"none\" style:text-overline-style=\"none\" "
       "style:text-overline-color=\"font-color\"/>"
    << std::endl
    << "</style:style>" << std::endl
    << "</office:styles>" << std::endl;

  FODGPage(width, height);
  FODGStyles();

  os << "<office:master-styles>" << std::endl
     << "<draw:layer-set>" << std::endl
     << "<draw:layer draw:name=\"layout\"/>" << std::endl
     << "<draw:layer draw:name=\"background\"/>" << std::endl
     << "<draw:layer draw:name=\"backgroundobjects\"/>" << std::endl
     << "<draw:layer draw:name=\"controls\"/>" << std::endl
     << "<draw:layer draw:name=\"measurelines\"/>" << std::endl
     << "</draw:layer-set>" << std::endl
     << "<style:master-page style:name=\"Standard\" "
        "style:page-layout-name=\"PM0\" draw:style-name=\"dp1\"/>"
     << std::endl
     << "</office:master-styles>" << std::endl
     << "<office:body>" << std::endl
     << "<office:drawing>" << std::endl
     << "<draw:page draw:name=\"page1\" draw:style-name=\"dp1\" "
        "draw:master-page-name=\"Standard\">"
     << std::endl;

  return;
}

void
FODGWriter::FODGclose()
{
  os << "</draw:page>" << std::endl
     << "</office:drawing>" << std::endl
     << "</office:body>" << std::endl
     << "</office:document>" << std::endl;

  return;
}

void
FODGWriter::FODGStyles()
{
  // Shape styles
  // Bounding box
  os
    << "<!-- Shape formats -->" << std::endl
    << "<!-- Bounding box -->" << std::endl
    << "<style:style style:name=\"entity\" style:family=\"graphic\" "
       "style:parent-style-name=\"standard\">"
    << std::endl
    << "<style:graphic-properties svg:stroke-width=\""
    << cfg.getString("entity.strokeWidth") << "\" draw:stroke=\""
    << cfg.getString("entity.stroke") << "\" svg:stroke-color=\""
    << cfg.getString("entity.strokeColor") << "\" draw:fill=\""
    << cfg.getString("entity.fill") << "\" draw:fill-color=\""
    << cfg.getString("entity.fillColor")
    << "\" draw:textarea-horizontal-align=\"justify\" "
       "draw:textarea-vertical-align=\"top\" draw:auto-grow-height=\"false\" />"
    << std::endl
    << "</style:style>" << std::endl;

  // Port box
  os << "<!-- port box -->" << std::endl
     << "<style:style style:name=\"port\" style:family=\"graphic\" "
        "style:parent-style-name=\"standard\">"
     << std::endl
     << "<style:graphic-properties svg:stroke-width=\""
     << cfg.getString("port.strokeWidth") << "\" draw:stroke=\""
     << cfg.getString("port.stroke") << "\" svg:stroke-color=\""
     << cfg.getString("port.strokeColor") << "\" draw:fill=\""
     << cfg.getString("port.fill") << "\" draw:fill-color=\""
     << cfg.getString("port.fillColor")
     << "\" draw:textarea-horizontal-align=\"justify\" "
        "draw:textarea-vertical-align=\"middle\" "
        "draw:auto-grow-height=\"false\" />"
     << std::endl
     << "</style:style>" << std::endl;

  // Connector
  os << "<!-- Connector -->" << std::endl
     << "<style:style style:name=\"connector\" style:family=\"graphic\" "
        "style:parent-style-name=\"standard\">"
     << std::endl
     << "<style:graphic-properties svg:stroke-width=\""
     << cfg.getString("connector.strokeWidth") << "\" draw:stroke=\""
     << cfg.getString("connector.stroke") << "\" svg:stroke-color=\""
     << cfg.getString("connector.strokeColor") << "\" draw:fill=\""
     << cfg.getString("connector.fill") << "\" draw:fill-color=\""
     << cfg.getString("connector.fillColor")
     << "\" draw:textarea-horizontal-align=\"justify\" "
        "draw:textarea-vertical-align=\"middle\" "
        "draw:auto-grow-height=\"false\" />"
     << std::endl
     << "</style:style>" << std::endl;

  // Label box
  os << "<!-- Label box -->" << std::endl
     << "<style:style style:name=\"label\" style:family=\"graphic\" "
        "style:parent-style-name=\"standard\">"
     << std::endl
     << "<style:graphic-properties svg:stroke-width=\""
     << cfg.getString("label.strokeWidth") << "\" draw:stroke=\""
     << cfg.getString("label.stroke") << "\" svg:stroke-color=\""
     << cfg.getString("label.strokeColor") << "\" draw:fill=\""
     << cfg.getString("label.fill") << "\" draw:fill-color=\""
     << cfg.getString("label.fillColor")
     << "\" draw:textarea-horizontal-align=\"justify\" "
        "draw:textarea-vertical-align=\"middle\" "
        "draw:auto-grow-height=\"false\" />"
     << std::endl
     << "</style:style>" << std::endl;

  // VectorLength box
  os << "<!-- VectorLength box -->" << std::endl
     << "<style:style style:name=\"vectorLength\" style:family=\"graphic\" "
        "style:parent-style-name=\"standard\">"
     << std::endl
     << "<style:graphic-properties svg:stroke-width=\""
     << cfg.getString("label.strokeWidth")
     << "\" draw:stroke=\"none\" draw:fill=\"none\" "
        "draw:textarea-horizontal-align=\"justify\" "
        "draw:textarea-vertical-align=\"middle\" "
        "draw:auto-grow-height=\"false\" />"
     << std::endl
     << "</style:style>" << std::endl;

  // Generic box
  os << "<!-- Generic box -->" << std::endl
     << "<style:style style:name=\"generics\" style:family=\"graphic\" "
        "style:parent-style-name=\"standard\">"
     << std::endl
     << "<style:graphic-properties svg:stroke-width=\""
     << cfg.getString("generics.strokeWidth") << "\" draw:stroke=\""
     << cfg.getString("generics.stroke") << "\" svg:stroke-color=\""
     << cfg.getString("generics.strokeColor") << "\" draw:fill=\""
     << cfg.getString("generics.fill") << "\" draw:fill-color=\""
     << cfg.getString("generics.fillColor")
     << "\" draw:textarea-horizontal-align=\"justify\" "
        "draw:textarea-vertical-align=\"start\" "
        "draw:auto-grow-height=\"false\" />"
     << std::endl
     << "</style:style>" << std::endl;

  // Generic signal
  os << "<!-- Generic box -->" << std::endl
     << "<style:style style:name=\"genericSignal\" style:family=\"graphic\" "
        "style:parent-style-name=\"standard\">"
     << std::endl
     << "<style:graphic-properties svg:stroke-width=\""
     << cfg.getString("genericSignal.strokeWidth") << "\" draw:stroke=\""
     << cfg.getString("genericSignal.stroke") << "\" svg:stroke-color=\""
     << cfg.getString("genericSignal.strokeColor") << "\" draw:fill=\""
     << cfg.getString("genericSignal.fill") << "\" draw:fill-color=\""
     << cfg.getString("genericSignal.fillColor")
     << "\" draw:textarea-horizontal-align=\"justify\" "
        "draw:textarea-vertical-align=\"middle\" "
        "draw:auto-grow-height=\"false\" />"
     << std::endl
     << "</style:style>" << std::endl;

  // Text styles
  // Label
  os << "<!-- Text styles -->" << std::endl
     << "<!-- label -->" << std::endl
     << "<style:style style:name=\"tlabel\" style:family=\"paragraph\">"
     << std::endl
     << "<style:paragraph-properties fo:text-align=\"center\"/>" << std::endl
     << "<style:text-properties fo:font-family=\""
     << cfg.getString("label.fontFamily") << "\" fo:font-size=\""
     << cfg.getString("label.fontSize") << "\" style:font-size-asian=\""
     << cfg.getString("label.fontSize") << "\" style:font-size-complex=\""
     << cfg.getString("label.fontSize") << "\" fo:font-weight=\""
     << cfg.getString("label.fontWeight") << "\"/>" << std::endl
     << "</style:style>" << std::endl;

  // Entity
  os << "<!-- Entity -->" << std::endl
     << "<style:style style:name=\"tentity\" style:family=\"paragraph\">"
     << std::endl
     << "<style:paragraph-properties fo:text-align=\"center\"/>" << std::endl
     << "<style:text-properties fo:font-family=\""
     << cfg.getString("entity.fontFamily") << "\" fo:font-size=\""
     << cfg.getString("entity.fontSize") << "\" style:font-size-asian=\""
     << cfg.getString("entity.fontSize") << "\" style:font-size-complex=\""
     << cfg.getString("entity.fontSize") << "\" fo:font-weight=\""
     << cfg.getString("entity.fontWeight") << "\"/>" << std::endl
     << "</style:style>" << std::endl;

  // Left side port
  os << "<!-- Left side port -->" << std::endl
     << "<style:style style:name=\"tlsport\" style:family=\"paragraph\">"
     << std::endl
     << "<style:paragraph-properties fo:text-align=\"start\"/>" << std::endl
     << "<style:text-properties fo:font-family=\""
     << cfg.getString("port.fontFamily") << "\" fo:font-size=\""
     << cfg.getString("port.fontSize") << "\" style:font-size-asian=\""
     << cfg.getString("port.fontSize") << "\" style:font-size-complex=\""
     << cfg.getString("port.fontSize") << "\" fo:font-weight=\""
     << cfg.getString("port.fontWeight") << "\"/>" << std::endl
     << "</style:style>" << std::endl;

  // Right side port
  os << "<!-- Right side port -->" << std::endl
     << "<style:style style:name=\"trsport\" style:family=\"paragraph\">"
     << std::endl
     << "<style:paragraph-properties fo:text-align=\"end\"/>" << std::endl
     << "<style:text-properties fo:font-family=\""
     << cfg.getString("port.fontFamily") << "\" fo:font-size=\""
     << cfg.getString("port.fontSize") << "\" style:font-size-asian=\""
     << cfg.getString("port.fontSize") << "\" style:font-size-complex=\""
     << cfg.getString("port.fontSize") << "\" fo:font-weight=\""
     << cfg.getString("port.fontWeight") << "\"/>" << std::endl
     << "</style:style>" << std::endl;

  // Center port
  os << "<!-- Center port -->" << std::endl
     << "<style:style style:name=\"tcport\" style:family=\"paragraph\">"
     << std::endl
     << "<style:paragraph-properties fo:text-align=\"center\"/>" << std::endl
     << "<style:text-properties fo:font-family=\""
     << cfg.getString("port.fontFamily") << "\" fo:font-size=\""
     << cfg.getString("port.fontSize") << "\" style:font-size-asian=\""
     << cfg.getString("port.fontSize") << "\" style:font-size-complex=\""
     << cfg.getString("port.fontSize") << "\" fo:font-weight=\""
     << cfg.getString("port.fontWeight") << "\"/>" << std::endl
     << "</style:style>" << std::endl;

  // Vector length centered
  os
    << "<!-- Vector length left side -->" << std::endl
    << "<style:style style:name=\"vector_centered\" style:family=\"paragraph\">"
    << std::endl
    << "<style:paragraph-properties fo:text-align=\"center\"/>" << std::endl
    << "<style:text-properties fo:font-family=\""
    << cfg.getString("vector.fontFamily") << "\" fo:font-size=\""
    << cfg.getString("vector.fontSize") << "\" style:font-size-asian=\""
    << cfg.getString("vector.fontSize") << "\" style:font-size-complex=\""
    << cfg.getString("vector.fontSize") << "\" fo:font-weight=\""
    << cfg.getString("vector.fontWeight") << "\"/>" << std::endl
    << "</style:style>" << std::endl;

  // Vector length left side
  os << "<!-- Vector length left side -->" << std::endl
     << "<style:style style:name=\"vectorls\" style:family=\"paragraph\">"
     << std::endl
     << "<style:paragraph-properties fo:text-align=\"end\"/>" << std::endl
     << "<style:text-properties fo:font-family=\""
     << cfg.getString("vector.fontFamily") << "\" fo:font-size=\""
     << cfg.getString("vector.fontSize") << "\" style:font-size-asian=\""
     << cfg.getString("vector.fontSize") << "\" style:font-size-complex=\""
     << cfg.getString("vector.fontSize") << "\" fo:font-weight=\""
     << cfg.getString("vector.fontWeight") << "\"/>" << std::endl
     << "</style:style>" << std::endl;

  // Vector length right side
  os << "<!-- Vector length right side -->" << std::endl
     << "<style:style style:name=\"vectorrs\" style:family=\"paragraph\">"
     << std::endl
     << "<style:paragraph-properties fo:text-align=\"start\"/>" << std::endl
     << "<style:text-properties fo:font-family=\""
     << cfg.getString("vector.fontFamily") << "\" fo:font-size=\""
     << cfg.getString("vector.fontSize") << "\" style:font-size-asian=\""
     << cfg.getString("vector.fontSize") << "\" style:font-size-complex=\""
     << cfg.getString("vector.fontSize") << "\" fo:font-weight=\""
     << cfg.getString("vector.fontWeight") << "\"/>" << std::endl
     << "</style:style>" << std::endl;

  // Generics
  os << "<!-- generics length right side -->" << std::endl
     << "<style:style style:name=\"generics\" style:family=\"paragraph\">"
     << std::endl
     << "<style:paragraph-properties fo:text-align=\"start\"/>" << std::endl
     << "<style:text-properties fo:font-family=\""
     << cfg.getString("genericSignal.fontFamily") << "\" fo:font-size=\""
     << cfg.getString("genericSignal.fontSize") << "\" style:font-size-asian=\""
     << cfg.getString("genericSignal.fontSize")
     << "\" style:font-size-complex=\""
     << cfg.getString("genericSignal.fontSize") << "\" fo:font-weight=\""
     << cfg.getString("genericSignal.fontWeight") << "\"/>" << std::endl
     << "</style:style>" << std::endl;

  os << "</office:automatic-styles>" << std::endl;

  return;
}

void
FODGWriter::FODGPage(double width, double height)
{
  // Default declarations
  os << " <office:automatic-styles>" << std::endl
     << "  <!-- page layout -->" << std::endl
     << "  <style:page-layout style:name=\"PM0\">" << std::endl
     << "   <style:page-layout-properties fo:margin-top=\"0cm\" "
        "fo:margin-bottom=\"0cm\" fo:margin-left=\"0cm\" "
        "fo:margin-right=\"0cm\" fo:page-width=\""
     << width << "cm\" fo:page-height=\"" << height
     << "cm\" style:print-orientation=\"portrait\"/>" << std::endl
     << "  </style:page-layout>" << std::endl
     << "  <!-- page style ?? -->" << std::endl
     << "  <style:style style:name=\"dp1\" style:family=\"drawing-page\">"
     << std::endl
     << "   <style:drawing-page-properties draw:background-size=\"border\" "
        "draw:fill=\"none\"/>"
     << std::endl
     << "  </style:style>" << std::endl
     << "  <!-- page style ?? -->" << std::endl
     << "  <style:style style:name=\"dp2\" style:family=\"drawing-page\"/>"
     << std::endl;

  return;
}

void
FODGWriter::circle(double x, double y, double diameter, std::string shapeID)
{
  os << "\t<draw:custom-shape draw:style-name=\"" << shapeID
     << "\" draw:layer = \"layout\" svg:width=\"" << diameter
     << "cm\" svg:height=\"" << diameter << "cm\" svg:x=\""
     << x - (diameter / 2) << "cm\" svg:y=\"" << y - (diameter / 2) << "cm\">"
     << std::endl
     << "\t\t<draw:enhanced-geometry draw:type=\"ellipse\" "
        "draw:enhanced-path=\"U 10800 10800 10800 10800 0 360 Z N\"/>"
     << std::endl
     << "\t</draw:custom-shape>" << std::endl;

  return;
}