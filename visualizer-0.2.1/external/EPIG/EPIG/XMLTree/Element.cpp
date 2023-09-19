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
 * \file  EPIG/XMLTree/Element.cpp
 */

#include "EPIG/XMLTree/Element.hpp"

using namespace EPIG::XMLTree;

// Element

const NodeType Element::cNodeType = eElement;

const std::string Element::cIdentifierAttributeName = "id";
const std::string Element::cXMLNamespaceAttributeName = "xmlns";

Element::Element(const std::string &inName, const AttributeList &inAttributes) 
: Node(new Data(inName, inAttributes)) { }

Element::Element(AbstractData *inData) : Node(inData) { }

std::string Element::getName() const {
    return getData().getName();
}

String Element::getIdentifier() const {
    return getAttribute(cIdentifierAttributeName);
}
void Element::setIdentifier(const String &inIdentifier) {
    setAttribute(cIdentifierAttributeName, inIdentifier);
}
void Element::clearIdentifier() {
    removeAttribute(cIdentifierAttributeName);
}

bool Element::hasAttribute(const std::string &inName) const {
    return getData().hasAttribute(inName);
}
String Element::getAttribute(const std::string &inName) const {
    return getData().getAttribute(inName);
}
void Element::setAttribute(const std::string &inName, const String &inValue) {
    getData().setAttribute(inName, inValue);
}
void Element::setAttributes(const AttributeList &inAttributes) {
    getData().setAttributes(inAttributes);
}
void Element::appendToAttribute(const std::string &inName, 
                                const String &inAddition) {
    getData().appendToAttribute(inName, inAddition);
}
void Element::removeAttribute(const std::string &inName) {
    getData().removeAttribute(inName);
}

Element &Element::operator << (const Node &inNode) {
    getData().append(inNode);
    return *this;
}

NodeList Element::getContent() const {
    return getData().getContent();
}
AttributeList Element::getAttributes() const {
    return getData().getAttributes();
}

void Element::setContent(const NodeList &inContent) {
    getData().setContent(inContent);
}

unsigned Element::getChildCount() const {
    return getData().getChildCount();
}

Element *Element::getChild(const String &inIdentifier) {
    return getData().getChild(inIdentifier);
}
const Element *Element::getChild(const String &inIdentifier) const {
    return getData().getChild(inIdentifier);
}
        
Node *Element::getChild(unsigned inIndex) {
    return getData().getChild(inIndex);
}
const Node *Element::getChild(unsigned inIndex) const {
    return getData().getChild(inIndex);
}


// Element data

Element::Data::Data(const std::string &inName, const AttributeList &inAttributes) 
: mName(inName), mAttributes(inAttributes) { }

std::string Element::Data::getName() const {
    return mName;
}

bool Element::Data::hasAttribute(const std::string &inName) const {
    return mAttributes.defined(inName);
}
String Element::Data::getAttribute(const std::string &inName) const {
    return mAttributes.get(inName);
}
void Element::Data::setAttribute(const std::string &inName, 
                                 const String &inValue) {
    mAttributes[inName] = inValue;
}
void Element::Data::setAttributes(const AttributeList &inAttributes) {
    mAttributes += inAttributes;
}
void Element::Data::appendToAttribute(const std::string &inName, 
                                      const String &inAddition) {
    mAttributes[inName] += inAddition;
}

void Element::Data::removeAttribute(const std::string &inName) {
    mAttributes.erase(inName);
}

void Element::Data::append(const Node &inNode) {
    mContent += inNode;
}

NodeList Element::Data::getContent() const {
    return mContent;
}
AttributeList Element::Data::getAttributes() const {
    return mAttributes;
}

void Element::Data::setContent(const NodeList &inContent) {
    mContent = inContent;
}

unsigned Element::Data::getChildCount() const {
    return (unsigned)mContent.size();
}

Element *Element::Data::getChild(const String &inIdentifier) {
    return mContent.getElementById(inIdentifier);
}
const Element *Element::Data::getChild(const String &inIdentifier) const {
    return mContent.getElementById(inIdentifier);
}
        
Node *Element::Data::getChild(unsigned inIndex) {
    return &mContent.at(inIndex);
}
const Node *Element::Data::getChild(unsigned inIndex) const {
    return &mContent.at(inIndex);
}

void Element::Data::write(std::ostream &inStream) const {
    inStream << "<" << mName;
    {
        AttributeList::const_iterator i = mAttributes.begin();
        while ( i != mAttributes.end() ) {
            inStream << " " << i->first << "=\"" << i->second << "\"";
            i++;
        }
    }
    if ( mContent.size() == 0 ) {
		inStream << "/>";
    } else {
		inStream << ">";
        /*
        NodeList::const_iterator i = mContent.begin();
        while ( i != mContent.end() )
            (i++)->write(inStream);
        */
        for ( unsigned j = 0; j < mContent.size(); j++ ) {
            const Node &n = mContent[j];
            n.write(inStream);
        }
		inStream << "</" << mName << ">";
    }
}

Element::Data *Element::Data::clone() const {
    return new Data(*this);
}
