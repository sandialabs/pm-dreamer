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
 * \file  EPIG/Path.cpp
 */

#include "EPIG/Path.hpp"

namespace EPIG {
using namespace EPIG::XMLTree;

// Path Graphic Attribute

const String Path::cElementName = "path";
const String Path::cDataAttributeName('d');

Path::Path(const Style &inStyle) : Graphic(cElementName, inStyle) { }

Path::Path(const PathData &inCommands, const Style &inStyle)
: Graphic(cElementName,
          Attribute(cDataAttributeName, inCommands) +
          inStyle) { }

Path Path::operator + (const PathData &inCommands) const {
    return Path(*this) += inCommands;
}

Path &Path::operator += (const PathData &inCommands) {
    String lDefinition = getAttribute(cDataAttributeName);
    lDefinition += PathData::cSpaceSeparator + inCommands;
    setAttribute(cDataAttributeName, lDefinition);
    return *this;
}


// Path Component

const String PathData::cSpaceSeparator(' ');
const String PathData::cCommaSeparator(',');

PathData::PathData(const String &inValue) : String(inValue) { }

PathData PathData::operator + (const PathData &inCommands) {
    return PathData((String &)*this + cSpaceSeparator + inCommands);
}


// MoveTo Path Component

const String MoveTo::cCommandName('M');

MoveTo::MoveTo(float inX, float inY) 
: PathData(cCommandName + Point(inX, inY).getStringValue()) { }

MoveTo::MoveTo(const Point &inPoint) 
: PathData(cCommandName + inPoint.getStringValue()) { }


// LineTo Path Component

const String LineTo::cCommandName('L');

LineTo::LineTo(float inX, float inY) 
: PathData(cCommandName + Point(inX, inY).getStringValue()) { }
            
LineTo::LineTo(const Point &inPoint) 
: PathData(cCommandName + inPoint.getStringValue()) { }


// ClosePath Path Component

const String ClosePath::cCommandName('z');

ClosePath::ClosePath() : PathData(cCommandName) { }


// CubicCurveTo Path Component

const String CubicCurveTo::cCommandName('C');
const String CubicCurveTo::cSplineComponentName('S');

CubicCurveTo::CubicCurveTo(const Point &inPoint, const Point &inControlPoint1, 
                           const Point &inControlPoint2)
: PathData(cCommandName +
       inControlPoint1.getStringValue() + cSpaceSeparator +
       inControlPoint2.getStringValue() + cSpaceSeparator +
       inPoint.getStringValue()) { }

CubicCurveTo::CubicCurveTo(const Point &inPoint, const Point &inControlPoint)
: PathData(cSplineComponentName +
       inControlPoint.getStringValue() + cSpaceSeparator +
       inPoint.getStringValue()) { }


// QuadraticCurveTo Path Component

const String QuadraticCurveTo::cCommandName('Q');
const String QuadraticCurveTo::cShorthandComponentName('T');

QuadraticCurveTo::QuadraticCurveTo(const Point &inPoint, const Point &inControlPoint)
: PathData(cCommandName +
       inControlPoint.getStringValue() + cSpaceSeparator +
       inPoint.getStringValue()) { }

QuadraticCurveTo::QuadraticCurveTo(const Point &inPoint)
: PathData(cShorthandComponentName + inPoint.getStringValue()) { }

QuadraticCurveTo::QuadraticCurveTo(float inX, float inY)
: PathData(cShorthandComponentName + Point(inX, inY).getStringValue()) { }


// EllipticalArcTo Path Component

const String EllipticalArcTo::cCommandName('A');
const String EllipticalArcTo::cLogicalTrue('1');
const String EllipticalArcTo::cLogicalFalse('0');

EllipticalArcTo::EllipticalArcTo(const Point &inPoint, float inXRadius, 
                                 float inYRadius, float inXAxisRotation, 
                                 bool inLargeArc, bool inSweep)
: PathData(cCommandName +
       String(inXRadius) + cCommaSeparator + 
       String(inYRadius) + cSpaceSeparator +
       String(inXAxisRotation) + cSpaceSeparator +
       ( inLargeArc ? cLogicalTrue : cLogicalFalse ) + cSpaceSeparator +
       ( inSweep ? cLogicalTrue : cLogicalFalse ) + cSpaceSeparator +
       inPoint.getStringValue()) { }

} // namespace EPIG
