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
 * \file  EPIG/Graphics.cpp
 */

#include "EPIG/Graphics.hpp"
#include <stdexcept>

using namespace EPIG;
using namespace EPIG::XMLTree;


// Graphic Attribute

const std::string Graphic::cElementName = "";

const std::string Graphic::cXAttributeName = "x";
const std::string Graphic::cYAttributeName = "y";
const std::string Graphic::cWidthAttributeName = "width";
const std::string Graphic::cHeightAttributeName = "height";
const std::string Graphic::cTransformAttributeName = "transform";

const String Graphic::cSVGNamespace = "http://www.w3.org/2000/svg";

void Graphic::addStyle(const Style &inStyle) {
    setAttributes(inStyle);
}

void Graphic::setTransform(const Transform &inTransform) {
    setAttribute(cTransformAttributeName, inTransform);
}
void Graphic::clearTransform() {
    removeAttribute(cTransformAttributeName);
}


// Group Graphic Attribute

const std::string Group::cElementName = "g";

Group::Group(const Style &inStyle) : Graphic(cElementName, inStyle) { }

Group::Group(const std::string &inName, const AttributeList &inAttributes)
: Graphic(inName, inAttributes) { }

Group &Group::operator << (const Graphic &inGraphic) {
    Element::operator << (inGraphic);
    return *this;
}

unsigned Group::count() const {
    return Element::getChildCount();
}

void Group::clear() {
    Element::setContent(NodeList());
}

void Group::checkElement(const Node *inElement, 
                         const std::string &inRequestedElementName) {
	if ( inElement == 0 ) {
        throw BadIdentifierException("EPIG::Group::checkElement(): "
                            "Could not find element with this identifier.");
    } else if ( inElement->getNodeType() != Element::cNodeType ||
                ( inRequestedElementName.length() > 0 && 
                  ((Element *)inElement)->getName() != inRequestedElementName ) 
              ) {
        throw IncompatibleClassException("EPIG::Group::checkElement(): "
                            "Node is incompatible with requested class.");
	}
}


// Frame

const std::string Frame::cElementName = "svg";

Frame::Frame(const Point &inOrigin, const Size &inSize, const Style &inStyle)
: Group(cElementName,
          Attribute(cXAttributeName, String(inOrigin.x)) +
          Attribute(cYAttributeName, String(inOrigin.y)) +
          Attribute(cWidthAttributeName, String(inSize.width)) +
          Attribute(cHeightAttributeName, String(inSize.height)) +
          inStyle) 
{ 
	setAttribute(cXMLNamespaceAttributeName, cSVGNamespace);		  
}

Point Frame::getOrigin() const {
    return Point(getAttribute(cXAttributeName).getFloatValue(),
                 getAttribute(cYAttributeName).getFloatValue());
}
void Frame::setOrigin(const Point &inOrigin) {
    setAttribute(cXAttributeName, String(inOrigin.x));
    setAttribute(cYAttributeName, String(inOrigin.y));
}
void Frame::setOrigin(float inX, float inY) {
    setAttribute(cXAttributeName, String(inX));
    setAttribute(cYAttributeName, String(inY));
}

Size Frame::getSize() const {
    return Size(getAttribute(cWidthAttributeName).getFloatValue(),
             getAttribute(cHeightAttributeName).getFloatValue());
}
void Frame::setSize(const Size &inSize) {
    setAttribute(cWidthAttributeName, String(inSize.width));
    setAttribute(cHeightAttributeName, String(inSize.height));
}
void Frame::setSize(float inWidth, float inHeight) {
    setAttribute(cWidthAttributeName, String(inWidth));
    setAttribute(cHeightAttributeName, String(inHeight));
}

// SVGDocument

const std::string SVGDocument::cDocType = 
  "svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\" \n" 
  "              \"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\"";

SVGDocument::SVGDocument(const std::string &inTitle, const Frame &inRootFrame, 
                         const std::string &inEncoding)
: Document(inRootFrame, cDocType, inEncoding) {
    convertToCartesian();
    addTitle(inTitle);
}

SVGDocument::SVGDocument(const std::string &inTitle, const Frame &inRootFrame, 
                         bool inIsCartesian, const std::string &inEncoding)
: Document(inRootFrame, cDocType, inEncoding) {
    if ( inIsCartesian ) convertToCartesian();
    addTitle(inTitle);
}

void SVGDocument::addTitle(const std::string &inTitle) {
    // Create title element
    Element lTitleElement("title");
    lTitleElement << String(inTitle);
    // Append it to the start of the document's root frame content.
    mRoot.setContent(lTitleElement + mRoot.getContent());
}
void SVGDocument::convertToCartesian() {
    // Add transform for root element content.
    // Must place elements into a group.
    Element lContentGroup("g");
    lContentGroup.setAttribute("transform", 
        "scale(1, -1) translate(0, -"+std::string(mRoot.getAttribute("height"))+")");
    ((Element &)lContentGroup).setContent(mRoot.getContent());
    mRoot.setContent(lContentGroup);
    
    // "Unflip" text elements recursively.
    unflip(mRoot);
}
void SVGDocument::unflip(Element &inElement) {
    // Pass over all elements to "unflip" text.
    if ( inElement.getName() == "text" ) {
        float y = inElement.getAttribute("y").getFloatValue();
        inElement.setAttribute("y", String(-y));
        inElement.appendToAttribute("transform", " scale(1, -1)");
    }
    
    for ( unsigned i = 0; i < inElement.getChildCount(); i++ ) {
        Node *lNode = inElement.getChild(i);
        if ( lNode->getNodeType() == eElement )
            unflip((Element &)*lNode);
    }
}
