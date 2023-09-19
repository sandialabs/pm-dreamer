/*
 * EPIG - The Easy Programming Interface for Graphics
 * Copyright (C) 2003  Michel Fortin, Jean-Phillipe Lebel, Marie-Eve Tremblay
 * http://www.gel.ulaval.ca/~epig/
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/*!
 * \file  EPIG/Types.cpp
 */

#include "EPIG/Types.hpp"

using namespace EPIG;
using namespace EPIG::XMLTree;

/* Color constants */
const Color Color::cAliceBlue("aliceblue");
const Color Color::cAntiqueWhite("antiquewhite");
const Color Color::cAqua("aqua");
const Color Color::cAquaMarine("aquamarine");
const Color Color::cAzure("azure");
const Color Color::cBeige("beige");
const Color Color::cBisque("bisque");
const Color Color::cBlack("black");
const Color Color::cBlacheDalmond("blanchedalmond");
const Color Color::cBlue("blue");
const Color Color::cBlueViolet("blueviolet");
const Color Color::cBrown("brown");
const Color Color::cBurlyWood("burlywood");
const Color Color::cCadetBlue("cadetblue");
const Color Color::cChartreuse("chartreuse");
const Color Color::cChocolate("chocolate");
const Color Color::cCoral("coral");
const Color Color::cCornFlowerBlue("cornflowerblue");
const Color Color::cCornSilk("cornsilk");
const Color Color::cCrimson("crimson");
const Color Color::cCyan("cyan");
const Color Color::cDarkBlue("darkblue");
const Color Color::cDarkCyan("darkcyan");
const Color Color::cDarkGoldenRod("darkgoldenrod");
const Color Color::cDarkGray("darkgray");
const Color Color::cDarkGreen("darkgreen");
const Color Color::cDarkGrey("darkgrey");
const Color Color::cDarkKhaki("darkkhaki");
const Color Color::cDarkMagenta("darkmagenta");
const Color Color::cDarkOliveGreen("darkolivegreen");
const Color Color::cDarkOrange("darkorange");
const Color Color::cDarkOrchid("darkorchid");
const Color Color::cDarkRed("darkred");
const Color Color::cDarkSalmon("darksalmon");
const Color Color::cDarkSeaGreen("darkseagreen");
const Color Color::cDarkSlateBlue("darkslateblue");
const Color Color::cDarkSlateGray("darkslategray");
const Color Color::cDarkSlateGrey("darkslategrey");
const Color Color::cDarkTurquoise("darkturquoise");
const Color Color::cDarkViolet("darkviolet");
const Color Color::cDeepPink("deeppink");
const Color Color::cDeepSkyBlue("deepskyblue");
const Color Color::cDimGray("dimgray");
const Color Color::cDimGrey("dimgrey");
const Color Color::cDodgerBlue("dodgerblue");
const Color Color::cFireBrick("firebrick");
const Color Color::cFloralWhite("floralwhite");
const Color Color::cForestGreen("forestgreen");
const Color Color::cFuchsia("fuchsia");
const Color Color::cGainsboro("gainsboro");
const Color Color::cGostWhite("ghostwhite");
const Color Color::cGold("gold");
const Color Color::cGoldenRod("goldenrod");
const Color Color::cGray("gray");
const Color Color::cGrey("grey");
const Color Color::cGreen("green");
const Color Color::cGreenYellow("greenyellow");
const Color Color::cHoneyDew("honeydew");
const Color Color::cHotPink("hotpink");
const Color Color::cIndianRed("indianred");
const Color Color::cIndigo("indigo");
const Color Color::cIvory("ivory");
const Color Color::cKhaki("khaki");
const Color Color::cLavender("lavender");
const Color Color::cLavenderBlush("lavenderblush");
const Color Color::cLawnGreen("lawngreen");
const Color Color::cLemonChiffon("lemonchiffon");
const Color Color::cLightBlue("lightblue");
const Color Color::cLightCoral("lightcoral");
const Color Color::cLightCyan("lightcyan");
const Color Color::cLightGolderRodYellow("lightgoldenrodyellow");
const Color Color::cLightGray("lightgray");
const Color Color::cLightGreen("lightgreen");
const Color Color::cLightGrey("lightgrey");
const Color Color::cLigntPink("lightpink");
const Color Color::cLightSalmon("lightsalmon");
const Color Color::cLightSeaGreen("lightseagreen");
const Color Color::cLightSkyBlue("lightskyblue");
const Color Color::cLightSlateGray("lightslategray");
const Color Color::cLightSlateGrey("lightslategrey");
const Color Color::cLightSteelBlue("lightsteelblue");
const Color Color::cLightYellow("lightyellow");
const Color Color::cLime("lime");
const Color Color::cLimeGreen("limegreen");
const Color Color::cLinen("linen");
const Color Color::cMagenta("magenta");
const Color Color::cMaroon("maroon");
const Color Color::cMediumAquaMarine("mediumaquamarine");
const Color Color::cMediumBlue("mediumblue");
const Color Color::cMediumOrchid("mediumorchid");
const Color Color::cMediumPurple("mediumpurple");
const Color Color::cMeidumSeaGreen("mediumseagreen");
const Color Color::cMediumSlateBlue("mediumslateblue");
const Color Color::cMediumSpringGreen("mediumspringgreen");
const Color Color::cMediumTurquoise("mediumturquoise");
const Color Color::cMediumVioletRed("mediumvioletred");
const Color Color::cModNightBlue("midnightblue");
const Color Color::cMintCream("mintcream");
const Color Color::cMistyRose("mistyrose");
const Color Color::cMoccasin("moccasin");
const Color Color::cNavajoWhite("navajowhite");
const Color Color::cNavy("navy");
const Color Color::cOldLace("oldlace");
const Color Color::cOlive("olive");
const Color Color::cOliveDrab("olivedrab");
const Color Color::cOrange("orange");
const Color Color::cOrangered("orangered");
const Color Color::cOrchid("orchid");
const Color Color::cPaleGoldenRod("palegoldenrod");
const Color Color::cPaleGreen("palegreen");
const Color Color::cPaleTurquoise("paleturquoise");
const Color Color::cPaleVioletRed("palevioletred");
const Color Color::cPapayaWhip("papayawhip");
const Color Color::cPeachPuff("peachpuff");
const Color Color::cPeru("peru");
const Color Color::cPink("pink");
const Color Color::cPlum("plum");
const Color Color::cPowderBlue("powderblue");
const Color Color::cPurple("purple");
const Color Color::cRed("red");
const Color Color::cRosyBrown("rosybrown");
const Color Color::cRoyalBlue("royalblue");
const Color Color::cSaddleBrown("saddlebrown");
const Color Color::cSalmon("salmon");
const Color Color::cSandyBrown("sandybrown");
const Color Color::cSeaGreen("seagreen");
const Color Color::cSeaShell("seashell");
const Color Color::cSienna("sienna");
const Color Color::cSilver("silver");
const Color Color::cSkyBlue("skyblue");
const Color Color::cSlateBlue("slateblue");
const Color Color::cSlateGray("slategray");
const Color Color::cSlateGrey("slategrey");
const Color Color::cSnow("snow");
const Color Color::cSpringGreen("springgreen");
const Color Color::cSteelBlue("steelblue");
const Color Color::cTan("tan");
const Color Color::cTeal("teal");
const Color Color::cThistle("thistle");
const Color Color::cTomato("tomato");
const Color Color::cTurquoise("turquoise");
const Color Color::cViolet("violet");
const Color Color::cWheat("wheat");
const Color Color::cWhite("white");
const Color Color::cWhiteSmoke("whitesmoke");
const Color Color::cYellow("yellow");
const Color Color::cYellowGreen("yellowgreen");
const Color Color::cNone("none");

/*!
 * Function that output a string in the form "rgb(10%, 12%, 100%)" representing
 * a SVG Color.
 */
String makeRGBColorString(float inRed, float inGreen, float inBlue) {
    static const String cStart = "rgb(";
    static const String cSeparator = "%, ";
    static const String cEnd = "%)";
    return cStart + String(inRed*100) + 
           cSeparator + String(inGreen*100) + 
           cSeparator + String(inBlue*100) + cEnd;
}

RGBColor::RGBColor(float inRed, float inGreen, float inBlue) : Color(makeRGBColorString(inRed, inGreen, inBlue)) { }


// Point

PointList Point::operator + (const PointList &inPoints) const {
    return PointList(*this) += inPoints;
}
PointList Point::operator + (const Point &inPoint) const {
    return PointList(*this) += inPoint;
}

String Point::getStringValue() const {
    static const String cslCoordinateSeparator = ",";
    return String(x) + cslCoordinateSeparator + String(y);
}


// Size

String Size::getStringValue() const {
    static const String cslCoordinateSeparator = ",";
    return String(width) + cslCoordinateSeparator + String(height);
}


// PointList


PointList::PointList() { }

PointList::PointList(const Point inPoint)
: std::vector<Point>(1, inPoint) { }

PointList PointList::operator + (const PointList &inLinePath) const {
    return PointList(*this) += inLinePath;
}

PointList PointList::operator + (const Point &inLinePath) const {
    return PointList(*this) += inLinePath;
}

PointList &PointList::operator += (const PointList &inLinePath) {
    for ( unsigned i = 0; i < inLinePath.size(); i++ )
        push_back(inLinePath[i]);
    return *this;
}

PointList &PointList::operator += (const Point &inPoint) {
    push_back(inPoint);
    return *this;
}

String PointList::getStringValue() const {
    if ( size() == 0 ) return String();
    
    static const String cslComponentSeparator = " ";

    String lString((*this)[0].getStringValue());
    for ( unsigned i = 1; i < size(); i++ )
        lString +=  cslComponentSeparator + (*this)[i].getStringValue();
    return lString;
}

