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
 * \file  EPIG/Transforms.cpp
 */

#include "EPIG/Transforms.hpp"
#include <sstream>

using namespace EPIG;
using namespace EPIG::XMLTree;


// Transform Attribute

const String Transform::cArgumentSeparator(' ');

Transform::Transform(const String &inName, const String &inValue)
: String(makeTransformString(inName, inValue)) { }

Transform Transform::operator + (const Transform &inTransform) const {
    return Transform(*this) += inTransform;
}

Transform &Transform::operator += (const Transform &inTransform) {
    static const String cslSeparator = " ";
    String::operator += (cslSeparator + inTransform);
    return *this;
}

String Transform::makeTransformString(const String &inName, 
                                      const String &inValue) {
    const static String cslOpen = "(";
    const static String cslClose = ")";
    return inName + (cslOpen + inValue + cslClose);
}


// Rotate Transform

const String Rotate::cTransformName = "rotate";

Rotate::Rotate(float inAngle)
: Transform(cTransformName,
            String(inAngle)) { }

Rotate::Rotate(float inAngle, float inX, float inY)
: Transform(cTransformName,
            String(inAngle) + cArgumentSeparator +
            String(inX) + cArgumentSeparator +
            String(inY)) { }

Rotate::Rotate(float inAngle, const Point &inPoint)
: Transform(cTransformName,
            String(inAngle) + cArgumentSeparator +
            String(inPoint.x) + cArgumentSeparator +
            String(inPoint.y)) { }


// Translate Transform

const String Translate::cTransformName = "translate";

Translate::Translate(float inX, float inY)
: Transform(cTransformName,
            String(inX) + cArgumentSeparator +
            String(inY)) { }


// Scale Transform

const String Scale::cTransformName = "scale";

Scale::Scale(float inX, float inY)
: Transform(cTransformName,
            String(inX) + cArgumentSeparator +
            String(inY)) { }

Scale::Scale(float inScale)
: Transform(cTransformName,
            String(inScale)) { }


// Skew on X Axis Transform

const String SkewX::cTransformName = "skewX";

SkewX::SkewX(float inAngle)
: Transform(cTransformName,
            String(inAngle)) { }


// Skew on Y Axis Transform

const String SkewY::cTransformName = "skewY";

SkewY::SkewY(float inAngle) :
Transform(cTransformName,
          String(inAngle)) { }


// Matrix Transform

const String MatrixTransform::cTransformName = "matrix";

MatrixTransform::MatrixTransform(float inA, float inB, float inC, 
                                 float inD, float inE, float inF)
: Transform(cTransformName,
            String(inA) + cArgumentSeparator +
            String(inB) + cArgumentSeparator +
            String(inC) + cArgumentSeparator +
            String(inD) + cArgumentSeparator +
            String(inE) + cArgumentSeparator +
            String(inF)) { }
