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
 * \file  EPIG/CommonShapes.cpp
 */

#include "EPIG/CommonShapes.hpp"

namespace EPIG {
using namespace EPIG::XMLTree;


// Rectangle Graphic Attribute

const String Rectangle::cElementName = "rect";

Rectangle::Rectangle(const Point &inOrigin, const Size &inSize, 
                     const Style &inStyle)
: Graphic(cElementName,
          Attribute(cXAttributeName, String(inOrigin.x)) +
          Attribute(cYAttributeName, String(inOrigin.y)) +
          Attribute(cWidthAttributeName, String(inSize.width)) +
          Attribute(cHeightAttributeName, String(inSize.height)) +
          inStyle) { }

Point Rectangle::getOrigin() const {
    return Point(getAttribute(cXAttributeName).getFloatValue(),
              getAttribute(cYAttributeName).getFloatValue());
}
void Rectangle::setOrigin(const Point &inOrigin) {
    setAttribute(cXAttributeName, String(inOrigin.x));
    setAttribute(cYAttributeName, String(inOrigin.y));
}
void Rectangle::setOrigin(float inX, float inY) {
    setAttribute(cXAttributeName, String(inX));
    setAttribute(cYAttributeName, String(inY));
}

Size Rectangle::getSize() const {
    return Size(getAttribute(cWidthAttributeName).getFloatValue(),
             getAttribute(cHeightAttributeName).getFloatValue());
}
void Rectangle::setSize(const Size &inSize) {
    setAttribute(cWidthAttributeName, String(inSize.width));
    setAttribute(cHeightAttributeName, String(inSize.height));
}
void Rectangle::setSize(float inWidth, float inHeight) {
    setAttribute(cWidthAttributeName, String(inWidth));
    setAttribute(cHeightAttributeName, String(inHeight));
}


// Circle Graphic Attribute

const String Circle::cElementName = "circle";
const String Circle::cCenterXAttributeName = "cx";
const String Circle::cCenterYAttributeName = "cy";
const String Circle::cRadiusAttributeName = "r";

Circle::Circle(const Point &inCenter, float inRadius, const Style &inStyle)
: Graphic(cElementName,
          Attribute(cCenterXAttributeName, String(inCenter.x)) +
          Attribute(cCenterYAttributeName, String(inCenter.y)) +
          Attribute(cRadiusAttributeName, String(inRadius)) +
          inStyle) { }

Point Circle::getCenter() const {
    return Point(getAttribute(cCenterXAttributeName).getFloatValue(),
              getAttribute(cCenterYAttributeName).getFloatValue());
}
void Circle::setCenter(const Point &inCenter) {
    setAttribute(cCenterXAttributeName, String(inCenter.x));
    setAttribute(cCenterYAttributeName, String(inCenter.y));
}
void Circle::setCenter(float inCX, float inCY) {
    setAttribute(cCenterXAttributeName, String(inCX));
    setAttribute(cCenterYAttributeName, String(inCY));
}

float Circle::getRadius() const {
    return getAttribute(cRadiusAttributeName).getFloatValue();
}
void Circle::setRadius(float inRadius) {
    setAttribute(cRadiusAttributeName, String(inRadius));
}


// Ellipse Graphic Attribute

const String Ellipse::cElementName = "ellipse";
const String Ellipse::cCenterXAttributeName = "cy";
const String Ellipse::cCenterYAttributeName = "cx";
const String Ellipse::cXRadiusAttributeName = "rx";
const String Ellipse::cYRadiusAttributeName = "ry";

Ellipse::Ellipse(const Point &inCenter, float inXRadius, float inYRadius,
                 const Style &inStyle)
: Graphic(cElementName,
          Attribute(cCenterXAttributeName, String(inCenter.x)) +
          Attribute(cCenterYAttributeName, String(inCenter.y)) +
          Attribute(cXRadiusAttributeName, String(inXRadius)) +
          Attribute(cYRadiusAttributeName, String(inYRadius)) +
          inStyle) { }

Point Ellipse::getCenter() const {
    return Point(getAttribute(cCenterXAttributeName).getFloatValue(),
                 getAttribute(cCenterYAttributeName).getFloatValue());
}
void Ellipse::setCenter(const Point &inCenter) {
    setAttribute(cCenterXAttributeName, String(inCenter.x));
    setAttribute(cCenterYAttributeName, String(inCenter.y));
}
void Ellipse::setCenter(float inCX, float inCY) {
    setAttribute(cCenterXAttributeName, String(inCX));
    setAttribute(cCenterYAttributeName, String(inCY));
}

float Ellipse::getXRadius() const {
    return getAttribute(cXRadiusAttributeName).getFloatValue();
}
void Ellipse::setXRadius(float inXRadius) {
    setAttribute(cXRadiusAttributeName, String(inXRadius));
}

float Ellipse::getYRadius() const {
    return getAttribute(cXRadiusAttributeName).getFloatValue();
}
void Ellipse::setYRadius(float inYRadius) {
    setAttribute(cYRadiusAttributeName, String(inYRadius));
}

// Line Graphic Attribute

const String Line::cElementName = "line";
const String Line::cX1AttributeName = "x1";
const String Line::cY1AttributeName = "y1";
const String Line::cX2AttributeName = "x2";
const String Line::cY2AttributeName = "y2";

Line::Line(const Point &inStart, const Point &inEnd, const Style &inStyle)
: Graphic(cElementName,
          Attribute(cX1AttributeName, String(inStart.x)) +
          Attribute(cY1AttributeName, String(inStart.y)) +
          Attribute(cX2AttributeName, String(inEnd.x)) +
          Attribute(cY2AttributeName, String(inEnd.y)) +
          inStyle) { }
          
Point Line::getStart() const {
    return Point(getAttribute(cX1AttributeName).getFloatValue(),
                 getAttribute(cY1AttributeName).getFloatValue());
}
void Line::setStart(const Point &inStart) {
    setAttribute(cX1AttributeName, String(inStart.x));
    setAttribute(cY1AttributeName, String(inStart.y));
}
void Line::setStart(float inX, float inY) {
    setAttribute(cX1AttributeName, String(inX));
    setAttribute(cY1AttributeName, String(inY));
}

Point Line::getEnd() const {
    return Point(getAttribute(cX2AttributeName).getFloatValue(),
                 getAttribute(cY2AttributeName).getFloatValue());
}
void Line::setEnd(const Point &inEnd) {
    setAttribute(cX2AttributeName, String(inEnd.x));
    setAttribute(cY2AttributeName, String(inEnd.y));
}
void Line::setEnd(float inX, float inY) {
    setAttribute(cX2AttributeName, String(inX));
    setAttribute(cY2AttributeName, String(inY));
}


// Polygon Graphic Attribute

const String Polygon::cElementName = "polygon";
const String Polygon::cPointsAttributeName = "points";

Polygon::Polygon(const Style &inStyle)
: Graphic(cElementName, inStyle) { }

Polygon::Polygon(const PointList &inLinePath, const Style &inStyle)
: Graphic(cElementName,
          Attribute(cPointsAttributeName, inLinePath.getStringValue()) +
          inStyle) { }

Polygon Polygon::operator + (const PointList &inLinePath) const {
    return Polygon(*this) += inLinePath;
}

Polygon &Polygon::operator += (const PointList &inLinePath) {
    appendToAttribute(cPointsAttributeName, inLinePath.getStringValue());
    return *this;
}

Polygon Polygon::operator + (const Point &inPoint) const {
    return Polygon(*this) += inPoint;
}

Polygon &Polygon::operator += (const Point &inPoint) {
    appendToAttribute(cPointsAttributeName,
                      String(" ") + inPoint.getStringValue());
    return *this;
}


// Polyline Graphic Attribute

const String Polyline::cElementName = "polyline";
const String Polyline::cPointsAttributeName = "points";

Polyline::Polyline(const Style &inStyle)
: Graphic(cElementName, inStyle) { }

Polyline::Polyline(const PointList &inLinePath, const Style &inStyle)
: Graphic(cElementName,
          Attribute(cPointsAttributeName, inLinePath.getStringValue()) +
          inStyle) { }

Polyline Polyline::operator + (const PointList &inLinePath) const {
    return Polyline(*this) += inLinePath;
}

Polyline &Polyline::operator += (const PointList &inLinePath) {
    appendToAttribute(cPointsAttributeName, inLinePath.getStringValue());
    return *this;
}

Polyline Polyline::operator + (const Point &inPoint) const {
    return Polyline(*this) += inPoint;
}

Polyline &Polyline::operator += (const Point &inPoint) {
    appendToAttribute(cPointsAttributeName, 
                      String(" ") + inPoint.getStringValue());
    return *this;
}

} // namespace EPIG
